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

#ifndef AIERROR_H
#define AIERROR_H

#include <QMetaType>
#include <QObject>
#include <QString>
#include <functional>

// Left side of the Either<AiError, T> results returned by the AI helpers.
// Either keeps its value in a QVariant keyed by type, so the error needs a
// type of its own rather than a bare QString.
struct AiError {
  QString message;
  // stopped on request rather than failed
  bool cancelled = false;

  static AiError cancellation() {
    AiError e;
    e.message = QObject::tr("cancelled");
    e.cancelled = true;
    return e;
  }
};

// Polled by long-running AI steps; returning true stops the step and kills
// the child process it waits for.
typedef std::function<bool()> AiCancelCheck;

Q_DECLARE_METATYPE(AiError)

#endif  // AIERROR_H
