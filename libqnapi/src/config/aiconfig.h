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
  bool fallbackEnabled_ = false;
  QString ffmpegPath_;
  QString whisperPath_;
  QString whisperModel_;
  QString llamaServerPath_;
  QString llmModel_;
  QString sourceLanguage_ = "en";
  int chunkSize_ = defaultChunkSize;
  QString translationGuidelines_;
  QString llamaServerArgs_;

 public:
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
  // Style rules for the translation, one per line, with {source} and
  // {target} standing for the language names. Empty selects the built-in
  // rules, see SubtitleTranslator::defaultGuidelines().
  QString translationGuidelines() const { return translationGuidelines_; }
  // appended to llama-server's command line, so they override the defaults
  // (e.g. "-ngl 20" for a model larger than the GPU's memory, or "--jinja")
  QString llamaServerArgs() const { return llamaServerArgs_; }

  const AiConfig setFallbackEnabled(const bool& fallbackEnabled) const {
    AiConfig c = *this;
    c.fallbackEnabled_ = fallbackEnabled;
    return c;
  }
  const AiConfig setFfmpegPath(const QString& ffmpegPath) const {
    AiConfig c = *this;
    c.ffmpegPath_ = ffmpegPath;
    return c;
  }
  const AiConfig setWhisperPath(const QString& whisperPath) const {
    AiConfig c = *this;
    c.whisperPath_ = whisperPath;
    return c;
  }
  const AiConfig setWhisperModel(const QString& whisperModel) const {
    AiConfig c = *this;
    c.whisperModel_ = whisperModel;
    return c;
  }
  const AiConfig setLlamaServerPath(const QString& llamaServerPath) const {
    AiConfig c = *this;
    c.llamaServerPath_ = llamaServerPath;
    return c;
  }
  const AiConfig setLlmModel(const QString& llmModel) const {
    AiConfig c = *this;
    c.llmModel_ = llmModel;
    return c;
  }
  const AiConfig setSourceLanguage(const QString& sourceLanguage) const {
    AiConfig c = *this;
    c.sourceLanguage_ = sourceLanguage;
    return c;
  }
  const AiConfig setChunkSize(const int& chunkSize) const {
    AiConfig c = *this;
    c.chunkSize_ = chunkSize;
    return c;
  }
  const AiConfig setTranslationGuidelines(
      const QString& translationGuidelines) const {
    AiConfig c = *this;
    c.translationGuidelines_ = translationGuidelines;
    return c;
  }
  const AiConfig setLlamaServerArgs(const QString& llamaServerArgs) const {
    AiConfig c = *this;
    c.llamaServerArgs_ = llamaServerArgs;
    return c;
  }

  QString toString() const;
};

#endif  // AICONFIG_H
