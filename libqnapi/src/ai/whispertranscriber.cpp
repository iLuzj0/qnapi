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

#include "ai/whispertranscriber.h"

#include <QDeadlineTimer>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QRegularExpression>
#include <QThread>

#include "ai/childprocess.h"

namespace {

// Cue shaping. Lengths are in characters, times in milliseconds.
const int maxCueLength = 84;  // two lines of 42
const int sentenceBreakLength = 40;
const qint64 leadIn = 100;  // show a cue just before the first word
const qint64 tailAfterLastWord = 500;
const qint64 msPerCharacter = 60;  // a comfortable reading speed
const qint64 minCueDuration = 1000;
const qint64 maxCueDuration = 7000;
const qint64 gapBetweenCues = 40;

struct Word {
  QString text;
  qint64 ms;
};

// The DTW alignment heads whisper.cpp needs for word-level timestamps, guessed
// from the model file name. Without them only segment timestamps exist, which
// large-v3-turbo rounds to whole seconds.
QString dtwPresetFor(const QString& modelPath) {
  const QString name = QFileInfo(modelPath).fileName().toLower();
  static const QList<QPair<QString, QString>> presets = {
      {"large-v3-turbo", "large.v3.turbo"},
      {"large-v3", "large.v3"},
      {"large-v2", "large.v2"},
      {"large-v1", "large.v1"},
      {"medium.en", "medium.en"},
      {"medium", "medium"},
      {"small.en", "small.en"},
      {"small", "small"},
      {"base.en", "base.en"},
      {"base", "base"},
      {"tiny.en", "tiny.en"},
      {"tiny", "tiny"}};
  for (const auto& p : presets) {
    if (name.contains(p.first)) return p.second;
  }
  return QString();
}

// Joins whisper's sub-word tokens into words with a time each: a token that
// starts with a space starts a new word, anything else (punctuation, "'s")
// belongs to the previous one.
QList<Word> wordsOf(const QJsonObject& segment) {
  QList<Word> words;
  const qint64 segmentStart = segment.value("offsets")
                                  .toObject()
                                  .value("from")
                                  .toVariant()
                                  .toLongLong();

  const QJsonArray tokens = segment.value("tokens").toArray();
  for (const QJsonValue& v : tokens) {
    const QJsonObject token = v.toObject();
    QString text = token.value("text").toString();
    // special tokens: [_BEG_], [_TT_150], ...
    if (text.isEmpty() || text.startsWith("[_")) continue;

    // Whisper sometimes slips into quoting every line, with quotes opening in
    // one cue and closing in another. They mean nothing in dialogue subtitles
    // and they confuse the translator into merging lines.
    text.remove(QLatin1Char('"'));
    text.remove(QChar(0x201C));
    text.remove(QChar(0x201D));

    qint64 ms = -1;
    const qint64 dtw = token.value("t_dtw").toVariant().toLongLong();
    if (token.contains("t_dtw") && dtw >= 0) {
      ms = dtw * 10;  // centiseconds
    } else {
      ms = token.value("offsets")
               .toObject()
               .value("from")
               .toVariant()
               .toLongLong();
    }
    if (ms < segmentStart) ms = segmentStart;

    if (words.isEmpty() || text.startsWith(' ')) {
      words << Word{text.trimmed(), ms};
    } else {
      words.last().text += text;
    }
  }

  // drop words that were nothing but whitespace
  for (int i = words.size() - 1; i >= 0; --i) {
    if (words[i].text.isEmpty()) words.removeAt(i);
  }
  return words;
}

bool endsSentence(const QString& word) {
  return word.endsWith('.') || word.endsWith('?') || word.endsWith('!');
}

SrtCueList cuesFromWhisperJson(const QJsonDocument& doc) {
  // build cues as runs of words: a cue closes before it would grow past two
  // lines, or at the end of a sentence once it is long enough to stand alone
  struct RawCue {
    QString text;
    qint64 firstWordMs;
    qint64 lastWordMs;
  };
  QList<RawCue> raw;

  const QJsonArray segments = doc.object().value("transcription").toArray();
  for (const QJsonValue& v : segments) {
    const QList<Word> words = wordsOf(v.toObject());

    RawCue cue{QString(), 0, 0};
    for (int i = 0; i < words.size(); ++i) {
      const Word& w = words[i];
      if (!cue.text.isEmpty() &&
          cue.text.length() + 1 + w.text.length() > maxCueLength) {
        raw << cue;
        cue = RawCue{QString(), 0, 0};
      }
      if (cue.text.isEmpty()) {
        cue.text = w.text;
        cue.firstWordMs = w.ms;
      } else {
        cue.text += " " + w.text;
      }
      cue.lastWordMs = w.ms;

      const bool more = i + 1 < words.size();
      if (more && endsSentence(w.text) &&
          cue.text.length() >= sentenceBreakLength) {
        raw << cue;
        cue = RawCue{QString(), 0, 0};
      }
    }
    if (!cue.text.isEmpty()) raw << cue;
  }

  // whisper sometimes loops on a phrase over music or silence
  for (int i = raw.size() - 1; i > 0; --i) {
    if (raw[i].text == raw[i - 1].text) {
      raw[i - 1].lastWordMs = raw[i].lastWordMs;
      raw.removeAt(i);
    }
  }

  SrtCueList cues;
  for (int i = 0; i < raw.size(); ++i) {
    SrtCue cue;
    cue.startMs = qMax<qint64>(0, raw[i].firstWordMs - leadIn);
    if (!cues.isEmpty() && cue.startMs < cues.last().endMs + gapBetweenCues) {
      cue.startMs = cues.last().endMs + gapBetweenCues;
    }

    const qint64 readingTime = qBound(
        minCueDuration, raw[i].text.length() * msPerCharacter, maxCueDuration);
    qint64 end =
        qMax(raw[i].lastWordMs + tailAfterLastWord, cue.startMs + readingTime);
    end = qMin(end, cue.startMs + maxCueDuration);

    // never run into the next cue; it is timed by speech, this end is not
    if (i + 1 < raw.size()) {
      const qint64 nextStart = qMax<qint64>(0, raw[i + 1].firstWordMs - leadIn);
      end = qMin(end, nextStart - gapBetweenCues);
    }
    cue.endMs = qMax(end, cue.startMs + 300);

    cue.text = raw[i].text;
    cues << cue;
  }

  return cues;
}

}  // namespace

WhisperTranscriber::WhisperTranscriber(const QString& whisperPath,
                                       const QString& modelPath)
    : whisper(whisperPath), model(modelPath) {}

bool WhisperTranscriber::isAvailable() const {
  return QFileInfo(whisper).isExecutable() && QFileInfo(model).isFile();
}

Either<AiError, SrtCueList> WhisperTranscriber::transcribe(
    const QString& wavPath, const QString& language,
    const std::function<void(int)>& progress,
    const AiCancelCheck& cancelled) const {
  // whisper-cli appends ".json" to the -of base name
  const QString outBase = wavPath + ".whisper";
  const QString jsonPath = outBase + ".json";
  QFile::remove(jsonPath);

  const int threads = qBound(1, QThread::idealThreadCount() / 2, 8);

  QStringList args = {"-m", model, "-f", wavPath, "-l", language, "-t",
                      QString::number(threads),
                      // full JSON carries the per-token timestamps
                      "-ojf", "-of", outBase, "-pp"};

  const QString dtwPreset = dtwPresetFor(model);
  if (!dtwPreset.isEmpty()) {
    // word-level timestamps; whisper.cpp computes them only without flash
    // attention, which costs some speed
    args << "-dtw" << dtwPreset << "-nfa";
  }

  QProcess p;
  // progress goes to stderr; the transcription itself to stdout, unused
  p.setProcessChannelMode(QProcess::SeparateChannels);
  p.start(whisper, args);
  if (!p.waitForStarted()) {
    return AiError{QObject::tr("Could not start whisper: %1").arg(whisper)};
  }
  ChildProcess::bindToParentLifetime(p);

  static const QRegularExpression progressRe("progress\\s*=\\s*(\\d+)%");
  QByteArray errBuffer, errTail;
  QDeadlineTimer deadline(timeoutMsecs);

  while (!p.waitForFinished(250)) {
    if (p.state() == QProcess::NotRunning) break;
    if (cancelled && cancelled()) {
      p.kill();
      p.waitForFinished();
      QFile::remove(jsonPath);
      return AiError::cancellation();
    }
    if (deadline.hasExpired()) {
      p.kill();
      p.waitForFinished();
      QFile::remove(jsonPath);
      return AiError{QObject::tr("whisper timed out")};
    }

    p.readAllStandardOutput();  // keep the pipe drained
    errBuffer += p.readAllStandardError();

    int nl;
    while ((nl = errBuffer.indexOf('\n')) >= 0) {
      const QByteArray line = errBuffer.left(nl);
      errBuffer.remove(0, nl + 1);
      errTail = (errTail + line + '\n').right(4000);

      QRegularExpressionMatch m = progressRe.match(QString::fromLatin1(line));
      if (m.hasMatch() && progress) progress(m.captured(1).toInt());
    }
  }

  errTail = (errTail + errBuffer + p.readAllStandardError()).right(4000);

  if (p.exitStatus() != QProcess::NormalExit || p.exitCode() != 0) {
    QFile::remove(jsonPath);
    return AiError{
        QObject::tr("whisper failed (%1)").arg(ChildProcess::tail(errTail, 3))};
  }

  QFile f(jsonPath);
  if (!f.open(QIODevice::ReadOnly)) {
    return AiError{QObject::tr("whisper produced no output file")};
  }
  QJsonParseError parseError;
  const QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &parseError);
  f.close();
  QFile::remove(jsonPath);

  if (parseError.error != QJsonParseError::NoError) {
    return AiError{QObject::tr("cannot read whisper's output (%1)")
                       .arg(parseError.errorString())};
  }

  const SrtCueList cues = cuesFromWhisperJson(doc);
  if (cues.isEmpty()) {
    return AiError{QObject::tr("whisper found no speech in the audio")};
  }

  if (progress) progress(100);
  return cues;
}
