/*****************************************************************************
** QNapi
** Copyright (C) 2008-2017 Piotr Krzemiński <pio.krzeminski@gmail.com>
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

#include "clisubtitlesdownloader.h"
#include "cliaifallback.h"
#include "libqnapi.h"
#include "qnapi.h"
#include "subtitlelanguage.h"

#include <QAtomicInt>
#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QVector>
#include <iostream>

namespace CliSubtitlesDownloader {

// Measured against NapiProjekt: a burst of concurrent requests succeeds, and
// the limiter then needs about half a minute before it will accept another.
const int rateLimitResetMsecs = 30000;
// Retry rounds are capped, but a round that rescues nothing ends the loop
// early - otherwise files that simply have no subtitles cost 30s per round.
const int maxRetryRounds = 3;

enum ExitCode {
  EC_OK = 0,
  EC_P7ZIP_UNAVAILABLE = 2,
  EC_CANNOT_WRITE_TMP_DIR = 3,
  EC_NO_WRITE_PERMISSIONS = 5,
  EC_SUBTITLES_NOT_FOUND = 6,
  EC_COULD_NOT_DOWNLOAD = 7,
  EC_COULD_NOT_UNARCHIVE = 8,
  EC_COULD_NOT_MATCH = 9
};

// What happened to one file, so that the AI fallback can pick up the files
// that ended up without subtitles in the main language.
struct FileOutcome {
  bool processed = false;
  int code = EC_OK;
  // language of the saved subtitles; empty when none were saved
  QString language;
};

int configChecks(const Console& c, const QNapiConfig& config) {
  QString p7zipPath = config.generalConfig().p7zipPath();
  if (!QFileInfo(p7zipPath).isExecutable()) {
    c.printLine(tr("Invalid path to p7zip executable: %1").arg(p7zipPath));
    return EC_P7ZIP_UNAVAILABLE;
  }

  QString tmpPath = config.generalConfig().tmpPath();
  QFileInfo tmp(tmpPath);
  if (!tmp.isDir() || !tmp.isWritable()) {
    c.printLine(tr("Can't write to temporary directory: %1").arg(tmpPath));
    return EC_CANNOT_WRITE_TMP_DIR;
  }

  return EC_OK;
}

bool findSubtitlesOnce(const Console& c, const QNapiConfig& config,
                       QNapi& napi) {
  bool found = false;

  SearchPolicy sp = config.generalConfig().searchPolicy();
  QString lang = config.generalConfig().language();
  QString langBackup = config.generalConfig().backupLanguage();

  if (sp == SP_SEARCH_ALL_WITH_BACKUP_LANG) {
    foreach (QString e, napi.listLoadedEngines()) {
      c.printLineOrdinary(
          tr("Searching for subtitles [%1] (%2)...").arg(lang, e));
      found = napi.lookForSubtitles(lang, e) || found;
      c.printLineOrdinary(
          tr("Searching for subtitles in alternative language [%1] (%2)...")
              .arg(langBackup, e));
      found = napi.lookForSubtitles(langBackup, e) || found;
    }
  } else {
    foreach (QString e, napi.listLoadedEngines()) {
      c.printLineOrdinary(
          tr("Searching for subtitles [%1] (%2)...").arg(lang, e));
      found = napi.lookForSubtitles(lang, e) || found;

      if (sp == SP_BREAK_IF_FOUND && found) break;
    }

    if (!found && !langBackup.isEmpty()) {
      foreach (QString e, napi.listLoadedEngines()) {
        c.printLineOrdinary(
            tr("Searching for subtitles in alternative language [%1] (%2)...")
                .arg(langBackup, e));
        found = napi.lookForSubtitles(langBackup, e) || found;

        if (sp == SP_BREAK_IF_FOUND && found) break;
      }
    }
  }

  return found;
}

bool findSubtitles(const Console& c, const QNapiConfig& config, QNapi& napi,
                   int maxRetries) {
  c.printLineOrdinary(tr("Calculating checksums..."));
  napi.checksum();

  bool found = findSubtitlesOnce(c, config, napi);

  for (int attempt = 1; !found && attempt <= maxRetries &&
                        napi.lastSearchFailedTransiently();
       ++attempt) {
    c.printLineOrdinary(
        tr("Search failed (%4) - retrying in %1 s (%2/%3)...")
            .arg(QNapi::searchRetryDelayMsecs / 1000)
            .arg(attempt)
            .arg(maxRetries)
            .arg(napi.error().split(QLatin1Char('\n')).first()));

    QThread::msleep(QNapi::searchRetryDelayMsecs);
    napi.clearSubtitlesList();
    found = findSubtitlesOnce(c, config, napi);
  }

  return found;
}

void printSubtitlesList(const Console& c, QNapi& napi) {
  c.printLineOrdinary(tr("0)\tDo not download subtitles for this video"));
  c.printLineOrdinary(tr("Found subtitles:"));

  int i = 1;

  foreach (SubtitleInfo s, napi.listSubtitles()) {
    QString resolution = "";

    if (s.resolution == SUBTITLE_GOOD)
      resolution = tr(" (good)");
    else if (s.resolution == SUBTITLE_BAD)
      resolution = tr(" (bad)");

    c.printLineOrdinary(QString("%1)\t%2 (%3) (%4) (%5)%6")
                            .arg(i++)
                            .arg(s.name)
                            .arg(s.format)
                            .arg(s.lang)
                            .arg(s.engine)
                            .arg(resolution));
  }
}

Maybe<int> selectSubtitles(const Console& c, const QNapiConfig& config,
                           QNapi& napi) {
  bool showList = false;
  bool napiShowList = napi.needToShowList();

  if(config.generalConfig().quietBatch() || config.generalConfig().downloadPolicy() == DP_NEVER_SHOW_LIST) {
    showList = false;
  } else if (config.generalConfig().downloadPolicy() == DP_ALWAYS_SHOW_LIST) {
    showList = true;
  } else {
    showList = napiShowList;
  }

  if (!showList) {
    return just(napi.bestIdx());
  } else {
    QList<SubtitleInfo> subtitlesList = napi.listSubtitles();

    printSubtitlesList(c, napi);
    int selIdx = c.inputNumber(tr("Select subtitles to download: "), 0,
                               subtitlesList.size());
    if (selIdx == 0) {
      return nothing();
    } else {
      return just(selIdx - 1);
    }
  }
}

int finishSubtitles(int selIdx, const Console& c, QNapi& napi) {
  c.printLineOrdinary(tr("Downloading subtitles..."));
  if (!napi.download(selIdx)) {
    c.printLineError(tr("Unable to download subtitles!"));
    if (!napi.error().isEmpty()) c.printLineError(napi.error());
    return EC_COULD_NOT_DOWNLOAD;
  }

  c.printLineOrdinary(tr("Unpacking subtitles..."));
  if (!napi.unpack(selIdx)) {
    c.printLineError(tr("Failed to unpack subtitles!"));
    if (!napi.error().isEmpty()) c.printLineError(napi.error());
    return EC_COULD_NOT_UNARCHIVE;
  }

  if (napi.ppEnabled()) {
    c.printLineOrdinary(tr("Post-processing subtitles file..."));
    napi.postProcessSubtitles();
  }

  c.printLineOrdinary(tr("Adjusting subtitles..."));
  if (!napi.matchSubtitles()) {
    c.printLineError(tr("Could not adjust subtitles!"));
    return EC_COULD_NOT_MATCH;
  }

  napi.cleanup();
  return 0;
}

int downloadForMovie(const Console& c, const QString& movieFilePath, int i,
                     int total, const QNapiConfig& config, QNapi& napi,
                     int maxRetries, QString* savedLanguage = nullptr) {
  QString movieFileName = QFileInfo(movieFilePath).fileName();
  c.printLineHighlihted(tr("Downloading subtitles for '%1' [%2/%3]")
                            .arg(movieFileName)
                            .arg(i)
                            .arg(total));

  napi.setMoviePath(movieFilePath);

  if (!napi.checkWritePermissions()) {
    c.printLineError(tr("No permission to write to the directory '%1'!")
                         .arg(QFileInfo(movieFilePath).path()));
    return EC_NO_WRITE_PERMISSIONS;
  }

  napi.clearSubtitlesList();

  bool found = findSubtitles(c, config, napi, maxRetries);

  if (!found) {
    c.printLineWarning(tr("Subtitles not found!"));
    if (!napi.error().isEmpty()) c.printLineWarning(napi.error());
    return EC_SUBTITLES_NOT_FOUND;
  }

  Maybe<int> selIdx = selectSubtitles(c, config, napi);

  if (selIdx) {
    const int result = finishSubtitles(selIdx.value(), c, napi);
    if (result == EC_OK && savedLanguage != nullptr)
      *savedLanguage = napi.subtitleLanguage();
    return result;
  }

  napi.cleanup();

  return EC_OK;
}

// Files with no subtitles in the main language: none found, only the backup
// language, a download that failed, or still rate-limited. Left out are files
// the user declined in the list and files whose directory is not writable.
// (A later regular run replaces AI subtitles with downloaded ones.)
QStringList filesNeedingAiFallback(const Console& c,
                                   const QStringList& movieFilePaths,
                                   const QVector<FileOutcome>& outcomes,
                                   const QNapiConfig& config) {
  const QString mainLanguage =
      SubtitleLanguage(config.generalConfig().language()).toTwoLetter();
  const QStringList subtitleExtensions =
      LibQNapi::staticConfig()->subtitleExtensions();

  QStringList queue;
  for (int i = 0; i < movieFilePaths.size(); ++i) {
    const FileOutcome& o = outcomes[i];
    if (!o.processed) continue;

    bool needed = false;
    switch (o.code) {
      case EC_OK:
        // empty language: the user chose not to download anything
        needed = !o.language.isEmpty() &&
                 SubtitleLanguage(o.language).toTwoLetter() != mainLanguage;
        break;
      case EC_SUBTITLES_NOT_FOUND:
      case EC_COULD_NOT_DOWNLOAD:
      case EC_COULD_NOT_UNARCHIVE:
      case EC_COULD_NOT_MATCH:
        needed = true;
        break;
      default:
        needed = false;
    }
    if (!needed) continue;

    // never replace subtitles that are already there, human-made or not
    // without language suffixes, a Movie.srt saved by this very run is the
    // backup-language one and is meant to be replaced; an older one is not
    const bool unlabelled =
        config.generalConfig().languageSuffix() == LS_NONE && o.code != EC_OK;
    if (CliAiFallback::hasSubtitlesInLanguage(movieFilePaths[i], mainLanguage,
                                              subtitleExtensions, unlabelled)) {
      c.printLineOrdinary(
          tr("%1 - already has subtitles in the main language, no AI needed")
              .arg(QFileInfo(movieFilePaths[i]).fileName()));
      continue;
    }

    queue << movieFilePaths[i];
  }
  return queue;
}

void runAiFallbackIfEnabled(const Console& c,
                            const QStringList& movieFilePaths,
                            const QVector<FileOutcome>& outcomes,
                            const QNapiConfig& config) {
  if (!config.aiConfig().fallbackEnabled()) return;

  const QStringList queue =
      filesNeedingAiFallback(c, movieFilePaths, outcomes, config);
  if (queue.isEmpty()) {
    c.printLineOrdinary(tr("AI fallback: every file has subtitles already"));
    return;
  }

  CliAiFallback::generateSubtitles(c, queue, config);
}

// Asking the user to choose from a list of subtitles only works while files
// are handled one at a time, so parallel mode requires a non-interactive
// download policy.
bool canRunInParallel(const QNapiConfig& config) {
  return config.generalConfig().quietBatch() ||
         config.generalConfig().downloadPolicy() == DP_NEVER_SHOW_LIST;
}

// Each worker takes files off a shared counter and processes them with its own
// QNapi instance; nothing is shared but the counter and the console lock.
class DownloadWorker : public QThread {
 public:
  DownloadWorker(const Console& console, const QNapiConfig& config,
                 const QStringList& allPaths, const QList<int>& indices,
                 QAtomicInt& nextSlot, QMutex& sharedMutex,
                 QList<int>& failedIndices, QVector<FileOutcome>& outcomes,
                 QAtomicInt& criticalError, int round, int totalFiles)
      : console(console),
        config(config),
        allPaths(allPaths),
        indices(indices),
        nextSlot(nextSlot),
        sharedMutex(sharedMutex),
        failedIndices(failedIndices),
        outcomes(outcomes),
        criticalError(criticalError),
        round(round),
        totalFiles(totalFiles) {}

 protected:
  void run() override {
    // the per-file steps stay silent; one summary line is printed instead, so
    // the output of concurrent files does not interleave
    const Console silent(true);
    QNapi napi(config);

    for (;;) {
      if (criticalError.loadAcquire() != 0) return;

      const int slot = nextSlot.fetchAndAddOrdered(1);
      if (slot >= indices.size()) return;

      const int i = indices[slot];
      const QString moviePath = allPaths[i];
      const QString name = QFileInfo(moviePath).fileName();

      // Announce the start too: with several files in flight the only output
      // used to be on completion, so a slow engine looked like a hang.
      {
        QMutexLocker locker(&sharedMutex);
        console.printLineOrdinary(
            QString("[%1/%2] ").arg(i + 1).arg(totalFiles) + name + " ...");
      }

      // No per-file retry here: waiting between attempts is exactly what the
      // rate limiter punishes. Stragglers are retried as a group instead.
      QString savedLanguage;
      const int result = downloadForMovie(silent, moviePath, i + 1, totalFiles,
                                          config, napi, 0, &savedLanguage);

      {
        QMutexLocker locker(&sharedMutex);
        const QString prefix = QString("[%1/%2] ").arg(i + 1).arg(totalFiles);

        FileOutcome& outcome = outcomes[i];
        outcome.processed = true;
        outcome.code = result;
        outcome.language = savedLanguage;

        if (result == EC_OK) {
          console.printLineHighlihted(prefix + name);
        } else if (result == EC_P7ZIP_UNAVAILABLE ||
                   result == EC_CANNOT_WRITE_TMP_DIR) {
          QString reason = napi.error();
          reason = reason.split(QLatin1Char('\n')).first();
          console.printLineError(prefix + name + " - " + reason);
        } else if (napi.lastSearchFailedTransiently()) {
          // worth another go once the limiter has reset
          failedIndices.append(i);
        } else {
          QString reason = napi.error();
          reason = reason.split(QLatin1Char('\n')).first();
          if (reason.isEmpty()) reason = tr("could not download subtitles");
          console.printLineWarning(prefix + name + " - " + reason);
        }
      }

      if (result == EC_P7ZIP_UNAVAILABLE || result == EC_CANNOT_WRITE_TMP_DIR) {
        criticalError.storeRelease(result);
        return;
      }
    }
  }

 private:
  const Console& console;
  const QNapiConfig config;
  const QStringList& allPaths;
  const QList<int> indices;
  QAtomicInt& nextSlot;
  QMutex& sharedMutex;
  QList<int>& failedIndices;
  QVector<FileOutcome>& outcomes;
  QAtomicInt& criticalError;
  const int round;
  const int totalFiles;
};

int downloadSubtitlesFor(const Console& c, const QStringList& movieFilePaths,
                         const QNapiConfig& config) {
  int ccErr = configChecks(c, config);
  if (ccErr != 0) {
    return ccErr;
  }

  const int fileCount = movieFilePaths.size();
  const int jobs = qBound(
      1, qMin(config.generalConfig().jobs(), fileCount), GeneralConfig::maxJobs);

  if (jobs > 1 && canRunInParallel(config)) {
    // The services throttle requests arriving one after another but let a
    // handful in flight through, so this is the difference between seconds
    // and minutes for a whole season.
    // The limiter lets a burst of concurrent requests through and then needs
    // roughly half a minute before it accepts more. Spacing requests out is
    // what it punishes, so: fire everything, collect whatever got blocked,
    // wait once, and fire the stragglers together.
    c.printLineOrdinary(
        tr("Processing %1 files, %2 at a time...").arg(fileCount).arg(jobs));

    QAtomicInt criticalError(0);
    QMutex sharedMutex;
    QVector<FileOutcome> outcomes(fileCount);

    QList<int> pending;
    for (int i = 0; i < fileCount; ++i) pending << i;

    for (int round = 0; round <= maxRetryRounds && !pending.isEmpty(); ++round) {
      if (criticalError.loadAcquire() != 0) break;

      if (round > 0) {
        c.printLineOrdinary(
            tr("%n file(s) were rate-limited; waiting %1 s before retrying...",
               "", pending.size())
                .arg(rateLimitResetMsecs / 1000));
        QThread::msleep(rateLimitResetMsecs);
      }

      QList<int> failedIndices;
      QAtomicInt nextSlot(0);

      const int inRound = qMin(jobs, pending.size());

      QList<DownloadWorker*> workers;
      for (int w = 0; w < inRound; ++w) {
        workers << new DownloadWorker(c, config, movieFilePaths, pending,
                                      nextSlot, sharedMutex, failedIndices,
                                      outcomes, criticalError, round,
                                      fileCount);
      }
      foreach (DownloadWorker* worker, workers) { worker->start(); }
      foreach (DownloadWorker* worker, workers) {
        worker->wait();
        delete worker;
      }

      // stop as soon as a round rescues nothing; waiting again would only
      // burn 30s on files the service simply does not have
      const bool madeProgress = failedIndices.size() < pending.size();
      pending = failedIndices;
      if (!madeProgress) break;
    }

    // anything still pending has run out of rounds
    foreach (int i, pending) {
      c.printLineWarning(QString("[%1/%2] ")
                             .arg(i + 1)
                             .arg(fileCount) +
                         QFileInfo(movieFilePaths[i]).fileName() + " - " +
                         tr("still rate-limited, try again later"));
    }

    const int failed = criticalError.loadAcquire();
    if (failed != 0) return failed;

    runAiFallbackIfEnabled(c, movieFilePaths, outcomes, config);
    return EC_OK;
  }

  QNapi napi(config);
  QVector<FileOutcome> outcomes(fileCount);

  int total = movieFilePaths.size();
  for (int i = 1; i <= total; ++i) {
    QString movieFilePath = movieFilePaths[i - 1];

    QString savedLanguage;
    int result = downloadForMovie(c, movieFilePath, i, total, config, napi,
                                  QNapi::maxSearchRetries, &savedLanguage);

    outcomes[i - 1].processed = true;
    outcomes[i - 1].code = result;
    outcomes[i - 1].language = savedLanguage;

    if (result == EC_P7ZIP_UNAVAILABLE || result == EC_CANNOT_WRITE_TMP_DIR) {
      if (i < total) {
        c.printLineOrdinary(
            tr("Processing of remaining %n file(s) was ignored "
               "due to critical error.",
               "", total - i));
      }
      return result;
    }
  }

  runAiFallbackIfEnabled(c, movieFilePaths, outcomes, config);
  return EC_OK;
}
};  // namespace CliSubtitlesDownloader
