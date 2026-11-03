/*****************************************************************************
** QNapi
** Copyright (C) 2008-2026 Piotr Krzemiński <pio.krzeminski@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
*****************************************************************************/

#include "ai/subtitletranslator.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

SubtitleTranslator::SubtitleTranslator(
    const QSharedPointer<LlamaServer>& server,
    const QString& sourceLanguageName, const QString& targetLanguageName,
    int chunkSize, const QString& guidelines)
    : server(server),
      sourceLanguageName(sourceLanguageName),
      targetLanguageName(targetLanguageName),
      chunkSize(qMax(1, chunkSize)),
      guidelines(guidelines) {}

Either<AiError, SrtCueList> SubtitleTranslator::translate(
    const SrtCueList& source, const QString& title,
    const std::function<void(int, int)>& progress) const {
  untranslated = 0;

  // one line per cue: the model handles line breaks badly, and the result is
  // re-wrapped for the screen anyway
  QStringList sourceTexts;
  foreach (const SrtCue& cue, source) {
    sourceTexts << QString(cue.text).replace('\n', ' ').simplified();
  }

  QStringList result;
  for (int i = 0; i < sourceTexts.size(); ++i) result << QString();

  const int total = sourceTexts.size();
  if (progress) progress(0, total);

  for (int from = 0; from < total; from += chunkSize) {
    const int to = qMin(from + chunkSize, total);
    auto r = translateRange(sourceTexts, result, from, to, title);
    if (r.is1st()) return r.as1st();
    if (progress) progress(to, total);
  }

  SrtCueList translated;
  for (int i = 0; i < source.size(); ++i) {
    SrtCue cue = source[i];
    cue.text = SrtCues::wrap(result[i]);
    translated << cue;
  }
  return translated;
}

Either<AiError, bool> SubtitleTranslator::translateRange(
    const QStringList& source, QStringList& result, int from, int to,
    const QString& title) const {
  // a little randomness on retries gets the model out of a rut
  static const double temperatures[attemptsPerChunk] = {0.2, 0.5, 0.8};

  for (int attempt = 0; attempt < attemptsPerChunk; ++attempt) {
    auto answer =
        requestChunk(source, result, from, to, title, temperatures[attempt]);

    if (answer.is1st()) {
      // a dead server will not get better by asking again
      if (!server->isRunning()) return answer.as1st();
      continue;
    }

    const QStringList translated = answer.as2nd();
    if (looksValid(source.mid(from, to - from), translated)) {
      for (int i = from; i < to; ++i) result[i] = translated[i - from];
      return true;
    }
  }

  // smaller requests are easier to get right
  if (to - from > 1) {
    const int middle = from + (to - from) / 2;
    auto first = translateRange(source, result, from, middle, title);
    if (first.is1st()) return first;
    return translateRange(source, result, middle, to, title);
  }

  // a single line the model keeps failing on: better the original than a gap
  result[from] = source[from];
  ++untranslated;
  return true;
}

Either<AiError, QStringList> SubtitleTranslator::requestChunk(
    const QStringList& source, const QStringList& result, int from, int to,
    const QString& title, double temperature) const {
  const int count = to - from;
  QString user;

  // preceding lines with their translations keep names, register and
  // grammatical gender consistent across chunk boundaries
  const bool pl = polishPrompt();

  const int ctxFrom = qMax(0, from - contextLinesBefore);
  if (ctxFrom < from) {
    user += pl ? QString::fromUtf8(
                     "Poprzednie linie, już przetłumaczone (tylko kontekst):\n")
               : QString(
                     "Previous lines, already translated (context "
                     "only):\n");
    for (int i = ctxFrom; i < from; ++i) {
      user += "- " + source[i] + "\n  -> " + result[i] + "\n";
    }
    user += "\n";
  }

  const int ctxTo = qMin(source.size(), to + contextLinesAfter);
  if (to < ctxTo) {
    user += pl ? QString::fromUtf8(
                     "Dalsze linie (tylko kontekst, nie tłumacz ich):\n")
               : QString(
                     "Lines that follow (context only, do not "
                     "translate):\n");
    for (int i = to; i < ctxTo; ++i) user += "- " + source[i] + "\n";
    user += "\n";
  }

  QJsonObject toTranslate;
  QJsonObject properties;
  QJsonArray required;
  for (int i = 0; i < count; ++i) {
    const QString key = QString::number(i + 1);
    toTranslate.insert(key, source[from + i]);
    properties.insert(key, QJsonObject{{"type", "string"}, {"minLength", 1}});
    required.append(key);
  }

  user += (pl ? QString::fromUtf8(
                    "Przetłumacz te linie napisów (%1). Odpowiedz obiektem "
                    "JSON z dokładnie tymi samymi kluczami; każdy zawiera "
                    "tłumaczenie tylko swojej linii:\n")
              : QString("Translate these %1 subtitle lines. Answer with a "
                        "JSON object with exactly the same keys, each "
                        "holding the translation of that line only:\n"))
              .arg(count);
  user += QString::fromUtf8(
      QJsonDocument(toTranslate).toJson(QJsonDocument::Indented));

  const QJsonArray messages = {
      QJsonObject{{"role", "system"}, {"content", systemPrompt(title)}},
      QJsonObject{{"role", "user"}, {"content", user}}};

  // the grammar llama-server derives from the schema makes the reply valid
  // JSON with exactly these keys; the content is checked by looksValid()
  const QJsonObject schema{{"type", "object"},
                           {"properties", properties},
                           {"required", required},
                           {"additionalProperties", false}};

  const QJsonObject extra{
      {"temperature", temperature},
      {"top_p", 0.9},
      {"max_tokens", 100 + count * 80},
      {"response_format",
       QJsonObject{{"type", "json_schema"},
                   {"json_schema",
                    QJsonObject{{"name", "subtitles"}, {"schema", schema}}}}}};

  auto reply = server->chat(messages, extra);
  if (reply.is1st()) return reply.as1st();

  QJsonParseError parseError;
  const QJsonDocument doc =
      QJsonDocument::fromJson(reply.as2nd().toUtf8(), &parseError);
  if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
    return AiError{QObject::tr("the model's answer is not valid JSON")};
  }

  const QJsonObject answer = doc.object();
  QStringList translated;
  for (int i = 0; i < count; ++i) {
    QString line = answer.value(QString::number(i + 1)).toString();
    line.remove(QLatin1Char('"'));  // stray quoting, as in the source
    translated << line.simplified();
  }
  return translated;
}

bool SubtitleTranslator::looksValid(const QStringList& source,
                                    const QStringList& translated) const {
  if (translated.size() != source.size()) return false;

  // letters and digits only, so an echo with added quotes or punctuation is
  // still recognised as one
  auto normalized = [](const QString& text) {
    QString n;
    for (const QChar& ch : text) {
      if (ch.isLetterOrNumber()) n += ch.toLower();
    }
    return n;
  };

  int echoed = 0;
  for (int i = 0; i < source.size(); ++i) {
    const QString& s = source[i];
    const QString& t = translated[i];

    if (t.isEmpty() && !s.isEmpty()) return false;
    // runaway generation, or content pulled in from a neighbouring line; a
    // translation is rarely much longer than its source
    if (t.length() > 2 * s.length() + 30) return false;
    // left in the source language; short lines ("Okay.", names) may
    // legitimately read the same
    if (s.length() > 12 && normalized(t) == normalized(s)) ++echoed;
  }

  // some lines stay as they are (song lyrics, titles), but not many
  return echoed * 8 <= source.size() || echoed <= 1;
}

bool SubtitleTranslator::polishPrompt() const {
  // Polish-trained models such as Bielik follow Polish instructions more
  // faithfully (idioms, register) than the same instructions in English
  return sourceLanguageName == "English" && targetLanguageName == "Polish";
}

QString SubtitleTranslator::defaultGuidelines(
    const QString& sourceLanguageName, const QString& targetLanguageName) {
  if (sourceLanguageName == "English" && targetLanguageName == "Polish") {
    return QString::fromUtf8(
        "- Pisz naturalną, potoczną polszczyzną, tak jak zawodowy tłumacz "
        "napisów, a nie dosłownie. Tłumaczenie ma być zwięzłe, żeby dało się "
        "je przeczytać na ekranie.\n"
        "- Wykrzyknienia, idiomy, żarty i slang tłumacz według sensu, "
        "zachowując ton i humor (np. \"Boy, ...\" to \"Rany, ...\", \"Gee\" "
        "to \"Kurczę\", \"What's up?\" to \"Co tam?\").\n"
        "- Nie tłumacz imion. Dbaj o zgodność rodzaju gramatycznego z "
        "kontekstem.\n"
        "- Tekst pochodzi z automatycznego rozpoznawania mowy i może zawierać "
        "drobne błędy; tłumacz to, co najprawdopodobniej zostało "
        "powiedziane.");
  }

  return QString(
      "- Write natural, idiomatic, colloquial {target}, as a professional "
      "subtitler would - not a word-for-word translation. Keep it short "
      "enough to read on screen.\n"
      "- Translate jokes, idioms and slang by meaning, keeping their tone.\n"
      "- Do not translate personal names. Use grammatical gender consistent "
      "with the context.\n"
      "- The text comes from speech recognition and may contain small "
      "errors; translate what was most likely said.");
}

QString SubtitleTranslator::systemPrompt(const QString& title) const {
  QString rules =
      guidelines.trimmed().isEmpty()
          ? defaultGuidelines(sourceLanguageName, targetLanguageName)
          : guidelines.trimmed();
  rules.replace("{source}", sourceLanguageName)
      .replace("{target}", targetLanguageName);

  // The line-per-key rules are what keeps the reply parseable and the cues in
  // sync, so they stay outside the editable guidelines.
  QString prompt;
  if (polishPrompt()) {
    prompt = QString::fromUtf8(
                 "Jesteś doświadczonym tłumaczem napisów do filmów i seriali. "
                 "Tłumaczysz napisy z angielskiego na polski.\n"
                 "Zasady:\n"
                 "- Przetłumacz każdą linię, zachowując jej klucz. Każde "
                 "tłumaczenie zawiera wyłącznie treść swojej linii: nie "
                 "przenoś słów między liniami, nie łącz ani nie dziel "
                 "linii.\n") +
             rules + QString::fromUtf8("\n- Zwróć wyłącznie obiekt JSON.");
    if (!title.isEmpty()) {
      prompt += QString::fromUtf8("\n\nNapisy pochodzą z: %1").arg(title);
    }
    return prompt;
  }

  prompt = QString(
               "You are an experienced subtitle translator. Translate film "
               "and TV subtitles from %1 into %2.\n"
               "Rules:\n"
               "- Translate every line, keeping its key. Each translation "
               "must contain only the content of its own line: never move "
               "words between lines, never merge or split lines.\n")
               .arg(sourceLanguageName, targetLanguageName) +
           rules + "\n- Output only the JSON object.";
  if (!title.isEmpty()) {
    prompt += QString("\n\nThe subtitles are from: %1").arg(title);
  }
  return prompt;
}
