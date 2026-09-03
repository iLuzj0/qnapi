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

#ifndef NAPIPROJEKTDOWNLOADENGINE_H
#define NAPIPROJEKTDOWNLOADENGINE_H

#include "config/engineconfig.h"
#include "engines/subtitledownloadengine.h"
#include "utils/p7zipdecoder.h"

#include <QSharedPointer>

class NapiProjektDownloadEngine : public SubtitleDownloadEngine {
 public:
  NapiProjektDownloadEngine(
      const QString& tmpPath, const EngineConfig& config,
      const QSharedPointer<const P7ZipDecoder>& p7zipDecoder);
  ~NapiProjektDownloadEngine();

  static SubtitleDownloadEngineMetadata metadata;
  static const char* const pixmapData[];

  SubtitleDownloadEngineMetadata meta() const;
  const char* const* enginePixmapData() const;

  QString checksum(QString filename = "");
  bool lookForSubtitles(QString lang);
  QList<SubtitleInfo> listSubtitles();
  bool download(QUuid id);
  bool unpack(QUuid id);
  void cleanup();

  static bool checkUser(const QString& nick, const QString& pass);

 private:
  EngineConfig engineConfig;
  QSharedPointer<const P7ZipDecoder> p7zipDecoder;

  QString checksum(QString filename, bool limit10M);

  // Preferred path: the API returns the subtitles as plain text and is not
  // behind the Cloudflare rate limiter that guards dl.php.
  Maybe<QByteArray> fetchSubtitlesViaApi(QString lang, QString checksum) const;

  // Legacy path, kept as a fallback when the API cannot be reached. Returns a
  // password-protected 7z archive.
  Maybe<QString> downloadByLangAndChecksum(QString lang,
                                           QString checksum) const;
  QString npFDigest(const QString& input) const;
  QString npLangWrapper(QString lang) const;
  QString napiOS() const;
};

#endif
