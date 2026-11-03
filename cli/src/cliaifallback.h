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

// Console front end of the --ai-fallback step, run after the download rounds
// for the files that still have no subtitles in the main language; the work
// itself is done by AiSubtitleGenerator.
namespace CliAiFallback {

Q_DECLARE_NAMESPACE_TR(CliAiFallback)

// Returns the number of files that got subtitles.
int generateSubtitles(const Console& c, const QStringList& movieFilePaths,
                      const QNapiConfig& config);

}  // namespace CliAiFallback

#endif  // CLIAIFALLBACK_H
