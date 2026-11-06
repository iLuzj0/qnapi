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

#ifndef AISUBTITLEGENERATOR_H
#define AISUBTITLEGENERATOR_H

#include <QString>
#include <QStringList>
#include <QVector>

#include "batchdownloader.h"
#include "config/qnapiconfig.h"

struct AiFileResult {
  bool saved = false;
  QString error;
  int untranslatedLines = 0;
  qint64 msecs = 0;
};

// Methods are called from the thread running generate().
class AiGenerationListener {
 public:
  virtual ~AiGenerationListener() = default;

  virtual void started(int fileCount, const QString& targetLanguageName) {
    Q_UNUSED(fileCount)
    Q_UNUSED(targetLanguageName)
  }
  virtual void toolsMissing(const QStringList& paths) { Q_UNUSED(paths) }

  virtual void transcriptionStarted(int index) { Q_UNUSED(index) }
  virtual void transcriptionProgress(int index, int percent) {
    Q_UNUSED(index)
    Q_UNUSED(percent)
  }
  // lines == 0 means it failed with the given error
  virtual void transcriptionFinished(int index, int lines, const QString& error,
                                     qint64 msecs) {
    Q_UNUSED(index)
    Q_UNUSED(lines)
    Q_UNUSED(error)
    Q_UNUSED(msecs)
  }
  virtual void nothingTranscribed() {}

  virtual void modelLoading() {}
  virtual void modelLoaded(qint64 msecs) { Q_UNUSED(msecs) }
  virtual void modelFailed(const QString& error, const QString& logPath) {
    Q_UNUSED(error)
    Q_UNUSED(logPath)
  }
  // The server died while translating the given file, which is translated
  // again once the model is reloaded.
  virtual void modelRestarting(int index, const QString& error) {
    Q_UNUSED(index)
    Q_UNUSED(error)
  }

  virtual void translationStarted(int index) { Q_UNUSED(index) }
  virtual void translationProgress(int index, int done, int count) {
    Q_UNUSED(index)
    Q_UNUSED(done)
    Q_UNUSED(count)
  }
  virtual void translationFinished(int index, const AiFileResult& result) {
    Q_UNUSED(index)
    Q_UNUSED(result)
  }

  virtual bool abortRequested() const { return false; }
};

// Generates subtitles for movies no service had them for:
//
//   1. extract each file's audio (ffmpeg) and transcribe it (whisper.cpp);
//   2. start llama-server once and translate all transcriptions;
//   3. post-process and save them like downloaded subtitles.
//
// Transcription and translation run as separate phases so that whisper and
// the LLM never compete for VRAM, and the LLM is loaded only once.
class AiSubtitleGenerator {
 public:
  explicit AiSubtitleGenerator(const QNapiConfig& config);

  // Executables and models that are not where the configuration says.
  QStringList missingTools() const;

  // The files of a finished batch that still have no subtitles in the main
  // language: none found, only the backup language, a download that failed,
  // or still rate-limited. Left out are files the user declined in the list,
  // files whose directory is not writable and files that already have
  // main-language subtitles next to them; the latter go to alreadyCovered.
  QStringList filesNeedingSubtitles(const QStringList& movieFilePaths,
                                    const QVector<DownloadOutcome>& outcomes,
                                    QStringList* alreadyCovered) const;

  // Indices passed to the listener refer to movieFilePaths. Returns the
  // number of files that got subtitles.
  int generate(const QStringList& movieFilePaths,
               AiGenerationListener& listener) const;

  // True when a subtitle file in the given language already sits next to the
  // movie (Movie.pol.srt, Movie.pl.srt, ...). With unlabelledCounts, a plain
  // Movie.srt counts too: without a language suffix there is no telling what
  // language it is, and the AI output would overwrite it.
  static bool hasSubtitlesInLanguage(const QString& movieFilePath,
                                     const QString& language,
                                     const QStringList& subtitleExtensions,
                                     bool unlabelledCounts);

  // The prompt is in English, so it needs English language names; the ones
  // SubtitleLanguage gives are translated into the UI language.
  static QString englishLanguageName(const QString& language);

  // The style rules the translator uses when none are configured, for a
  // source language code ("auto" to detect) and a target language code.
  static QString defaultGuidelines(const QString& sourceLanguage,
                                   const QString& targetLanguage);

 private:
  static const int maxServerRestartsPerFile = 2;

  static QString sourceLanguageName(const QString& sourceLanguage);

 public:
 private:
  const QNapiConfig config;
};

#endif  // AISUBTITLEGENERATOR_H
