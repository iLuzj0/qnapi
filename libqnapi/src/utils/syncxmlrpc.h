/*****************************************************************************
** QNapi
** Copyright (C) 2008-2017 Piotr Krzemiński <pio.krzeminski@gmail.com>
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

#ifndef SYNCXMLRPC_H
#define SYNCXMLRPC_H

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QObject>
#include "maiaXmlRpcClient.h"

class SyncXmlRpc : public QObject {
  Q_OBJECT
 public:
  static const int defaultTimeoutMsecs = 20000;

  explicit SyncXmlRpc(const QUrl& endPoint,
                      int timeoutMsecs = defaultTimeoutMsecs);

  bool request(const QString& method, const QVariantList& args);
  QVariant getResponse() const;
  QVariant getError() const;

  // true when the last request was given up on because it ran out of time
  bool timedOut() const { return timeout; }

 private slots:

  void whenResponse(QVariant& response);

  void whenFault(int err, const QString& msg);

 private:
  bool result = false;
  int error = 0;
  QString message;
  QVariant resp;
  QEventLoop loop;
  MaiaXmlRpcClient rpc;
  int timeoutMsecs;
  bool timeout = false;
};

#endif
