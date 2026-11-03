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

#ifndef LLAMASERVER_H
#define LLAMASERVER_H

#include <Either.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QProcess>
#include <QString>

#include "ai/aierror.h"

// A private llama.cpp server on localhost. Started once for a whole batch so
// the model is loaded into VRAM once, not once per file, and stopped (or
// killed with QNapi) afterwards.
class LlamaServer {
 public:
  // loading ~7 GB from a hard disk can take a while
  static const int startTimeoutMsecs = 5 * 60 * 1000;
  static const int requestTimeoutMsecs = 5 * 60 * 1000;
  static const int contextSize = 8192;

  // extraArgs: appended to the command line, so they override the defaults
  LlamaServer(const QString& serverPath, const QString& modelPath,
              const QString& logPath, const QString& extraArgs = QString());
  ~LlamaServer();

  QString serverPath() const { return server; }
  QString modelPath() const { return model; }
  bool isAvailable() const;

  // Starts the server and waits until the model is loaded.
  Either<AiError, QString> start();
  void stop();
  bool isRunning() const;

  // POSTs an OpenAI-style chat completion and returns the reply's text.
  // `extra` is merged into the request (temperature, response_format, ...).
  Either<AiError, QString> chat(const QJsonArray& messages,
                                const QJsonObject& extra) const;

 private:
  static int findFreePort();
  QString logTail() const;

  QString server;
  QString model;
  QString logPath;
  QString extraArgs;
  QString baseUrl;
  QProcess* process;
};

#endif  // LLAMASERVER_H
