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

#include "batchdownloader.h"

#include <QAtomicInt>
#include <QDeadlineTimer>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

BatchDownloader::BatchDownloader(const QNapiConfig& config,
                                 const Maybe<QString>& specificEngine)
    : config(config), specificEngine(specificEngine) {}

bool BatchDownloader::sleepUnlessAborted(
    int msecs, const BatchDownloadListener& listener) {
  QDeadlineTimer deadline(msecs);
  while (!deadline.hasExpired()) {
    if (listener.abortRequested()) return false;
    QThread::msleep(qMin<qint64>(250, deadline.remainingTime()));
  }
  return !listener.abortRequested();
}

bool BatchDownloader::findSubtitlesOnce(QNapi& napi, int index,
                                        BatchDownloadListener& l) const {
  bool found = false;

  const SearchPolicy sp = config.generalConfig().searchPolicy();
  const QString lang = config.generalConfig().language();
  const QString langBackup = config.generalConfig().backupLanguage();

  auto search = [&](DownloadStep step, const QString& language,
                    const QString& engine) {
    DownloadStepInfo info;
    info.language = language;
    info.engine = engine;
    l.fileStep(index, step, info);
    found = napi.lookForSubtitles(language, engine) || found;
  };

  if (sp == SP_SEARCH_ALL_WITH_BACKUP_LANG) {
    foreach (QString e, napi.listLoadedEngines()) {
      if (l.abortRequested()) return found;
      search(DownloadStep::Searching, lang, e);
      if (l.abortRequested()) return found;
      search(DownloadStep::SearchingBackup, langBackup, e);
    }
    return found;
  }

  foreach (QString e, napi.listLoadedEngines()) {
    if (l.abortRequested()) return found;
    search(DownloadStep::Searching, lang, e);
    if (sp == SP_BREAK_IF_FOUND && found) break;
  }

  if (!found && !langBackup.isEmpty()) {
    foreach (QString e, napi.listLoadedEngines()) {
      if (l.abortRequested()) return found;
      search(DownloadStep::SearchingBackup, langBackup, e);
      if (sp == SP_BREAK_IF_FOUND && found) break;
    }
  }

  return found;
}

bool BatchDownloader::findSubtitles(QNapi& napi, int index, int maxRetries,
                                    BatchDownloadListener& l) const {
  l.fileStep(index, DownloadStep::Checksum, DownloadStepInfo());
  napi.checksum();

  bool found = findSubtitlesOnce(napi, index, l);

  for (int attempt = 1;
       !found && attempt <= maxRetries && napi.lastSearchFailedTransiently();
       ++attempt) {
    DownloadStepInfo info;
    info.attempt = attempt;
    info.maxAttempts = maxRetries;
    info.delayMsecs = QNapi::searchRetryDelayMsecs;
    info.reason = napi.error().split(QLatin1Char('\n')).first();
    l.fileStep(index, DownloadStep::WaitingToRetry, info);

    if (!sleepUnlessAborted(QNapi::searchRetryDelayMsecs, l)) return false;
    napi.clearSubtitlesList();
    found = findSubtitlesOnce(napi, index, l);
  }

  return found;
}

DownloadOutcome BatchDownloader::downloadOne(QNapi& napi,
                                             const QString& movieFilePath,
                                             int index, int maxRetries,
                                             BatchDownloadListener& l) const {
  DownloadOutcome outcome;
  outcome.processed = true;

  auto fail = [&](DownloadResult result) {
    outcome.result = result;
    outcome.error = napi.error();
    return outcome;
  };

  napi.setMoviePath(movieFilePath);

  l.fileStep(index, DownloadStep::CheckingPermissions, DownloadStepInfo());
  if (!napi.checkWritePermissions())
    return fail(DownloadResult::NoWritePermission);

  napi.clearSubtitlesList();

  const bool found = findSubtitles(napi, index, maxRetries, l);
  if (l.abortRequested()) return fail(DownloadResult::Aborted);

  if (!found) {
    return fail(napi.lastSearchFailedTransiently() ? DownloadResult::RateLimited
                                                   : DownloadResult::NotFound);
  }

  // needToShowList() also works out bestIdx(), so it runs either way
  const bool showList = napi.needToShowList() && l.canSelectSubtitles();
  Maybe<int> selected = just(napi.bestIdx());
  if (showList) selected = l.selectSubtitles(index, napi.listSubtitles());

  if (l.abortRequested()) return fail(DownloadResult::Aborted);

  if (!selected) {
    napi.cleanup();
    outcome.result = DownloadResult::Declined;
    return outcome;
  }

  const int selIdx = selected.value();

  l.fileStep(index, DownloadStep::Downloading, DownloadStepInfo());
  if (!napi.download(selIdx)) return fail(DownloadResult::DownloadFailed);

  l.fileStep(index, DownloadStep::Unpacking, DownloadStepInfo());
  if (!napi.unpack(selIdx)) return fail(DownloadResult::UnpackFailed);

  if (napi.ppEnabled()) {
    l.fileStep(index, DownloadStep::PostProcessing, DownloadStepInfo());
    napi.postProcessSubtitles();
  }

  l.fileStep(index, DownloadStep::Matching, DownloadStepInfo());
  if (!napi.matchSubtitles()) return fail(DownloadResult::MatchFailed);

  outcome.result = DownloadResult::Saved;
  outcome.language = napi.subtitleLanguage();
  outcome.subtitle = napi.listSubtitles().at(selIdx);
  napi.cleanup();
  return outcome;
}

bool BatchDownloader::runsInParallel(int fileCount,
                                     const BatchDownloadListener& l) const {
  const int jobs = qBound(1, qMin(config.generalConfig().jobs(), fileCount),
                          GeneralConfig::maxJobs);
  if (jobs <= 1) return false;

  const bool mayAsk =
      l.canSelectSubtitles() &&
      config.generalConfig().downloadPolicy() != DP_NEVER_SHOW_LIST;
  return !mayAsk || l.canSelectSubtitlesConcurrently();
}

QVector<DownloadOutcome> BatchDownloader::run(const QStringList& movieFilePaths,
                                              BatchDownloadListener& l) const {
  if (runsInParallel(movieFilePaths.size(), l)) {
    const int jobs = qBound(
        1, qMin(config.generalConfig().jobs(), int(movieFilePaths.size())),
        GeneralConfig::maxJobs);
    return runInParallel(movieFilePaths, jobs, l);
  }
  return runSequentially(movieFilePaths, l);
}

QVector<DownloadOutcome> BatchDownloader::runSequentially(
    const QStringList& movieFilePaths, BatchDownloadListener& l) const {
  QVector<DownloadOutcome> outcomes(movieFilePaths.size());
  QNapi napi(config, specificEngine);

  l.batchStarted(movieFilePaths.size(), 1);

  for (int i = 0; i < movieFilePaths.size(); ++i) {
    if (l.abortRequested()) break;
    l.fileStarted(i);
    outcomes[i] =
        downloadOne(napi, movieFilePaths[i], i, QNapi::maxSearchRetries, l);
    l.fileFinished(i, outcomes[i]);
  }

  l.batchFinished();
  return outcomes;
}

// Each worker takes files off a shared counter and processes them with its own
// QNapi instance; nothing else is shared.
class BatchDownloadWorker : public QThread {
 public:
  BatchDownloadWorker(const BatchDownloader& downloader,
                      const QStringList& allPaths, const QList<int>& indices,
                      QAtomicInt& nextSlot, QMutex& resultsMutex,
                      QList<int>& rateLimited,
                      QVector<DownloadOutcome>& outcomes,
                      BatchDownloadListener& listener)
      : downloader(downloader),
        allPaths(allPaths),
        indices(indices),
        nextSlot(nextSlot),
        resultsMutex(resultsMutex),
        rateLimited(rateLimited),
        outcomes(outcomes),
        listener(listener) {}

 protected:
  void run() override {
    QNapi napi(downloader.config, downloader.specificEngine);

    for (;;) {
      if (listener.abortRequested()) return;

      const int slot = nextSlot.fetchAndAddOrdered(1);
      if (slot >= indices.size()) return;

      const int i = indices[slot];
      listener.fileStarted(i);

      // No per-file retry: pausing between attempts is exactly what the rate
      // limiter punishes. Stragglers are retried as a group instead.
      const DownloadOutcome outcome =
          downloader.downloadOne(napi, allPaths[i], i, 0, listener);

      {
        QMutexLocker locker(&resultsMutex);
        outcomes[i] = outcome;
        if (outcome.result == DownloadResult::RateLimited) rateLimited << i;
      }

      // a rate-limited file is reported once it has run out of rounds
      if (outcome.result == DownloadResult::RateLimited)
        listener.fileDeferred(i);
      else
        listener.fileFinished(i, outcome);
    }
  }

 private:
  const BatchDownloader& downloader;
  const QStringList& allPaths;
  const QList<int> indices;
  QAtomicInt& nextSlot;
  QMutex& resultsMutex;
  QList<int>& rateLimited;
  QVector<DownloadOutcome>& outcomes;
  BatchDownloadListener& listener;
};

QVector<DownloadOutcome> BatchDownloader::runInParallel(
    const QStringList& movieFilePaths, int jobs,
    BatchDownloadListener& l) const {
  const int fileCount = movieFilePaths.size();
  QVector<DownloadOutcome> outcomes(fileCount);
  QMutex resultsMutex;

  l.batchStarted(fileCount, jobs);

  QList<int> pending;
  for (int i = 0; i < fileCount; ++i) pending << i;

  for (int round = 0; round <= maxRetryRounds && !pending.isEmpty(); ++round) {
    if (round > 0) {
      l.waitingForRateLimit(pending.size(), rateLimitResetMsecs);
      if (!sleepUnlessAborted(rateLimitResetMsecs, l)) break;
    }

    QList<int> rateLimited;
    QAtomicInt nextSlot(0);

    QList<BatchDownloadWorker*> workers;
    const int inRound = qMin(jobs, int(pending.size()));
    for (int w = 0; w < inRound; ++w) {
      workers << new BatchDownloadWorker(*this, movieFilePaths, pending,
                                         nextSlot, resultsMutex, rateLimited,
                                         outcomes, l);
    }
    foreach (BatchDownloadWorker* worker, workers) worker->start();
    foreach (BatchDownloadWorker* worker, workers) {
      worker->wait();
      delete worker;
    }

    const bool madeProgress = rateLimited.size() < pending.size();
    pending = rateLimited;
    if (!madeProgress || l.abortRequested()) break;
  }

  foreach (int i, pending) {
    if (l.abortRequested() && !outcomes[i].processed) continue;
    l.fileFinished(i, outcomes[i]);
  }

  l.batchFinished();
  return outcomes;
}
