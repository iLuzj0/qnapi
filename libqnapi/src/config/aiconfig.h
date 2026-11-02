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

#ifndef AICONFIG_H
#define AICONFIG_H

#include <QString>

// Settings for the AI fallback: when no service has subtitles in the main
// language, the audio is transcribed locally (whisper.cpp) and the
// transcription translated by a local LLM (llama.cpp server).
//
// Empty paths mean "the default file next to the executable"; relative paths
// are relative to the executable's directory, see LibQNapi::resolveAiPath().
class AiConfig {
 public:
  static const int defaultChunkSize = 35;
  static const int minChunkSize = 5;
  static const int maxChunkSize = 100;

  static QString defaultFfmpegPath();
  static QString defaultWhisperPath();
  static QString defaultWhisperModel();
  static QString defaultLlamaServerPath();
  static QString defaultLlmModel();

 private:
  bool fallbackEnabled_;
  QString ffmpegPath_;
  QString whisperPath_;
  QString whisperModel_;
  QString llamaServerPath_;
  QString llmModel_;
  QString sourceLanguage_;
  int chunkSize_;

 public:
  AiConfig()
      : fallbackEnabled_(false),
        sourceLanguage_("en"),
        chunkSize_(defaultChunkSize) {}
  AiConfig(const bool& fallbackEnabled, const QString& ffmpegPath,
           const QString& whisperPath, const QString& whisperModel,
           const QString& llamaServerPath, const QString& llmModel,
           const QString& sourceLanguage, const int& chunkSize)
      : fallbackEnabled_(fallbackEnabled),
        ffmpegPath_(ffmpegPath),
        whisperPath_(whisperPath),
        whisperModel_(whisperModel),
        llamaServerPath_(llamaServerPath),
        llmModel_(llmModel),
        sourceLanguage_(sourceLanguage),
        chunkSize_(chunkSize) {}

  bool fallbackEnabled() const { return fallbackEnabled_; }
  QString ffmpegPath() const { return ffmpegPath_; }
  QString whisperPath() const { return whisperPath_; }
  QString whisperModel() const { return whisperModel_; }
  QString llamaServerPath() const { return llamaServerPath_; }
  QString llmModel() const { return llmModel_; }
  // language spoken in the movie, passed to whisper ("auto" to detect)
  QString sourceLanguage() const {
    return sourceLanguage_.isEmpty() ? QString("en") : sourceLanguage_;
  }
  // number of subtitle lines sent to the LLM in one request
  int chunkSize() const {
    return (chunkSize_ >= minChunkSize && chunkSize_ <= maxChunkSize)
               ? chunkSize_
               : defaultChunkSize;
  }

  const AiConfig setFallbackEnabled(const bool& fallbackEnabled) const {
    return AiConfig(fallbackEnabled, ffmpegPath_, whisperPath_, whisperModel_,
                    llamaServerPath_, llmModel_, sourceLanguage_, chunkSize_);
  }
  const AiConfig setFfmpegPath(const QString& ffmpegPath) const {
    return AiConfig(fallbackEnabled_, ffmpegPath, whisperPath_, whisperModel_,
                    llamaServerPath_, llmModel_, sourceLanguage_, chunkSize_);
  }
  const AiConfig setWhisperPath(const QString& whisperPath) const {
    return AiConfig(fallbackEnabled_, ffmpegPath_, whisperPath, whisperModel_,
                    llamaServerPath_, llmModel_, sourceLanguage_, chunkSize_);
  }
  const AiConfig setWhisperModel(const QString& whisperModel) const {
    return AiConfig(fallbackEnabled_, ffmpegPath_, whisperPath_, whisperModel,
                    llamaServerPath_, llmModel_, sourceLanguage_, chunkSize_);
  }
  const AiConfig setLlamaServerPath(const QString& llamaServerPath) const {
    return AiConfig(fallbackEnabled_, ffmpegPath_, whisperPath_, whisperModel_,
                    llamaServerPath, llmModel_, sourceLanguage_, chunkSize_);
  }
  const AiConfig setLlmModel(const QString& llmModel) const {
    return AiConfig(fallbackEnabled_, ffmpegPath_, whisperPath_, whisperModel_,
                    llamaServerPath_, llmModel, sourceLanguage_, chunkSize_);
  }
  const AiConfig setSourceLanguage(const QString& sourceLanguage) const {
    return AiConfig(fallbackEnabled_, ffmpegPath_, whisperPath_, whisperModel_,
                    llamaServerPath_, llmModel_, sourceLanguage, chunkSize_);
  }
  const AiConfig setChunkSize(const int& chunkSize) const {
    return AiConfig(fallbackEnabled_, ffmpegPath_, whisperPath_, whisperModel_,
                    llamaServerPath_, llmModel_, sourceLanguage_, chunkSize);
  }

  QString toString() const;
};

#endif  // AICONFIG_H
