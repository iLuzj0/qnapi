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

#ifndef __FRMOPTIONS__H__
#define __FRMOPTIONS__H__

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGuiApplication>
#include <QList>
#include <QMessageBox>
#include <QScreen>
#include <QSharedPointer>
#include <QTextCodec>

#include "config/qnapiconfig.h"
#include "engines/subtitledownloadenginesregistry.h"
#include "subconvert/subtitleformatsregistry.h"
#include "subtitlelanguage.h"
#include "ui_frmoptions.h"

class frmOptions : public QDialog {
  Q_OBJECT
 public:
  frmOptions(QWidget *parent = 0, Qt::WindowFlags f = Qt::WindowFlags());
  ~frmOptions();

 public slots:
  void writeConfig();
  void readConfig(const QNapiConfig &config);

 private slots:
  void le7zPathChanged();
  void select7zPath();
  void leTmpPathChanged();
  void selectTmpPath();

  void twEnginesSelectionChanged();
  void twEnginesItemChanged(QTableWidgetItem *item);
  void pbMoveUpClicked();
  void pbMoveDownClicked();
  void pbEngineConfClicked();
  void pbEngineInfoClicked();

  void subFormatChanged(int format);
  void encodingMethodChanged(int method);
  void autoDetectEncodingClicked();
  void showAllEncodingsClicked();

  void restoreDefaults();

 private:
  Ui::frmOptions ui;

  QSharedPointer<const SubtitleFormatsRegistry> subtitleFormatsRegistry;
  QSharedPointer<const SubtitleDownloadEnginesRegistry> enginesRegistry;

  void showAllEncodings();

  AiConfig aiConfigFromForm(const AiConfig &base) const;
  QString currentDefaultGuidelines() const;
  void aiLanguagesChanged();
  void updateAiStatus();

  // the default guidelines last put in the editor, so a language change can
  // tell an untouched default from the user's own text
  QString shownDefaultGuidelines;
};

#endif
