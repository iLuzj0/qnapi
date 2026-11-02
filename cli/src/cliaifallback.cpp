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

#include "cliaifallback.h"

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QUuid>

#include "libqnapi.h"
#include "subtitlelanguage.h"

namespace CliAiFallback {

namespace {

// The prompt is in English, so it needs English language names; the ones
// SubtitleLanguage gives are translated into the UI language.
QString englishLanguageName(const QString& language) {
  static const QMap<QString, QString> names = {
      {"en", "English"},    {"pl", "Polish"},    {"de", "German"},
      {"fr", "French"},     {"es", "Spanish"},   {"it", "Italian"},
      {"pt", "Portuguese"}, {"nl", "Dutch"},     {"cs", "Czech"},
      {"sk", "Slovak"},     {"ru", "Russian"},   {"uk", "Ukrainian"},
      {"sv", "Swedish"},    {"no", "Norwegian"}, {"da", "Danish"},
      {"fi", "Finnish"},    {"hu", "Hungarian"}, {"ro", "Romanian"},
      {"bg", "Bulgarian"},  {"hr", "Croatian"},  {"sr", "Serbian"},
      {"sl", "Slovenian"},  {"el", "Greek"},     {"tr", "Turkish"},
      {"ja", "Japanese"},   {"ko", "Korean"},    {"zh", "Chinese"}};

  SubtitleLanguage l(language);
  const QString two = l.toTwoLetter();
  return names.value(two, l.toFullName().isEmpty() ? language : l.toFullName());
}

// "Some.Movie.2010.720p" reads better to the model as "Some Movie 2010 720p"
QString titleFor(const QString& movieFilePath) {
  QString title = QFileInfo(movieFilePath).completeBaseName();
  if (!title.contains(' ')) {
    title.replace('.', ' ').replace('_', ' ');
  }
  return title.simplified();
}

QString noticeText(const QString& targetTwoLetter) {
  if (targetTwoLetter == "pl") {
    return QString::fromUtf8(
        "[Napisy wygenerowane automatycznie przez AI – mogą zawierać błędy]");
  }
  return "[Subtitles generated automatically by AI - may contain errors]";
}

// Opens the subtitles with a cue saying they are machine-made, so nobody
// mistakes them for a proper translation. Shown before the first line of
// dialogue where there is room, otherwise briefly alongside it.
SrtCueList withNotice(const SrtCueList& cues, const QString& notice) {
  SrtCue cue;
  cue.startMs = 0;
  cue.endMs = 4000;
  if (!cues.isEmpty()) {
    cue.endMs = qBound(qint64(1500), cues.first().startMs - 100, qint64(4000));
  }
  cue.text = SrtCues::wrap(notice);

  SrtCueList result;
  result << cue << cues;
  return result;
}

QString tempPath(const QNapiConfig& config, const QString& suffix) {
  return QDir(config.generalConfig().tmpPath())
      .filePath("QNapi.ai-" +
                QUuid::createUuid().toString(QUuid::WithoutBraces) + suffix);
}

QString formatDuration(qint64 msecs) {
  const qint64 secs = msecs / 1000;
  if (secs < 60) return QString("%1 s").arg(secs);
  return QString("%1 min %2 s").arg(secs / 60).arg(secs % 60);
}

bool checkTools(const Console& c, const AudioExtractor& extractor,
                const WhisperTranscriber& whisper, const LlamaServer& llm) {
  QStringList missing;

  if (!extractor.isAvailable()) missing << extractor.ffmpegPath();
  if (!QFileInfo(whisper.whisperPath()).isExecutable())
    missing << whisper.whisperPath();
  if (!QFileInfo(whisper.modelPath()).isFile()) missing << whisper.modelPath();
  if (!QFileInfo(llm.serverPath()).isExecutable()) missing << llm.serverPath();
  if (!QFileInfo(llm.modelPath()).isFile()) missing << llm.modelPath();

  if (missing.isEmpty()) return true;

  c.printLineError(tr("AI fallback is unavailable, missing:"));
  foreach (const QString& path, missing) {
    c.printLineError("   " + QDir::toNativeSeparators(path));
  }
  c.printLineError(
      tr("Build the tools with scripts/build-ai-tools.bat and download the "
         "models with scripts/download-ai-models.ps1"));
  return false;
}

}  // namespace

bool hasSubtitlesInLanguage(const QString& movieFilePath,
                            const QString& language,
                            const QStringList& subtitleExtensions,
                            bool unlabelledCounts) {
  SubtitleLanguage l(language);
  QStringList suffixes = {"." + l.toTwoLetter(), "." + l.toTriLetter()};
  if (unlabelledCounts) suffixes << QString();

  const QFileInfo movie(movieFilePath);
  const QDir dir = movie.absoluteDir();

  foreach (const QString& suffix, suffixes) {
    if (suffix == ".") continue;  // unknown language code
    foreach (const QString& ext, subtitleExtensions) {
      if (dir.exists(movie.completeBaseName() + suffix + "." + ext))
        return true;
    }
  }
  return false;
}

int generateSubtitles(const Console& c, const QStringList& movieFilePaths,
                      const QNapiConfig& config) {
  if (movieFilePaths.isEmpty()) return 0;

  const AiConfig aiConfig = config.aiConfig();
  const QString targetLanguage =
      SubtitleLanguage(config.generalConfig().language()).toTwoLetter();
  const QString sourceLanguage = aiConfig.sourceLanguage().toLower();
  const bool detectSource = sourceLanguage == "auto";

  const auto extractor = LibQNapi::audioExtractor(aiConfig);
  const auto whisper = LibQNapi::whisperTranscriber(aiConfig);
  const QString llamaLog =
      QDir(config.generalConfig().tmpPath()).filePath("QNapi.ai-llama.log");
  const auto llm = LibQNapi::llamaServer(aiConfig, llamaLog);

  c.printLine();
  c.printLineHighlihted(
      tr("AI fallback: generating %1 subtitles for %n file(s)", "",
         movieFilePaths.size())
          .arg(englishLanguageName(targetLanguage)));

  if (!checkTools(c, *extractor, *whisper, *llm)) return 0;

  const int total = movieFilePaths.size();
  auto prefix = [total](int i) {
    return QString("[AI %1/%2] ").arg(i + 1).arg(total);
  };

  // --- 1. transcription ----------------------------------------------------
  QMap<int, SrtCueList> transcriptions;
  const QString audioLanguage =
      detectSource ? QString() : SubtitleLanguage(sourceLanguage).toTriLetter();

  for (int i = 0; i < total; ++i) {
    const QString& movie = movieFilePaths[i];
    const QString name = QFileInfo(movie).fileName();
    const QString label = prefix(i) + tr("Transcribing %1").arg(name);
    QElapsedTimer timer;
    timer.start();

    c.printOrdinary(label + "...\r");

    const QString wav = tempPath(config, ".wav");
    auto audio = extractor->extract(movie, wav, audioLanguage);
    if (audio.is1st()) {
      c.printLine();
      c.printLineWarning(prefix(i) + name + " - " + audio.as1st().message);
      continue;
    }

    int lastShown = -1;
    auto transcription = whisper->transcribe(
        wav, detectSource ? QString("auto") : sourceLanguage, [&](int percent) {
          if (percent / 5 == lastShown / 5) return;
          lastShown = percent;
          c.printOrdinary(QString("%1... %2%\r").arg(label).arg(percent));
        });
    QFile::remove(wav);
    c.printLine();

    if (transcription.is1st()) {
      c.printLineWarning(prefix(i) + name + " - " +
                         transcription.as1st().message);
      continue;
    }

    transcriptions.insert(i, transcription.as2nd());
    c.printLineOrdinary(prefix(i) + tr("%n line(s) transcribed in %1", "",
                                       transcription.as2nd().size())
                                        .arg(formatDuration(timer.elapsed())));
  }

  if (transcriptions.isEmpty()) {
    c.printLineWarning(tr("AI fallback: nothing was transcribed"));
    return 0;
  }

  // --- 2. translation --------------------------------------------------------
  c.printLineOrdinary(tr("Loading the translation model..."));
  QElapsedTimer loadTimer;
  loadTimer.start();

  auto started = llm->start();
  if (started.is1st()) {
    c.printLineError(tr("AI fallback: %1").arg(started.as1st().message));
    c.printLineError(
        tr("Server log: %1").arg(QDir::toNativeSeparators(llamaLog)));
    return 0;
  }
  c.printLineOrdinary(
      tr("Model loaded in %1").arg(formatDuration(loadTimer.elapsed())));

  const auto translator = LibQNapi::subtitleTranslator(
      llm, aiConfig,
      detectSource ? QString("the original language")
                   : englishLanguageName(sourceLanguage),
      englishLanguageName(targetLanguage));

  const auto postProcessor =
      LibQNapi::subtitlePostProcessor(config.postProcessingConfig());
  const auto matcher = LibQNapi::subtitleMatcher(config);
  const QString notice = noticeText(targetLanguage);

  int saved = 0;

  foreach (int i, transcriptions.keys()) {
    const QString& movie = movieFilePaths[i];
    const QString name = QFileInfo(movie).fileName();
    const QString label = prefix(i) + tr("Translating %1").arg(name);
    QElapsedTimer timer;
    timer.start();

    auto translation = translator->translate(
        transcriptions[i], titleFor(movie), [&](int done, int count) {
          c.printOrdinary(
              QString("%1... %2/%3\r").arg(label).arg(done).arg(count));
        });
    c.printLine();

    if (translation.is1st()) {
      c.printLineError(prefix(i) + name + " - " + translation.as1st().message);
      // without the server the remaining files cannot be translated either
      if (!llm->isRunning()) break;
      continue;
    }

    const QString srt = tempPath(config, ".srt");
    if (!SrtCues::writeFile(srt, withNotice(translation.as2nd(), notice))) {
      c.printLineError(prefix(i) + name + " - " +
                       tr("cannot write temporary file %1")
                           .arg(QDir::toNativeSeparators(srt)));
      continue;
    }

    if (config.postProcessingConfig().enabled()) {
      postProcessor->perform(movie, srt);
    }

    const bool matched = matcher->matchSubtitles(srt, movie, targetLanguage);
    QFile::remove(srt);

    if (!matched) {
      c.printLineError(prefix(i) + name + " - " +
                       tr("could not save the subtitles next to the movie"));
      continue;
    }

    ++saved;
    QString summary =
        prefix(i) + name + " - " +
        tr("AI subtitles saved (%1)").arg(formatDuration(timer.elapsed()));
    if (translator->untranslatedLines() > 0) {
      summary += ", " + tr("%n line(s) left untranslated", "",
                           translator->untranslatedLines());
    }
    c.printLineHighlihted(summary);
  }

  llm->stop();
  return saved;
}

}  // namespace CliAiFallback
