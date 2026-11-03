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

#include "ai/llamaserver.h"

#include <QDeadlineTimer>
#include <QFile>
#include <QFileInfo>
#include <QHostAddress>
#include <QJsonDocument>
#include <QTcpServer>
#include <QUrl>

#include "ai/childprocess.h"
#include "utils/synchttp.h"

LlamaServer::LlamaServer(const QString& serverPath, const QString& modelPath,
                         const QString& logPath, const QString& extraArgs)
    : server(serverPath),
      model(modelPath),
      logPath(logPath),
      extraArgs(extraArgs),
      process(nullptr) {}

LlamaServer::~LlamaServer() { stop(); }

bool LlamaServer::isAvailable() const {
  return QFileInfo(server).isExecutable() && QFileInfo(model).isFile();
}

int LlamaServer::findFreePort() {
  // let the OS pick; the tiny window before llama-server binds it is harmless
  // for a local, one-off server
  QTcpServer probe;
  if (!probe.listen(QHostAddress::LocalHost, 0)) return 0;
  const int port = probe.serverPort();
  probe.close();
  return port;
}

QString LlamaServer::logTail() const {
  QFile f(logPath);
  if (!f.open(QIODevice::ReadOnly)) return QString();
  if (f.size() > 8192) f.seek(f.size() - 8192);
  return ChildProcess::tail(f.readAll(), 4);
}

Either<AiError, QString> LlamaServer::start(const AiCancelCheck& cancelled) {
  if (isRunning()) return baseUrl;
  stop();

  const int port = findFreePort();
  if (port == 0) {
    return AiError{QObject::tr("No free local port for llama-server")};
  }

  QStringList args = {
      "-m", model, "--host", "127.0.0.1", "--port", QString::number(port),
      // everything on the GPU; a model larger than its memory needs a lower
      // -ngl in the extra arguments
      "-ngl", "999", "-c", QString::number(contextSize), "-np", "1", "-fa",
      "auto",
      // The built-in templates, picked by matching the model's own template.
      // With the Jinja engine llama.cpp folds the template's generation
      // prompt into the JSON grammar, which Bielik's tokenizer cannot match,
      // so every schema-constrained request fails.
      "--no-jinja", "--no-webui"};
  // llama.cpp takes the last occurrence of an option, so these override the
  // defaults above; "--jinja" re-enables the template engine for models whose
  // template llama.cpp does not recognise
  args << QProcess::splitCommand(extraArgs);

  process = new QProcess();
  process->setProcessChannelMode(QProcess::MergedChannels);
  process->setStandardOutputFile(logPath, QIODevice::Truncate);
  process->start(server, args);

  if (!process->waitForStarted()) {
    delete process;
    process = nullptr;
    return AiError{QObject::tr("Could not start llama-server: %1").arg(server)};
  }
  ChildProcess::bindToParentLifetime(*process);

  baseUrl = QString("http://127.0.0.1:%1").arg(port);

  // /health answers 503 while the model is loading and 200 once it is ready
  QDeadlineTimer deadline(startTimeoutMsecs);
  while (!deadline.hasExpired()) {
    if (cancelled && cancelled()) {
      stop();
      return AiError::cancellation();
    }
    if (process->waitForFinished(500)) {
      const QString details = logTail();
      delete process;
      process = nullptr;
      return AiError{QObject::tr("llama-server exited while loading the "
                                 "model (%1)")
                         .arg(details)};
    }

    SyncHTTP http(2000);
    QNetworkReply* reply =
        http.syncGet(QNetworkRequest(QUrl(baseUrl + "/health")));
    const int status =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->deleteLater();

    if (status == 200) return baseUrl;
  }

  stop();
  return AiError{QObject::tr("llama-server did not become ready in time")};
}

void LlamaServer::stop() {
  if (process == nullptr) return;

  if (process->state() != QProcess::NotRunning) {
    process->kill();
    process->waitForFinished(10000);
  }
  delete process;
  process = nullptr;
}

bool LlamaServer::isRunning() const {
  return process != nullptr && process->state() == QProcess::Running;
}

Either<AiError, QString> LlamaServer::chat(const QJsonArray& messages,
                                           const QJsonObject& extra) const {
  if (!isRunning()) {
    return AiError{
        QObject::tr("llama-server is not running (%1)").arg(logTail())};
  }

  QJsonObject body = extra;
  body.insert("messages", messages);

  QNetworkRequest req(QUrl(baseUrl + "/v1/chat/completions"));
  req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  SyncHTTP http(requestTimeoutMsecs);
  QNetworkReply* reply =
      http.syncPost(req, QJsonDocument(body).toJson(QJsonDocument::Compact));

  const int status =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  const QByteArray data = reply->readAll();
  const QString networkError = reply->errorString();
  reply->deleteLater();

  if (http.timedOut()) {
    return AiError{QObject::tr("llama-server did not answer in time")};
  }
  if (status != 200) {
    return AiError{QObject::tr("llama-server request failed (%1 %2)")
                       .arg(status)
                       .arg(status == 0 ? networkError
                                        : QString::fromUtf8(data.left(300)))};
  }

  const QJsonObject response = QJsonDocument::fromJson(data).object();
  const QJsonArray choices = response.value("choices").toArray();
  if (choices.isEmpty()) {
    return AiError{QObject::tr("llama-server returned no answer")};
  }

  return choices.first()
      .toObject()
      .value("message")
      .toObject()
      .value("content")
      .toString();
}
