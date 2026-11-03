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

#ifndef SUBTITLE_MATCHER_H
#define SUBTITLE_MATCHER_H

#include <QFileInfo>
#include <QSharedPointer>

#include "config/generalconfig.h"
#include "subconvert/subtitleformatsregistry.h"

class SubtitleMatcher : public QObject {
  Q_OBJECT
 public:
  SubtitleMatcher(bool _noBackup, bool _isPostProcessingEnabled,
                  QString _ppSubFormat, QString _ppSubExtension,
                  bool _changePermissions, QString _changePermissionsTo,
                  const QSharedPointer<const SubtitleFormatsRegistry>
                      &subtitleFormatsRegistry,
                  LanguageSuffix _languageSuffix = LS_NONE);

  // subtitleLanguage is the language actually downloaded; when the language
  // suffix is enabled it becomes part of the file name, e.g. Movie.pol.srt
  bool matchSubtitles(QString subtitlesTmpFilePath, QString targetMovieFilePath,
                      QString subtitleLanguage = QString()) const;

 private:
  QString selectTargetExtension(QFileInfo subtitlesTmpFileInfo) const;
  QString constructSubtitlePath(QString targetMovieFilePath,
                                QString targetExtension,
                                QString languageCode = "",
                                QString baseSuffix = "") const;
  QString languageCodeFor(const QString &subtitleLanguage) const;
  bool isWritablePath(QString path) const;
  void removeOrCopy(QString targetMoviefilePath,
                    QString targetSubtitlesFilePath,
                    QString languageCode) const;
  bool dryCopy(QString srcFilePath, QString dstFilePath) const;
  void fixFilePermissions(QString targetSubtitlesFilePath,
                          QString changePermissionsTo) const;

  bool noBackup;
  bool isPostProcessingEnabled;
  QString ppSubFormat;
  QString ppSubExtension;
  bool changePermissions;
  QString changePermissionsTo;
  LanguageSuffix languageSuffix;

  QSharedPointer<const SubtitleFormatsRegistry> subtitleFormatsRegistry;
};

#endif  // SUBTITLE_MATCHER_H
