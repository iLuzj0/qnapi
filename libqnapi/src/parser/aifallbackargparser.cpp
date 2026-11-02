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

#include "aifallbackargparser.h"

AiFallbackArgParser::AiFallbackArgParser() {}

QVariant AiFallbackArgParser::parse(const QStringList& args,
                                    const QNapiConfig& config) const {
  if (args.contains("-ai") || args.contains("--ai-fallback")) {
    return QVariant::fromValue(ParsedModifier{
        config.setAiConfig(config.aiConfig().setFallbackEnabled(true))});
  } else {
    return QVariant::fromValue(NothingParsed());
  }
}

Maybe<CliArgParser::HelpInfo> AiFallbackArgParser::helpInfo() const {
  return just(HelpInfo{
      "-ai", "--ai-fallback", "",
      tr("When no subtitles in the main language are found, generate them "
         "locally: transcribe the audio (whisper.cpp) and translate it with a "
         "local language model (llama.cpp). Needs a CUDA GPU, ffmpeg and the "
         "models next to the executable")});
}
