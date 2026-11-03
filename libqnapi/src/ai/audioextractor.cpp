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

#include "ai/audioextractor.h"

#include <QFile>
#include <QFileInfo>
#include <QProcess>

#include "ai/childprocess.h"

AudioExtractor::AudioExtractor(const QString& ffmpegPath)
    : ffmpeg(ffmpegPath) {}

bool AudioExtractor::isAvailable() const {
  return QFileInfo(ffmpeg).isExecutable();
}

Either<AiError, QString> AudioExtractor::extract(
    const QString& moviePath, const QString& wavPath,
    const QString& languageCode, const AiCancelCheck& cancelled) const {
  if (!languageCode.isEmpty()) {
    // fails when no track carries that tag, which is common: many files have
    // a single untagged track
    auto tagged =
        run(moviePath, wavPath, "0:a:m:language:" + languageCode, cancelled);
    if (tagged.is2nd() || tagged.as1st().cancelled) return tagged;
  }

  return run(moviePath, wavPath, "0:a:0", cancelled);
}

Either<AiError, QString> AudioExtractor::run(
    const QString& moviePath, const QString& wavPath, const QString& streamSpec,
    const AiCancelCheck& cancelled) const {
  QFile::remove(wavPath);

  const QStringList args = {
      "-nostdin", "-hide_banner", "-loglevel", "error",    "-y",
      "-i",       moviePath,      "-map",      streamSpec, "-vn",
      "-sn",      "-dn",          "-ac",       "1",        "-ar",
      "16000",    "-c:a",         "pcm_s16le", wavPath};

  QProcess p;
  p.setProcessChannelMode(QProcess::MergedChannels);
  p.start(ffmpeg, args);
  if (!p.waitForStarted()) {
    return AiError{QObject::tr("Could not start ffmpeg: %1").arg(ffmpeg)};
  }
  ChildProcess::bindToParentLifetime(p);

  const ChildProcess::WaitResult waited =
      ChildProcess::waitForFinished(p, timeoutMsecs, cancelled);
  if (waited != ChildProcess::WaitResult::Finished) {
    QFile::remove(wavPath);
    if (waited == ChildProcess::WaitResult::Cancelled)
      return AiError::cancellation();
    return AiError{QObject::tr("ffmpeg timed out extracting audio")};
  }

  if (p.exitStatus() != QProcess::NormalExit || p.exitCode() != 0 ||
      QFileInfo(wavPath).size() == 0) {
    QFile::remove(wavPath);
    QString details = ChildProcess::tail(p.readAll(), 3);
    return AiError{
        QObject::tr("ffmpeg could not extract audio (%1)")
            .arg(details.isEmpty() ? QString::number(p.exitCode()) : details)};
  }

  return wavPath;
}
