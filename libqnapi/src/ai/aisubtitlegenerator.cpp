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

#include "ai/aisubtitlegenerator.h"

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QUuid>

#include "libqnapi.h"
#include "subtitlelanguage.h"

namespace {

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

}  // namespace

AiSubtitleGenerator::AiSubtitleGenerator(const QNapiConfig& config)
    : config(config) {}

QString AiSubtitleGenerator::englishLanguageName(const QString& language) {
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

QStringList AiSubtitleGenerator::missingTools() const {
  const AiConfig aiConfig = config.aiConfig();
  const auto extractor = LibQNapi::audioExtractor(aiConfig);
  const auto whisper = LibQNapi::whisperTranscriber(aiConfig);
  const auto llm = LibQNapi::llamaServer(aiConfig, QString());

  QStringList missing;
  if (!extractor->isAvailable()) missing << extractor->ffmpegPath();
  if (!QFileInfo(whisper->whisperPath()).isExecutable())
    missing << whisper->whisperPath();
  if (!QFileInfo(whisper->modelPath()).isFile())
    missing << whisper->modelPath();
  if (!QFileInfo(llm->serverPath()).isExecutable())
    missing << llm->serverPath();
  if (!QFileInfo(llm->modelPath()).isFile()) missing << llm->modelPath();
  return missing;
}

bool AiSubtitleGenerator::hasSubtitlesInLanguage(
    const QString& movieFilePath, const QString& language,
    const QStringList& subtitleExtensions, bool unlabelledCounts) {
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

QStringList AiSubtitleGenerator::filesNeedingSubtitles(
    const QStringList& movieFilePaths, const QVector<DownloadOutcome>& outcomes,
    QStringList* alreadyCovered) const {
  const QString mainLanguage =
      SubtitleLanguage(config.generalConfig().language()).toTwoLetter();
  const QStringList subtitleExtensions =
      LibQNapi::staticConfig()->subtitleExtensions();

  QStringList queue;
  for (int i = 0; i < movieFilePaths.size(); ++i) {
    const DownloadOutcome& o = outcomes[i];
    if (!o.processed) continue;

    bool needed = false;
    switch (o.result) {
      case DownloadResult::Saved:
        needed = !o.language.isEmpty() &&
                 SubtitleLanguage(o.language).toTwoLetter() != mainLanguage;
        break;
      case DownloadResult::NotFound:
      case DownloadResult::RateLimited:
      case DownloadResult::DownloadFailed:
      case DownloadResult::UnpackFailed:
      case DownloadResult::MatchFailed:
        needed = true;
        break;
      case DownloadResult::Declined:
      case DownloadResult::NoWritePermission:
      case DownloadResult::Aborted:
        needed = false;
    }
    if (!needed) continue;

    // Without language suffixes, a Movie.srt saved by this very run is the
    // backup-language one and is meant to be replaced; an older one is not.
    const bool unlabelled =
        config.generalConfig().languageSuffix() == LS_NONE &&
        o.result != DownloadResult::Saved;
    if (hasSubtitlesInLanguage(movieFilePaths[i], mainLanguage,
                               subtitleExtensions, unlabelled)) {
      if (alreadyCovered != nullptr) *alreadyCovered << movieFilePaths[i];
      continue;
    }

    queue << movieFilePaths[i];
  }
  return queue;
}

int AiSubtitleGenerator::generate(const QStringList& movieFilePaths,
                                  AiGenerationListener& l) const {
  if (movieFilePaths.isEmpty()) return 0;

  const AiConfig aiConfig = config.aiConfig();
  const QString targetLanguage =
      SubtitleLanguage(config.generalConfig().language()).toTwoLetter();
  const QString sourceLanguage = aiConfig.sourceLanguage().toLower();
  const bool detectSource = sourceLanguage == "auto";

  l.started(movieFilePaths.size(), englishLanguageName(targetLanguage));

  const QStringList missing = missingTools();
  if (!missing.isEmpty()) {
    l.toolsMissing(missing);
    return 0;
  }

  const auto extractor = LibQNapi::audioExtractor(aiConfig);
  const auto whisper = LibQNapi::whisperTranscriber(aiConfig);
  const QString llamaLog =
      QDir(config.generalConfig().tmpPath()).filePath("QNapi.ai-llama.log");
  const auto llm = LibQNapi::llamaServer(aiConfig, llamaLog);

  const AiCancelCheck cancelled = [&l]() { return l.abortRequested(); };

  // --- 1. transcription ----------------------------------------------------
  QMap<int, SrtCueList> transcriptions;
  const QString audioLanguage =
      detectSource ? QString() : SubtitleLanguage(sourceLanguage).toTriLetter();

  for (int i = 0; i < movieFilePaths.size(); ++i) {
    if (l.abortRequested()) return 0;

    const QString& movie = movieFilePaths[i];
    QElapsedTimer timer;
    timer.start();
    l.transcriptionStarted(i);

    const QString wav = tempPath(config, ".wav");
    auto audio = extractor->extract(movie, wav, audioLanguage, cancelled);
    if (audio.is1st()) {
      if (audio.as1st().cancelled) return 0;
      l.transcriptionFinished(i, 0, audio.as1st().message, timer.elapsed());
      continue;
    }

    auto transcription = whisper->transcribe(
        wav, detectSource ? QString("auto") : sourceLanguage,
        [&](int percent) { l.transcriptionProgress(i, percent); }, cancelled);
    QFile::remove(wav);

    if (transcription.is1st()) {
      if (transcription.as1st().cancelled) return 0;
      l.transcriptionFinished(i, 0, transcription.as1st().message,
                              timer.elapsed());
      continue;
    }

    transcriptions.insert(i, transcription.as2nd());
    l.transcriptionFinished(i, transcription.as2nd().size(), QString(),
                            timer.elapsed());
  }

  if (transcriptions.isEmpty()) {
    l.nothingTranscribed();
    return 0;
  }
  if (l.abortRequested()) return 0;

  // --- 2. translation --------------------------------------------------------
  l.modelLoading();
  QElapsedTimer loadTimer;
  loadTimer.start();

  auto started = llm->start(cancelled);
  if (started.is1st()) {
    if (started.as1st().cancelled) return 0;
    l.modelFailed(started.as1st().message, llamaLog);
    return 0;
  }
  l.modelLoaded(loadTimer.elapsed());

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
    if (l.abortRequested()) break;

    const QString& movie = movieFilePaths[i];
    QElapsedTimer timer;
    timer.start();
    l.translationStarted(i);

    AiFileResult result;

    auto translation = translator->translate(
        transcriptions[i], titleFor(movie),
        [&](int done, int count) { l.translationProgress(i, done, count); },
        cancelled);

    if (translation.is1st()) {
      if (translation.as1st().cancelled) break;
      result.error = translation.as1st().message;
      result.msecs = timer.elapsed();
      l.translationFinished(i, result);
      // without the server the remaining files cannot be translated either
      if (!llm->isRunning()) break;
      continue;
    }

    const QString srt = tempPath(config, ".srt");
    if (!SrtCues::writeFile(srt, withNotice(translation.as2nd(), notice))) {
      result.error = QObject::tr("cannot write temporary file %1")
                         .arg(QDir::toNativeSeparators(srt));
      result.msecs = timer.elapsed();
      l.translationFinished(i, result);
      continue;
    }

    if (config.postProcessingConfig().enabled()) {
      postProcessor->perform(movie, srt);
    }

    const bool matched = matcher->matchSubtitles(srt, movie, targetLanguage);
    QFile::remove(srt);

    result.msecs = timer.elapsed();
    if (!matched) {
      result.error =
          QObject::tr("could not save the subtitles next to the movie");
      l.translationFinished(i, result);
      continue;
    }

    ++saved;
    result.saved = true;
    result.untranslatedLines = translator->untranslatedLines();
    l.translationFinished(i, result);
  }

  llm->stop();
  return saved;
}
