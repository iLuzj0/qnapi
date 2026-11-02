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

#ifndef CHILDPROCESS_H
#define CHILDPROCESS_H

#include <QProcess>
#include <QString>
#include <QStringList>

namespace ChildProcess {

// Makes a started process die together with QNapi. Without it an interrupted
// run (Ctrl+C ends qnapic through exit()) would leave llama-server holding
// several GB of VRAM. Windows only; elsewhere this does nothing.
void bindToParentLifetime(QProcess& process);

// Last lines of a process's merged output, for error messages.
QString tail(const QByteArray& output, int lines = 5);

// Quotes the arguments for display in error messages.
QString commandLine(const QString& program, const QStringList& args);

}  // namespace ChildProcess

#endif  // CHILDPROCESS_H
