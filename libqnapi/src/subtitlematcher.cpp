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

#include "subtitlematcher.h"

#include <QDir>

#include "subtitlelanguage.h"

SubtitleMatcher::SubtitleMatcher(
    bool _noBackup, bool _isPostProcessingEnabled, QString _ppSubFormat,
    QString _ppSubExtension, bool _changePermissions,
    QString _changePermissionsTo,
    const QSharedPointer<const SubtitleFormatsRegistry>&
        subtitleFormatsRegistry,
    LanguageSuffix _languageSuffix)
    : noBackup(_noBackup),
      isPostProcessingEnabled(_isPostProcessingEnabled),
      ppSubFormat(_ppSubFormat),
      ppSubExtension(_ppSubExtension),
      changePermissions(_changePermissions),
      changePermissionsTo(_changePermissionsTo),
      subtitleFormatsRegistry(subtitleFormatsRegistry),
      languageSuffix(_languageSuffix) {}

QString SubtitleMatcher::languageCodeFor(
    const QString& subtitleLanguage) const {
  if (languageSuffix == LS_NONE || subtitleLanguage.isEmpty()) return QString();

  SubtitleLanguage lang(subtitleLanguage);

  return languageSuffix == LS_THREE_LETTER ? lang.toTriLetter()
                                           : lang.toTwoLetter();
}

bool SubtitleMatcher::matchSubtitles(QString subtitlesTmpFilePath,
                                     QString targetMovieFilePath,
                                     QString subtitleLanguage) const {
  QFileInfo subtitlesTmpFileInfo(subtitlesTmpFilePath);

  if (!subtitlesTmpFileInfo.exists()) return false;

  QString targetExtension = selectTargetExtension(subtitlesTmpFileInfo);
  QString languageCode = languageCodeFor(subtitleLanguage);

  QString targetSubtitlesFilePath =
      constructSubtitlePath(targetMovieFilePath, targetExtension, languageCode);

  if (!isWritablePath(targetSubtitlesFilePath)) return false;

  removeOrCopy(targetMovieFilePath, targetSubtitlesFilePath, languageCode);

  bool result = false;

#ifdef Q_OS_WIN
  // Pod windowsem, aby "wyczyscic" atrybuty pliku, tworzymy go na nowo
  result = dryCopy(subtitlesTmpFilePath, targetSubtitlesFilePath);
#else
  // pod innymi OS-ami wystarczy skopiowac plik
  result = QFile::copy(subtitlesTmpFilePath, targetSubtitlesFilePath);
#endif

  if (changePermissions) {
    fixFilePermissions(targetSubtitlesFilePath, changePermissionsTo);
  }

  return result;
}

QString SubtitleMatcher::selectTargetExtension(
    QFileInfo subtitlesTmpFileInfo) const {
  QString targetExtension = subtitlesTmpFileInfo.suffix();

  if (isPostProcessingEnabled) {
    if (!ppSubFormat.isEmpty() && ppSubExtension.isEmpty()) {
      // an unrecognised format name yields a null pointer from select();
      // keep the downloaded file's own extension rather than crashing
      QSharedPointer<const SubtitleFormat> format =
          subtitleFormatsRegistry->select(ppSubFormat);
      if (!format.isNull()) {
        targetExtension = format->defaultExtension();
      }
    } else if (!ppSubExtension.isEmpty()) {
      targetExtension = ppSubExtension;
    }
  }

  return targetExtension;
}

QString SubtitleMatcher::constructSubtitlePath(QString targetMovieFilePath,
                                               QString targetExtension,
                                               QString languageCode,
                                               QString baseSuffix) const {
  QFileInfo targetMovieFileInfo(targetMovieFilePath);

  QString name = targetMovieFileInfo.completeBaseName() + baseSuffix;
  if (!languageCode.isEmpty()) name += "." + languageCode;

  return targetMovieFileInfo.path() + QDir::separator() + name + "." +
         targetExtension;
}

bool SubtitleMatcher::isWritablePath(QString path) const {
  QFileInfo pathFileInfo(path);
  return QFileInfo(pathFileInfo.absolutePath()).isWritable();
}

void SubtitleMatcher::removeOrCopy(QString targetMoviefilePath,
                                   QString targetSubtitlesFilePath,
                                   QString languageCode) const {
  if (QFile::exists(targetSubtitlesFilePath)) {
    if (!noBackup) {
      QFileInfo targetSubtitlesFileInfo(targetSubtitlesFilePath);
      QString newName = constructSubtitlePath(targetMoviefilePath,
                                              targetSubtitlesFileInfo.suffix(),
                                              languageCode, tr("_copy"));

      if (QFile::exists(newName)) QFile::remove(newName);

      QFile::rename(targetSubtitlesFilePath, newName);
    } else
      QFile::remove(targetSubtitlesFilePath);
  }
}

bool SubtitleMatcher::dryCopy(QString srcFilePath, QString dstFilePath) const {
  QFile dstFile(dstFilePath), srcFile(srcFilePath);
  bool result = false;

  if (!dstFile.open(QIODevice::WriteOnly) ||
      !srcFile.open(QIODevice::ReadOnly)) {
    dstFile.close();
  } else {
    const QByteArray content = srcFile.readAll();
    // write() returns the byte count, so an empty source file is not an error
    result = dstFile.write(content) == content.size();
    srcFile.close();
    dstFile.close();
  }

  return result;
}

void SubtitleMatcher::fixFilePermissions(QString targetSubtitlesFilePath,
                                         QString changePermissionsTo) const {
  bool validPermissions;
  int permInt = changePermissionsTo.toInt(&validPermissions, 8);

  if (validPermissions) {
    int perm = 0;
    perm |= (permInt & 0700) << 2;
    perm |= (permInt & 0070) << 1;
    perm |= (permInt & 0007);
    QFile::setPermissions(targetSubtitlesFilePath, QFile::Permissions(perm));
  }
}
