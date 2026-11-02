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

#ifndef CLIAIFALLBACK_H
#define CLIAIFALLBACK_H

#include <QStringList>

#include "config/qnapiconfig.h"
#include "tr.h"
#include "utils/console.h"

// The --ai-fallback step, run after the download rounds for the files that
// still have no subtitles in the main language:
//
//   1. extract each file's audio (ffmpeg) and transcribe it (whisper.cpp);
//   2. start llama-server once and translate all transcriptions;
//   3. post-process and save them like downloaded subtitles.
//
// Transcription and translation run as separate phases so that whisper and
// the LLM never compete for VRAM, and the LLM is loaded only once.
namespace CliAiFallback {

Q_DECLARE_NAMESPACE_TR(CliAiFallback)

// Returns the number of files that got subtitles.
int generateSubtitles(const Console& c, const QStringList& movieFilePaths,
                      const QNapiConfig& config);

// True when a subtitle file in the given language already sits next to the
// movie (Movie.pol.srt, Movie.pl.srt, ...); the AI step leaves those alone.
// With unlabelledCounts, a plain Movie.srt counts too: without a language
// suffix there is no telling what language it is, and the AI output would
// overwrite it.
bool hasSubtitlesInLanguage(const QString& movieFilePath,
                            const QString& language,
                            const QStringList& subtitleExtensions,
                            bool unlabelledCounts);

}  // namespace CliAiFallback

#endif  // CLIAIFALLBACK_H
