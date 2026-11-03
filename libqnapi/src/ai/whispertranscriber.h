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

#ifndef WHISPERTRANSCRIBER_H
#define WHISPERTRANSCRIBER_H

#include <Either.h>

#include <QString>
#include <functional>

#include "ai/aierror.h"
#include "ai/srtcues.h"

// Runs whisper.cpp's whisper-cli on a WAV file and returns the timed
// transcription. The cue timings come from the audio itself, so they match
// this exact file - which is what downloaded subtitles often get wrong.
class WhisperTranscriber {
 public:
  // a feature-length film on a slow GPU, with room to spare
  static const int timeoutMsecs = 3 * 60 * 60 * 1000;

  WhisperTranscriber(const QString& whisperPath, const QString& modelPath);

  QString whisperPath() const { return whisper; }
  QString modelPath() const { return model; }
  bool isAvailable() const;

  // language: whisper's language code ("en"), or "auto" to detect it.
  // progress is called with 0..100 as whisper reports it.
  Either<AiError, SrtCueList> transcribe(
      const QString& wavPath, const QString& language,
      const std::function<void(int)>& progress = nullptr,
      const AiCancelCheck& cancelled = nullptr) const;

 private:
  QString whisper;
  QString model;
};

#endif  // WHISPERTRANSCRIBER_H
