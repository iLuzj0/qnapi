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

#include "napiprojektdownloadengine.h"
#include "subtitlelanguage.h"
#include "utils/synchttp.h"

#include <cmath>

#include <QCryptographicHash>
#include <QDomDocument>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QProcess>
#include <QUrl>

#ifdef Q_OS_WIN  // for SetFileAttributes
#include <qt_windows.h>
#endif

// This sadly has to be added in order to build under VS >= 2005
#ifdef _MSC_VER
// from Visual Studio 2005 onwards
// https://msdn.microsoft.com/en-us/library/2ts7cx93%28v=vs.80%29.aspx
#include <stdio.h>
#define snprintf _snprintf_s
#endif

namespace NapiProjektDownloadEngineConsts {
const unsigned long NAPI_10MB = 10485760;

const QString napiZipPassword = "iBlm8NTigvru0Jr0";
const QString napiDownloadUrlTpl =
    "http://www.napiprojekt.pl/unit_napisy/dl.php"
    "?l=%1&f=%2&t=%3&v=other&kolejka=false&nick=%4&pass=%5&napios=%6";
const QString napiApiUrl = "http://napiprojekt.pl/api/api-napiprojekt3.php";
const QString napiCheckUserUrlTpl =
    "http://www.napiprojekt.pl/users_check.php?nick=%1&pswd=%2";
const QString napiUploadUrlTpl =
    "http://www.napiprojekt.pl/unit_napisy/upload.php"
    "?m_length=%1&m_resolution=%2x%3&m_fps=%4&m_hash=%5&m_filesize=%6";
const QString napiUploadUrlSimpleTpl =
    "http://www.napiprojekt.pl/unit_napisy/upload.php"
    "?m_hash=%5&m_filesize=%6";
const QString napiReportBadUrlTpl =
    "http://www.napiprojekt.pl/unit_napisy/zlenapisyadd.php";
}  // namespace NapiProjektDownloadEngineConsts

using namespace NapiProjektDownloadEngineConsts;

SubtitleDownloadEngineMetadata NapiProjektDownloadEngine::metadata =
    SubtitleDownloadEngineMetadata(
        "NapiProjekt",
        QObject::tr("<b>www.napiprojekt.pl</b> subtitles download engine"),
        just(QUrl("http://www.napiprojekt.pl/rejestracja")), nothing());

NapiProjektDownloadEngine::NapiProjektDownloadEngine(
    const QString &tmpPath, const EngineConfig &config,
    const QSharedPointer<const P7ZipDecoder> &p7zipDecoder)
    : SubtitleDownloadEngine(tmpPath),
      engineConfig(config),
      p7zipDecoder(p7zipDecoder) {}

NapiProjektDownloadEngine::~NapiProjektDownloadEngine() { cleanup(); }

SubtitleDownloadEngineMetadata NapiProjektDownloadEngine::meta() const {
  return NapiProjektDownloadEngine::metadata;
}

const char *const *NapiProjektDownloadEngine::enginePixmapData() const {
  return NapiProjektDownloadEngine::pixmapData;
}

QString NapiProjektDownloadEngine::checksum(QString filename) {
  if (filename.isEmpty()) filename = movie;
  return (checkSum = checksum(filename, true));
}

bool NapiProjektDownloadEngine::lookForSubtitles(QString lang) {
  if (checkSum.isEmpty()) return false;

  // The API is not rate-limited, so no artificial delay is needed here.
  throttleRequests(0);

  QString tmpFile;

  const Maybe<QByteArray> subtitlesOpt = fetchSubtitlesViaApi(lang, checkSum);

  if (subtitlesOpt) {
    const QByteArray &subtitles = subtitlesOpt.value();
    if (subtitles.isEmpty()) return false;

    tmpFile = generateTmpPath();
    QFile file(tmpFile);
    if (file.exists()) file.remove();
    if (!file.open(QIODevice::WriteOnly)) return false;
    const qint64 written = file.write(subtitles);
    file.close();
    if (written != subtitles.size()) return false;
  } else {
    // the API could not be reached at all - fall back to the old endpoint
    const Maybe<QString> tmpPackedFileOpt =
        downloadByLangAndChecksum(lang, checkSum);
    if (!tmpPackedFileOpt) return false;
    tmpFile = tmpPackedFileOpt.value();
  }

  subtitlesList << SubtitleInfo(lang, meta().name(), tmpFile,
                                QFileInfo(movie).completeBaseName(), "", "txt",
                                SUBTITLE_UNKNOWN);
  return true;
}

// Returns the subtitles as plain text. An empty (but present) result means
// "this hash is not in the database"; nothing() means the request itself
// failed and the caller may fall back to the legacy endpoint.
Maybe<QByteArray> NapiProjektDownloadEngine::fetchSubtitlesViaApi(
    QString lang, QString checksum) const {
  if (checksum.isEmpty()) return nothing();

  // NapiProjekt refuses some clients on this endpoint, QNapi among them. Once
  // refused, stop asking: a rejected request still counts against the rate
  // limit that guards the legacy endpoint we fall back to.
  static bool apiUnavailable = false;
  if (apiUnavailable) return nothing();

  // Mode 17 under the official client's name is accepted, but presenting
  // QNapi as another program is the user's call, so it is opt-in.
  const bool asOfficialClient = engineConfig.officialClientApi();

  QUrlQuery params;
  params.addQueryItem("downloaded_subtitles_id", checksum);
  params.addQueryItem("mode", asOfficialClient ? "17" : "1");
  params.addQueryItem("client", asOfficialClient ? "NapiProjekt" : "QNapi");
  params.addQueryItem("client_ver", "0");
  params.addQueryItem("downloaded_subtitles_lang", npLangWrapper(lang));
  params.addQueryItem("downloaded_subtitles_txt", "1");

  QNetworkRequest request((QUrl(napiApiUrl)));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");

  SyncHTTP http;
  QNetworkReply *reply = http.syncPost(request, params.query().toUtf8());

  if (reply->error() != QNetworkReply::NoError) {
    engineErrorMsg = http.timedOut()
                         ? QObject::tr("NapiProjekt did not respond in time.")
                         : QObject::tr("Network error while contacting "
                                       "NapiProjekt: %1")
                               .arg(reply->errorString());
    return nothing();
  }

  const QByteArray response = reply->readAll();

  QDomDocument doc;
  if (!doc.setContent(response)) return nothing();

  const QDomElement root = doc.documentElement();

  // The API refuses some clients outright ("odmowa dostepu"). That is not the
  // same as "this hash has no subtitles" - report it as unavailable so the
  // caller falls back to the legacy endpoint instead of reporting a miss.
  const QDomNodeList statusNodes = root.elementsByTagName("status");
  if (!statusNodes.isEmpty() &&
      statusNodes.at(0).toElement().text().compare(
          "success", Qt::CaseInsensitive) != 0) {
    apiUnavailable = true;
    return nothing();
  }

  const QDomNodeList contentNodes = root.elementsByTagName("content");

  if (contentNodes.isEmpty()) {
    // a well-formed answer with no subtitles for this hash
    return just(QByteArray());
  }

  const QByteArray encoded = contentNodes.at(0).toElement().text().toUtf8();

  return just(QByteArray::fromBase64(encoded));
}

QList<SubtitleInfo> NapiProjektDownloadEngine::listSubtitles() {
  return subtitlesList;
}

bool NapiProjektDownloadEngine::download(QUuid id) {
  Maybe<SubtitleInfo> ms = resolveById(id);

  return ms && QFile::exists(ms.value().sourceLocation);
}

Maybe<QString> NapiProjektDownloadEngine::downloadByLangAndChecksum(
    QString lang, QString checksum) const {
  if (checksum.isEmpty()) return nothing();

  QString urlTxt = napiDownloadUrlTpl.arg(npLangWrapper(lang))
                       .arg(checksum)
                       .arg(npFDigest(checksum))
                       .arg(engineConfig.nick())
                       .arg(engineConfig.password())
                       .arg(napiOS());

  QUrl url(urlTxt);

  SyncHTTP http;
  QNetworkReply *reply = http.syncGet(QNetworkRequest(url));

  if (reply->error() != QNetworkReply::NoError) {
    // a transient network problem is not the same as "no subtitles exist" -
    // say so, otherwise a retry looks like a lottery to the user
    engineErrorMsg = http.timedOut()
                         ? QObject::tr("NapiProjekt did not respond in time.")
                         : QObject::tr("Network error while contacting "
                                       "NapiProjekt: %1")
                               .arg(reply->errorString());
    return nothing();
  }

  QByteArray buffer = reply->readAll();

  // Cloudflare answers 429 with "error code: 1015" when it rate-limits us;
  // that is not the same as "these subtitles do not exist"
  const int httpStatus =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  if (httpStatus == 429 || buffer.contains("error code: 1015")) {
    engineErrorMsg = QObject::tr(
        "NapiProjekt is rate-limiting requests; try again in a few seconds.");
    return nothing();
  }

  if (buffer.indexOf("NPc") == 0) return nothing();

  QString tmpPackedFile = generateTmpPath();

  QFile file(tmpPackedFile);
  if (file.exists()) file.remove();
  if (!file.open(QIODevice::WriteOnly)) return nothing();

  long r = file.write(buffer);
  file.close();

  if (r < 0) return nothing();

  return just(tmpPackedFile);
}

bool NapiProjektDownloadEngine::unpack(QUuid id) {
  Maybe<SubtitleInfo> ms = resolveById(id);
  if (!ms) return false;

  if (!QFile::exists(movie)) return false;
  subtitlesTmp = tmpPath + "/" + checkSum + ".txt";

  if (QFile::exists(subtitlesTmp)) QFile::remove(subtitlesTmp);

  const QString downloadedPath = ms.value().sourceLocation;

  // The API hands back plain text; the legacy endpoint hands back a
  // password-protected 7z. Tell them apart by the archive's magic bytes so
  // both paths keep working.
  QFile downloaded(downloadedPath);
  if (!downloaded.open(QIODevice::ReadOnly)) return false;
  const QByteArray magic = downloaded.read(2);
  downloaded.close();

  if (magic == QByteArray("7z")) {
    const bool unpacked = p7zipDecoder->unpackSecureArchiveFiles(
        downloadedPath, napiZipPassword, tmpPath);
    return unpacked && QFile::exists(subtitlesTmp);
  }

  return QFile::copy(downloadedPath, subtitlesTmp) &&
         QFile::exists(subtitlesTmp);
}

void NapiProjektDownloadEngine::cleanup() {
  clearSubtitlesList();
  if (QFile::exists(subtitlesTmp)) QFile::remove(subtitlesTmp);
}

bool NapiProjektDownloadEngine::checkUser(const QString &nick,
                                          const QString &pass) {
  SyncHTTP http;
  QString urlTxt = napiCheckUserUrlTpl.arg(nick).arg(pass);

  QUrl url(urlTxt);
  QNetworkReply *reply = http.syncGet(QNetworkRequest(url));
  if (reply->error() != QNetworkReply::NoError) return false;

  QString buffer = reply->readAll();
  if (buffer.indexOf("ok") == 0) return true;
  return false;
}

QString NapiProjektDownloadEngine::checksum(QString filename, bool limit10M) {
  QFile file(filename);
  QByteArray fileArray;

  if (!openMovieFile(file)) {
    checkSum.clear();
    return QString("");
  }

  fileArray = limit10M ? file.read(NAPI_10MB) : file.readAll();
  file.close();

  QByteArray b = QCryptographicHash::hash(fileArray, QCryptographicHash::Md5);
  fileArray.clear();

  QString out;
  char next[3];

  for (int i = 0; i < 16; i++) {
    snprintf(next, 3, "%.2x", static_cast<unsigned char>(b[i]));
    out += next;
  }

  checkSum = out;
  return checkSum;
}

QString NapiProjektDownloadEngine::npFDigest(const QString &input) const {
  if (input.size() != 32) return "";

  int idx[] = {0xe, 0x3, 0x6, 0x8, 0x2}, mul[] = {2, 2, 5, 4, 3},
      add[] = {0x0, 0xd, 0x10, 0xb, 0x5}, a, m, i, t, v;

  QString b;
  char tmp[2] = {0, 0};

  for (int j = 0; j <= 4; j++) {
    a = add[j];
    m = mul[j];
    i = idx[j];

    tmp[0] = input[i].toLatin1();
    t = a + static_cast<int>(strtol(tmp, NULL, 16));
    v = static_cast<int>(strtol(input.mid(t, 2).toLocal8Bit(), NULL, 16));

    snprintf(tmp, 2, "%x", (v * m) % 0x10);
    b += tmp;
  }

  return b;
}

QString NapiProjektDownloadEngine::npLangWrapper(QString lang) const {
  lang = SubtitleLanguage(lang).toTwoLetter().toUpper();

  if (lang == "EN") lang = "ENG";

  return lang;
}

QString NapiProjektDownloadEngine::napiOS() const {
#if defined(Q_OS_WIN)
  return "Windows";
#elif defined(Q_OS_MAC)
  return "Mac OS X";
#else
  return "Linux/UNIX";
#endif
}

const char *const NapiProjektDownloadEngine::pixmapData[] = {
    "16 16 5 1",        "   c #FFFFFF",     ".  c #000080",
    "+  c #0052CC",     "@  c #005CE6",     "#  c #BFD9FF",
    "           ...  ", "         ... .  ", "       ... ...  ",
    "     ... ....   ", "   ... ....     ", " ... ....       ",
    ".. ....         ", "....+           ", " ...............",
    " . . . . . . . .", " ...............", " .@@####@####@@.",
    " .@@#@@#@#@@#@@.", " .@@#@@#@####@@.", " .@@#@@#@#@@@@@.",
    " ..............."};
