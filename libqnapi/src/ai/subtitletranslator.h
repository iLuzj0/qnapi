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

#ifndef SUBTITLETRANSLATOR_H
#define SUBTITLETRANSLATOR_H

#include <Either.h>

#include <QSharedPointer>
#include <QStringList>
#include <functional>

#include "ai/aierror.h"
#include "ai/llamaserver.h"
#include "ai/srtcues.h"

// Translates subtitles with the local LLM, a chunk of cues per request.
//
// The model only ever sees and returns text keyed by line number; the cue
// timings are copied from the source, so a misbehaving model can garble a
// line but never desynchronise the subtitles. Each chunk's answer is
// constrained to exactly the requested keys and then checked; a chunk that
// fails is retried, then split, and as a last resort a single line keeps its
// original text.
class SubtitleTranslator {
 public:
  static const int contextLinesBefore = 6;
  static const int contextLinesAfter = 4;
  static const int attemptsPerChunk = 3;

  SubtitleTranslator(const QSharedPointer<LlamaServer>& server,
                     const QString& sourceLanguageName,
                     const QString& targetLanguageName, int chunkSize,
                     const QString& guidelines = QString());

  // The style rules used when none are configured; the line-per-key rules
  // that keep the reply parseable are added separately and are not editable.
  static QString defaultGuidelines(const QString& sourceLanguageName,
                                   const QString& targetLanguageName);

  // title: shown to the model as context (names, register); typically the
  // movie file's name. progress is called with (cues done, cues total).
  Either<AiError, SrtCueList> translate(
      const SrtCueList& source, const QString& title,
      const std::function<void(int, int)>& progress = nullptr) const;

  // lines that ended up untranslated in the last translate() call
  int untranslatedLines() const { return untranslated; }

 private:
  // Translates source[from, to) into result[from, to). Left only on errors
  // that make continuing pointless (the server died).
  Either<AiError, bool> translateRange(const QStringList& source,
                                       QStringList& result, int from, int to,
                                       const QString& title) const;
  Either<AiError, QStringList> requestChunk(const QStringList& source,
                                            const QStringList& result, int from,
                                            int to, const QString& title,
                                            double temperature) const;
  bool looksValid(const QStringList& source,
                  const QStringList& translated) const;

  bool polishPrompt() const;
  QString systemPrompt(const QString& title) const;

  QSharedPointer<LlamaServer> server;
  QString sourceLanguageName;
  QString targetLanguageName;
  int chunkSize;
  QString guidelines;
  mutable int untranslated = 0;
};

#endif  // SUBTITLETRANSLATOR_H
