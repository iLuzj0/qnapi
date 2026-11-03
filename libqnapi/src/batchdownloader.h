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

#ifndef BATCHDOWNLOADER_H
#define BATCHDOWNLOADER_H

#include <Maybe.h>

#include <QString>
#include <QStringList>
#include <QVector>

#include "config/qnapiconfig.h"
#include "qnapi.h"
#include "subtitleinfo.h"

enum class DownloadResult {
  Saved,
  // the user chose not to download any of the subtitles found
  Declined,
  NotFound,
  // still blocked by the service's rate limiter after every retry round
  RateLimited,
  DownloadFailed,
  UnpackFailed,
  MatchFailed,
  NoWritePermission,
  Aborted
};

struct DownloadOutcome {
  bool processed = false;
  DownloadResult result = DownloadResult::NotFound;
  // language of the saved subtitles, e.g. "pl"; empty when none were saved
  QString language;
  QString error;
  // the subtitles that were saved, valid when result == Saved
  SubtitleInfo subtitle;
};

enum class DownloadStep {
  CheckingPermissions,
  Checksum,
  Searching,
  SearchingBackup,
  WaitingToRetry,
  Downloading,
  Unpacking,
  PostProcessing,
  Matching
};

struct DownloadStepInfo {
  QString language;
  QString engine;
  // WaitingToRetry only
  int attempt = 0;
  int maxAttempts = 0;
  int delayMsecs = 0;
  QString reason;
};

// In parallel mode every method except batchStarted/batchFinished is called
// from worker threads, possibly concurrently.
class BatchDownloadListener {
 public:
  virtual ~BatchDownloadListener() = default;

  virtual void batchStarted(int fileCount, int jobs) {
    Q_UNUSED(fileCount)
    Q_UNUSED(jobs)
  }
  virtual void fileStarted(int index) { Q_UNUSED(index) }
  virtual void fileStep(int index, DownloadStep step,
                        const DownloadStepInfo& info) {
    Q_UNUSED(index)
    Q_UNUSED(step)
    Q_UNUSED(info)
  }
  virtual void fileFinished(int index, const DownloadOutcome& outcome) {
    Q_UNUSED(index)
    Q_UNUSED(outcome)
  }
  virtual void waitingForRateLimit(int fileCount, int msecs) {
    Q_UNUSED(fileCount)
    Q_UNUSED(msecs)
  }
  virtual void batchFinished() {}

  // Whether to offer a choice when the download policy asks for one; without
  // it the best match is taken.
  virtual bool canSelectSubtitles() const { return false; }
  // A console can ask one question at a time, a GUI can queue dialogs from
  // several workers.
  virtual bool canSelectSubtitlesConcurrently() const { return false; }
  // Index into the list, or nothing() to download none of them.
  virtual Maybe<int> selectSubtitles(int index, const SubtitleInfoList& list) {
    Q_UNUSED(index)
    Q_UNUSED(list)
    return nothing();
  }

  virtual bool abortRequested() const { return false; }
};

// Downloads subtitles for a list of movies.
//
// Sequentially, a file whose search failed transiently is retried after a
// short pause. In parallel the services behave differently: their limiter
// lets a burst of concurrent requests through and then needs about half a
// minute before it accepts more, while requests spaced out one after another
// are what it blocks. So the parallel schedule fires every file at once,
// collects whatever came back rate-limited, waits once and fires the
// stragglers together.
class BatchDownloader {
 public:
  // Measured against NapiProjekt.
  static const int rateLimitResetMsecs = 30000;
  // A round that rescues nothing ends the loop early, otherwise files that
  // simply have no subtitles would cost half a minute per round.
  static const int maxRetryRounds = 3;

  BatchDownloader(const QNapiConfig& config,
                  const Maybe<QString>& specificEngine = nothing());

  QVector<DownloadOutcome> run(const QStringList& movieFilePaths,
                               BatchDownloadListener& listener) const;

  bool runsInParallel(int fileCount, const BatchDownloadListener& l) const;

 private:
  friend class BatchDownloadWorker;

  DownloadOutcome downloadOne(QNapi& napi, const QString& movieFilePath,
                              int index, int maxRetries,
                              BatchDownloadListener& listener) const;
  bool findSubtitles(QNapi& napi, int index, int maxRetries,
                     BatchDownloadListener& listener) const;
  bool findSubtitlesOnce(QNapi& napi, int index,
                         BatchDownloadListener& listener) const;
  // false when interrupted by an abort request
  static bool sleepUnlessAborted(int msecs,
                                 const BatchDownloadListener& listener);

  QVector<DownloadOutcome> runSequentially(
      const QStringList& movieFilePaths, BatchDownloadListener& listener) const;
  QVector<DownloadOutcome> runInParallel(const QStringList& movieFilePaths,
                                         int jobs,
                                         BatchDownloadListener& listener) const;

  const QNapiConfig config;
  const Maybe<QString> specificEngine;
};

#endif  // BATCHDOWNLOADER_H
