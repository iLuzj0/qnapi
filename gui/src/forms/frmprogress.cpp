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

#include "frmprogress.h"
#include <QCloseEvent>
#include <QGuiApplication>
#include <QScreen>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include "libqnapi.h"

#include <QMutexLocker>
#include <QThread>
#include "batchdownloader.h"
#include "qnapiapp.h"

frmProgress::frmProgress(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f) {
  qRegisterMetaType<SubtitleInfoList>("QNapiSubtitleInfoList");

  ui.setupUi(this);

  setAttribute(Qt::WA_DeleteOnClose, false);
  setAttribute(Qt::WA_QuitOnClose, false);

  setBatchMode(false);

  connect(&getThread, SIGNAL(fileNameChange(const QString &)), ui.lbFileName,
          SLOT(setText(const QString &)));
  connect(&getThread, SIGNAL(actionChange(const QString &)), ui.lbAction,
          SLOT(setText(const QString &)));
  connect(&getThread, SIGNAL(progressChange(int, int, float)), this,
          SLOT(updateProgress(int, int, float)));
  connect(&getThread, SIGNAL(selectSubtitles(QString, SubtitleInfoList)), this,
          SLOT(selectSubtitles(QString, SubtitleInfoList)));
  connect(this, SIGNAL(subtitlesSelected(int)), &getThread,
          SLOT(subtitlesSelected(int)));
  connect(&getThread, SIGNAL(finished()), this, SLOT(downloadFinished()));

  QRect position = frameGeometry();
  position.moveCenter(QGuiApplication::primaryScreen()->availableGeometry().center());
  move(position.topLeft());
}

void frmProgress::receiveRequest(const QString &request) {
  enqueueFile(request);
  if (!getThread.isRunning()) download(LibQNapi::loadConfig());
  raise();
  activateWindow();
}

void frmProgress::enqueueFile(const QString &filePath) {
  if (QFile::exists(filePath)) {
    getThread.enqueue(filePath);
    updateProgress(-1, getThread.queueSize(), -1);
  }
}

void frmProgress::enqueueFiles(const QStringList &fileList) {
  for (int i = 0; i < fileList.size(); i++) {
    enqueueFile(fileList.at(i));
  }
}

bool frmProgress::download(const QNapiConfig &config) {
  if (summary.isVisible()) {
    summary.close();
  }

  QNapi napi(config);

  // TODO: perform these checks earlier
  if (!napi.checkP7ZipPath()) {
    QMessageBox::warning(0, tr("Can not find p7zip!"),
                         tr("The path to the program p7zip is incorrect!"));
    return false;
  }

  if (!napi.checkTmpPath()) {
    QMessageBox::warning(
        0, tr("Invalid temporary directory!"),
        tr("Unable to write to the temporary directory! Check your settings."));
    return false;
  }

  if (getThread.queueSize() == 0) {
    QMessageBox::warning(
        0, tr("No files!"),
        tr("Can't download subtitles as no movie files specified!"));
    return false;
  }

  if (!isVisible()) {
    QRect position = frameGeometry();
    position.moveCenter(QGuiApplication::primaryScreen()->availableGeometry().center());
    move(position.topLeft());
    show();
  }

  showSummary = true;
  closeRequested = false;
  ui.pbCancel->setEnabled(true);

  getThread.setConfig(config);
  getThread.start();
  return true;
}

void frmProgress::updateProgress(int current, int all, float stageProgress) {
  static int lastCurrent, lastAll;
  static float lastStageProgress;

  static QMutex m;
  m.lock();

  if (current >= 0) lastCurrent = current;
  if (all >= 0) lastAll = all;
  if (stageProgress >= 0) lastStageProgress = stageProgress;

  QString windowTitle =
      (lastAll > 1) ? QString(tr("QNapi - downloading subtitles (%1/%2)"))
                          .arg(lastCurrent + 1)
                          .arg(lastAll)
                    : QString(tr("QNapi - downloading subtitles..."));
  setWindowTitle(windowTitle);

  ui.pbProgress->setMaximum(lastAll * 100);
  ui.pbProgress->setValue(lastCurrent * 100 + (int)(lastStageProgress * 100));

  m.unlock();
}

void frmProgress::selectSubtitles(QString fileName,
                                  SubtitleInfoList subtitles) {
  frmSelect.setFileName(fileName);
  frmSelect.setSubtitlesList(subtitles);

  int selIdx;

  if (frmSelect.exec() == QDialog::Accepted) {
    selIdx = frmSelect.getSelectedIndex();
  } else {
    selIdx = -1;
  }

  emit subtitlesSelected(selIdx);
}

void frmProgress::downloadFinished() {
  hide();

  const QStringList queue = getThread.takeQueue();

  mutex.lock();
  if (showSummary) {
    if (!getThread.criticalMessage.isEmpty()) {
      QMessageBox::critical(0, tr("Critical error!"),
                            getThread.criticalMessage);
    } else if (queue.size() > 0 && !getThread.subStatusList.isEmpty()) {
      summary.setSummaryList(getThread.subStatusList);
      summary.exec();
    }
  }

  if (closeRequested) close();

  mutex.unlock();

  // a file sent just as the thread was finishing would otherwise be dropped
  if (!closeRequested && getThread.queueSize() > 0) {
    download(LibQNapi::loadConfig());
    return;
  }

  if (batchMode) qApp->quit();
}

void frmProgress::closeEvent(QCloseEvent *event) {
  if (getThread.isRunning()) {
    if (QMessageBox::question(
            this, tr("QNapi"),
            tr("Do you want to cancel subtitles downloading?"),
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
      mutex.lock();
      showSummary = false;
      getThread.requestAbort();
      ui.lbAction->setText(tr("Finishing the tasks..."));
      ui.lbFileName->setText("");
      ui.pbCancel->setEnabled(false);
      qApp->processEvents();
      closeRequested = true;
      mutex.unlock();
      event->ignore();
    } else {
      event->ignore();
    }
  } else {
    event->accept();
  }
}

void frmProgress::dragEnterEvent(QDragEnterEvent *event) {
  QUrl url(event->mimeData()->urls().at(0));
  QFileInfo fi(url.toLocalFile());
  if (fi.exists() && fi.isFile()) event->acceptProposedAction();
}

void frmProgress::dropEvent(QDropEvent *event) {
  QList<QUrl> urlList;
  QFileInfo info;

  urlList = event->mimeData()->urls();

  foreach (QUrl url, urlList) {
    info.setFile(url.toLocalFile());
    if (!info.exists() || !info.isFile()) continue;
    enqueueFile(url.toLocalFile());
  }
}

GetThread::GetThread() : config(LibQNapi::loadConfig()) {
  connect(this, SIGNAL(criticalError(const QString &)), this,
          SLOT(setCriticalMessage(const QString &)));
}

void GetThread::subtitlesSelected(int idx) {
  selIdx = idx;
  selectionDone.release();
}

void GetThread::enqueue(const QString &filePath) {
  QMutexLocker locker(&queueMutex);
  queue << filePath;
}

int GetThread::queueSize() const {
  QMutexLocker locker(&queueMutex);
  return queue.size();
}

QStringList GetThread::takeQueue() {
  QMutexLocker locker(&queueMutex);
  const QStringList taken = queue.mid(0, dispatched);
  queue = queue.mid(dispatched);
  dispatched = 0;
  return taken;
}

// Turns the downloader's callbacks, which arrive from worker threads in
// parallel mode, into the thread's signals; Qt queues them to the GUI.
class GuiBatchListener : public BatchDownloadListener {
 public:
  GuiBatchListener(GetThread &thread, const QStringList &paths, int offset)
      : t(thread), paths(paths), offset(offset) {}

  void batchStarted(int fileCount, int jobs) override {
    parallel = jobs > 1;
    if (parallel) {
      emit t.fileNameChange(QString());
      emit t.actionChange(
          GetThread::tr("Processing %n file(s), %1 at a time...", "",
                        fileCount)
              .arg(jobs));
    }
    emit t.progressChange(offset, t.queueSize(), 0.0f);
  }

  void fileStarted(int index) override {
    QMutexLocker locker(&mutex);
    if (parallel) {
      inFlight << index;
      showParallelState();
    } else {
      emit t.fileNameChange(name(index));
    }
  }

  void fileStep(int index, DownloadStep step,
                const DownloadStepInfo &info) override {
    if (parallel) return;

    QString action;
    float stage = 0.0f;
    switch (step) {
      case DownloadStep::CheckingPermissions:
        action = GetThread::tr("Checking permissions of the video directory...");
        stage = 0.1f;
        break;
      case DownloadStep::Checksum:
        action = GetThread::tr("Calculating checksum of the file...");
        stage = 0.3f;
        break;
      case DownloadStep::Searching:
        action = GetThread::tr("Searching for subtitles [%1] (%2)...")
                     .arg(info.language, info.engine);
        stage = 0.4f;
        break;
      case DownloadStep::SearchingBackup:
        action =
            GetThread::tr(
                "Searching for subtitles in alternative language [%1] (%2)...")
                .arg(info.language, info.engine);
        stage = 0.45f;
        break;
      case DownloadStep::WaitingToRetry:
        action = GetThread::tr("Nothing found, retrying in %1 s (%2/%3)...")
                     .arg(info.delayMsecs / 1000)
                     .arg(info.attempt)
                     .arg(info.maxAttempts);
        stage = 0.45f;
        break;
      case DownloadStep::Downloading:
        action = GetThread::tr("Downloading subtitles file...");
        stage = 0.5f;
        break;
      case DownloadStep::Unpacking:
        action = GetThread::tr("Unpacking subtitles file...");
        stage = 0.65f;
        break;
      case DownloadStep::PostProcessing:
        action = GetThread::tr("Post-processing subtitles...");
        stage = 0.8f;
        break;
      case DownloadStep::Matching:
        action = GetThread::tr("Adjusting subtitles...");
        stage = 0.9f;
        break;
    }
    emit t.actionChange(action);
    emit t.progressChange(offset + index, t.queueSize(), stage);
  }

  void fileDeferred(int index) override {
    QMutexLocker locker(&mutex);
    inFlight.removeAll(index);
    showParallelState();
  }

  void fileFinished(int index, const DownloadOutcome &outcome) override {
    Q_UNUSED(outcome)
    QMutexLocker locker(&mutex);
    ++done;
    if (parallel) {
      inFlight.removeAll(index);
      showParallelState();
    } else {
      emit t.progressChange(offset + index, t.queueSize(), 1.0f);
    }
  }

  void waitingForRateLimit(int fileCount, int msecs) override {
    emit t.fileNameChange(QString());
    emit t.actionChange(
        GetThread::tr(
            "%n file(s) were rate-limited; waiting %1 s before retrying...", "",
            fileCount)
            .arg(msecs / 1000));
  }

  bool canSelectSubtitles() const override { return true; }
  bool canSelectSubtitlesConcurrently() const override { return true; }

  Maybe<int> selectSubtitles(int index, const SubtitleInfoList &list) override {
    QMutexLocker locker(&t.selectionMutex);
    if (t.abortRequested()) return nothing();

    emit t.selectSubtitles(name(index), list);
    t.selectionDone.acquire();

    if (t.selIdx < 0) return nothing();
    return just(t.selIdx);
  }

  bool abortRequested() const override { return t.abortRequested(); }

 private:
  QString name(int index) const { return QFileInfo(paths[index]).fileName(); }

  // called with the mutex held
  void showParallelState() {
    QString files;
    if (!inFlight.isEmpty()) {
      files = name(inFlight.last());
      if (inFlight.size() > 1)
        files += GetThread::tr(" (+%n more)", "", inFlight.size() - 1);
    }
    const int total = t.queueSize();
    emit t.fileNameChange(files);
    emit t.actionChange(
        GetThread::tr("%1 of %2 files done, %3 in progress")
            .arg(offset + done)
            .arg(total)
            .arg(inFlight.size()));
    // the window shows "current + 1"; a finished file is a whole step
    emit t.progressChange(qMax(0, offset + done - 1), total,
                          offset + done > 0 ? 1.0f : 0.0f);
  }

  GetThread &t;
  const QStringList paths;
  const int offset;
  bool parallel = false;
  int done = 0;
  QList<int> inFlight;
  QMutex mutex;
};

namespace {

SubtitleInfo summaryEntry(const QString &moviePath, const DownloadOutcome &o) {
  switch (o.result) {
    case DownloadResult::Saved:
      return o.subtitle;
    case DownloadResult::Declined:
    case DownloadResult::Aborted:
      return SubtitleInfo::fromFailed(moviePath);
    case DownloadResult::NoWritePermission:
      return SubtitleInfo::fromFailed(
          moviePath, GetThread::tr("No permission to write to the directory "
                                   "'%1'!")
                         .arg(QFileInfo(moviePath).path()));
    case DownloadResult::RateLimited:
      return SubtitleInfo::fromFailed(
          moviePath, GetThread::tr("still rate-limited, try again later"));
    case DownloadResult::MatchFailed:
      return SubtitleInfo::fromFailed(
          moviePath, GetThread::tr("Could not adjust subtitles!"));
    case DownloadResult::NotFound:
    case DownloadResult::DownloadFailed:
    case DownloadResult::UnpackFailed:
      break;
  }
  return SubtitleInfo::fromFailed(moviePath, o.error);
}

}  // namespace

void GetThread::run() {
  abort = false;
  criticalMessage.clear();
  subStatusList.clear();

  const auto downloader = LibQNapi::batchDownloader(config, specificEngine);

  QStringList paths;
  QVector<DownloadOutcome> outcomes;

  for (;;) {
    QStringList batch;
    {
      QMutexLocker locker(&queueMutex);
      batch = queue.mid(dispatched);
      dispatched = queue.size();
    }
    if (batch.isEmpty()) break;

    GuiBatchListener listener(*this, batch, paths.size());
    outcomes += downloader->run(batch, listener);
    paths += batch;

    if (abort) return;
  }

  for (int i = 0; i < paths.size(); ++i) {
    if (outcomes[i].processed)
      subStatusList << summaryEntry(paths[i], outcomes[i]);
  }
}
