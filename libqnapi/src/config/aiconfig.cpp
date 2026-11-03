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

#include "config/aiconfig.h"

#include <QTextStream>

#ifdef Q_OS_WIN
#define QNAPI_EXE_SUFFIX ".exe"
#else
#define QNAPI_EXE_SUFFIX ""
#endif

QString AiConfig::defaultFfmpegPath() {
  return QStringLiteral("ffmpeg" QNAPI_EXE_SUFFIX);
}

QString AiConfig::defaultWhisperPath() {
  return QStringLiteral("whisper-cli" QNAPI_EXE_SUFFIX);
}

QString AiConfig::defaultWhisperModel() {
  return QStringLiteral("models/ggml-large-v3-turbo.bin");
}

QString AiConfig::defaultLlamaServerPath() {
  return QStringLiteral("llama-server" QNAPI_EXE_SUFFIX);
}

QString AiConfig::defaultLlmModel() {
  return QStringLiteral("models/Bielik-11B-v3.0-Instruct.Q4_K_M.gguf");
}

QString AiConfig::toString() const {
  QString s;
  QTextStream(&s) << "fallbackEnabled: " << fallbackEnabled() << Qt::endl
                  << "ffmpegPath: " << ffmpegPath() << Qt::endl
                  << "whisperPath: " << whisperPath() << Qt::endl
                  << "whisperModel: " << whisperModel() << Qt::endl
                  << "llamaServerPath: " << llamaServerPath() << Qt::endl
                  << "llmModel: " << llmModel() << Qt::endl
                  << "sourceLanguage: " << sourceLanguage() << Qt::endl
                  << "chunkSize: " << chunkSize() << Qt::endl
                  << "translationGuidelines: " << translationGuidelines()
                  << Qt::endl
                  << "llamaServerArgs: " << llamaServerArgs() << Qt::endl;
  return s;
}
