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

#include <QFileInfo>
#include <QMutex>
#include <QMutexLocker>

#include "cliaifallback.h"
#include "libqnapi.h"

namespace CliSubtitlesDownloader {

enum ExitCode {
  EC_OK = 0,
  EC_P7ZIP_UNAVAILABLE = 2,
  EC_CANNOT_WRITE_TMP_DIR = 3
};

namespace {

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

QString firstLine(const QString& text) {
  return text.split(QLatin1Char('\n')).first();
}

// Sequentially every step is printed; in parallel the output of concurrent
// files would interleave, so each file gets a line when it starts and one
// when it is done.
class ConsoleListener : public BatchDownloadListener {
 public:
  ConsoleListener(const Console& c, const QNapiConfig& config,
                  const QStringList& movieFilePaths)
      : c(c), config(config), paths(movieFilePaths) {}

  void batchStarted(int fileCount, int jobs) override {
    parallel = jobs > 1;
    if (parallel) {
      c.printLineOrdinary(
          tr("Processing %1 files, %2 at a time...").arg(fileCount).arg(jobs));
    }
  }

  void fileStarted(int index) override {
    QMutexLocker locker(&mutex);
    if (parallel) {
      c.printLineOrdinary(prefix(index) + name(index) + " ...");
    } else {
      c.printLineHighlihted(tr("Downloading subtitles for '%1' [%2/%3]")
                                .arg(name(index))
                                .arg(index + 1)
                                .arg(paths.size()));
    }
  }

  void fileStep(int index, DownloadStep step,
                const DownloadStepInfo& info) override {
    Q_UNUSED(index)
    if (parallel) return;

    switch (step) {
      case DownloadStep::CheckingPermissions:
        break;
      case DownloadStep::Checksum:
        c.printLineOrdinary(tr("Calculating checksums..."));
        break;
      case DownloadStep::Searching:
        c.printLineOrdinary(tr("Searching for subtitles [%1] (%2)...")
                                .arg(info.language, info.engine));
        break;
      case DownloadStep::SearchingBackup:
        c.printLineOrdinary(
            tr("Searching for subtitles in alternative language [%1] (%2)...")
                .arg(info.language, info.engine));
        break;
      case DownloadStep::WaitingToRetry:
        c.printLineOrdinary(
            tr("Search failed (%4) - retrying in %1 s (%2/%3)...")
                .arg(info.delayMsecs / 1000)
                .arg(info.attempt)
                .arg(info.maxAttempts)
                .arg(info.reason));
        break;
      case DownloadStep::Downloading:
        c.printLineOrdinary(tr("Downloading subtitles..."));
        break;
      case DownloadStep::Unpacking:
        c.printLineOrdinary(tr("Unpacking subtitles..."));
        break;
      case DownloadStep::PostProcessing:
        c.printLineOrdinary(tr("Post-processing subtitles file..."));
        break;
      case DownloadStep::Matching:
        c.printLineOrdinary(tr("Adjusting subtitles..."));
        break;
    }
  }

  void fileFinished(int index, const DownloadOutcome& o) override {
    QMutexLocker locker(&mutex);
    if (parallel) {
      printParallelOutcome(index, o);
    } else {
      printSequentialOutcome(index, o);
    }
  }

  void waitingForRateLimit(int fileCount, int msecs) override {
    c.printLineOrdinary(
        tr("%n file(s) were rate-limited; waiting %1 s before retrying...", "",
           fileCount)
            .arg(msecs / 1000));
  }

  bool canSelectSubtitles() const override {
    return !config.generalConfig().quietBatch();
  }

  Maybe<int> selectSubtitles(int index, const SubtitleInfoList& list) override {
    Q_UNUSED(index)
    printSubtitlesList(list);
    const int selIdx =
        c.inputNumber(tr("Select subtitles to download: "), 0, list.size());
    if (selIdx == 0) return nothing();
    return just(selIdx - 1);
  }

 private:
  QString name(int index) const { return QFileInfo(paths[index]).fileName(); }

  QString prefix(int index) const {
    return QString("[%1/%2] ").arg(index + 1).arg(paths.size());
  }

  void printSequentialOutcome(int index, const DownloadOutcome& o) const {
    switch (o.result) {
      case DownloadResult::Saved:
      case DownloadResult::Declined:
      case DownloadResult::Aborted:
        break;
      case DownloadResult::NoWritePermission:
        c.printLineError(tr("No permission to write to the directory '%1'!")
                             .arg(QFileInfo(paths[index]).path()));
        break;
      case DownloadResult::NotFound:
      case DownloadResult::RateLimited:
        c.printLineWarning(tr("Subtitles not found!"));
        if (!o.error.isEmpty()) c.printLineWarning(o.error);
        break;
      case DownloadResult::DownloadFailed:
        c.printLineError(tr("Unable to download subtitles!"));
        if (!o.error.isEmpty()) c.printLineError(o.error);
        break;
      case DownloadResult::UnpackFailed:
        c.printLineError(tr("Failed to unpack subtitles!"));
        if (!o.error.isEmpty()) c.printLineError(o.error);
        break;
      case DownloadResult::MatchFailed:
        c.printLineError(tr("Could not adjust subtitles!"));
        break;
    }
  }

  void printParallelOutcome(int index, const DownloadOutcome& o) const {
    const QString line = prefix(index) + name(index);
    switch (o.result) {
      case DownloadResult::Saved:
        c.printLineHighlihted(line);
        break;
      case DownloadResult::Declined:
      case DownloadResult::Aborted:
        break;
      case DownloadResult::RateLimited:
        c.printLineWarning(line + " - " +
                           tr("still rate-limited, try again later"));
        break;
      default: {
        QString reason = firstLine(o.error);
        if (reason.isEmpty()) reason = tr("could not download subtitles");
        c.printLineWarning(line + " - " + reason);
      }
    }
  }

  void printSubtitlesList(const SubtitleInfoList& list) const {
    c.printLineOrdinary(tr("0)\tDo not download subtitles for this video"));
    c.printLineOrdinary(tr("Found subtitles:"));

    int i = 1;
    foreach (SubtitleInfo s, list) {
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

  const Console& c;
  const QNapiConfig config;
  const QStringList paths;
  bool parallel = false;
  QMutex mutex;
};

void runAiFallbackIfEnabled(const Console& c, const QStringList& movieFilePaths,
                            const QVector<DownloadOutcome>& outcomes,
                            const QNapiConfig& config) {
  if (!config.aiConfig().fallbackEnabled()) return;

  QStringList alreadyCovered;
  const QStringList queue =
      LibQNapi::aiSubtitleGenerator(config)->filesNeedingSubtitles(
          movieFilePaths, outcomes, &alreadyCovered);

  foreach (const QString& movie, alreadyCovered) {
    c.printLineOrdinary(
        tr("%1 - already has subtitles in the main language, no AI needed")
            .arg(QFileInfo(movie).fileName()));
  }

  if (queue.isEmpty()) {
    c.printLineOrdinary(tr("AI fallback: every file has subtitles already"));
    return;
  }

  CliAiFallback::generateSubtitles(c, queue, config);
}

}  // namespace

int downloadSubtitlesFor(const Console& c, const QStringList& movieFilePaths,
                         const QNapiConfig& config) {
  const int ccErr = configChecks(c, config);
  if (ccErr != EC_OK) return ccErr;

  ConsoleListener listener(c, config, movieFilePaths);
  const QVector<DownloadOutcome> outcomes =
      LibQNapi::batchDownloader(config)->run(movieFilePaths, listener);

  runAiFallbackIfEnabled(c, movieFilePaths, outcomes, config);
  return EC_OK;
}

};  // namespace CliSubtitlesDownloader
