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

#ifndef SRTCUES_H
#define SRTCUES_H

#include <QList>
#include <QMetaType>
#include <QString>

// A subtitle cue with plain text, as produced by whisper and consumed by the
// translator. Deliberately simpler than the subconvert token model: the AI
// pipeline only ever deals with untagged SubRip text.
struct SrtCue {
  qint64 startMs = 0;
  qint64 endMs = 0;
  // lines separated by '\n'
  QString text;
};

typedef QList<SrtCue> SrtCueList;

Q_DECLARE_METATYPE(SrtCueList)

namespace SrtCues {

SrtCueList parse(const QString& content);
QString format(const SrtCueList& cues);

// UTF-8 in and out
bool readFile(const QString& path, SrtCueList& cues);
bool writeFile(const QString& path, const SrtCueList& cues);

// Breaks a single-line text into at most two lines of roughly equal length
// when it is too long to read comfortably on screen.
QString wrap(const QString& text, int maxLineLength = 42);

}  // namespace SrtCues

#endif  // SRTCUES_H
