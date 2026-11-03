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

#ifndef __FRMPROGRESS__H__
#define __FRMPROGRESS__H__

#include <QMutex>
#include <QSemaphore>

#include "ui_frmprogress.h"

#include "frmsummary.h"

#include "config/qnapiconfig.h"

#include <Maybe.h>
#include "frmlistsubtitles.h"
#include "qnapi.h"
#include "qnapiopendialog.h"
#include "qnapithread.h"
#include "subtitleinfo.h"

class GetThread : public QNapiThread {
  Q_OBJECT

  friend class GuiBatchListener;

 public:
  GetThread();

 signals:
  void fileNameChange(const QString &newfileName);
  void actionChange(const QString &newAction);
  void progressChange(int current, int all, float stageProgress);
  void criticalError(const QString &message);
  void selectSubtitles(QString fileName, SubtitleInfoList subtitles);

 private slots:
  void setCriticalMessage(const QString &msg) { criticalMessage = msg; }
  void subtitlesSelected(int idx);

 public:
  void setSpecificEngine(Maybe<QString> engine) { specificEngine = engine; }

  void setConfig(const QNapiConfig &configuration) { config = configuration; }

  void run();
  bool abortRequested() const { return abort; }

  // Files may arrive from a second launch while a batch is running; they are
  // processed as a follow-up batch.
  void enqueue(const QString &filePath);
  int queueSize() const;
  QStringList takeQueue();

  Maybe<QString> specificEngine;
  QList<SubtitleInfo> subStatusList;
  QString criticalMessage;
  QNapiConfig config;

 private:
  QStringList queue;
  // files of the current run handed to the downloader so far
  int dispatched = 0;
  mutable QMutex queueMutex;

  // One subtitle list at a time even when several workers want to ask.
  QMutex selectionMutex;
  QSemaphore selectionDone;
  int selIdx = -1;
};

class frmProgress : public QWidget {
  Q_OBJECT

 public:
  frmProgress(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());

  void clearSpecificEngine() { getThread.setSpecificEngine(nothing()); }
  void setSpecificEngine(QString engine) {
    getThread.setSpecificEngine(just(engine));
  }

  void setBatchMode(bool value) { batchMode = value; }
  bool isBatchMode() { return batchMode; }

 signals:
  void subtitlesSelected(int idx);

 public slots:
  void receiveRequest(const QString &request);
  void enqueueFile(const QString &file);
  void enqueueFiles(const QStringList &fileList);
  bool download(const QNapiConfig &config);
  void updateProgress(int current, int all, float stageProgress);
  void selectSubtitles(QString fileName, SubtitleInfoList subtitles);
  void downloadFinished();

 private:
  void closeEvent(QCloseEvent *event);
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);

  Ui::frmProgress ui;
  GetThread getThread;
  frmListSubtitles frmSelect;
  frmSummary summary;

  bool batchMode, showSummary, closeRequested;
  QMutex mutex;
};

#endif
