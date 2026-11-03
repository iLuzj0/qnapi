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

#ifndef AUDIOEXTRACTOR_H
#define AUDIOEXTRACTOR_H

#include <Either.h>

#include <QString>

#include "ai/aierror.h"

// Extracts a movie's soundtrack with ffmpeg into the format whisper reads.
class AudioExtractor {
 public:
  // 30 minutes covers reading a full movie over a slow network share
  static const int timeoutMsecs = 30 * 60 * 1000;

  explicit AudioExtractor(const QString& ffmpegPath);

  QString ffmpegPath() const { return ffmpeg; }
  bool isAvailable() const;

  // Writes 16 kHz mono 16-bit WAV to wavPath and returns it. When the movie
  // has several audio tracks, the one tagged with languageCode (ISO 639-2,
  // e.g. "eng") is preferred over the first one.
  Either<AiError, QString> extract(
      const QString& moviePath, const QString& wavPath,
      const QString& languageCode,
      const AiCancelCheck& cancelled = nullptr) const;

 private:
  Either<AiError, QString> run(const QString& moviePath, const QString& wavPath,
                               const QString& streamSpec,
                               const AiCancelCheck& cancelled) const;

  QString ffmpeg;
};

#endif  // AUDIOEXTRACTOR_H
