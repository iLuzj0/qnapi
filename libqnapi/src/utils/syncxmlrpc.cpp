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

#include "syncxmlrpc.h"

#include <QTimer>

SyncXmlRpc::SyncXmlRpc(const QUrl &endPoint, int timeoutMsecs)
    : rpc(endPoint), timeoutMsecs(timeoutMsecs) {}

bool SyncXmlRpc::request(const QString &method, const QVariantList &args) {
  result = false;
  timeout = false;
  resp = QVariant();

  rpc.call(method, args, this, SLOT(whenResponse(QVariant &)), this,
           SLOT(whenFault(int, const QString &)));

  // without a deadline an unanswered call blocks the search indefinitely
  QTimer timer;
  timer.setSingleShot(true);
  connect(&timer, &QTimer::timeout, this, [this]() {
    timeout = true;
    result = false;
    loop.exit();
  });

  if (timeoutMsecs > 0) timer.start(timeoutMsecs);

  loop.exec();

  return result;
}

QVariant SyncXmlRpc::getResponse() const { return resp; }

QVariant SyncXmlRpc::getError() const {
  return QString::number(error) + " " + message;
}

void SyncXmlRpc::whenResponse(QVariant &response) {
  result = true;
  resp = response;
  loop.exit();
}

void SyncXmlRpc::whenFault(int err, const QString &msg) {
  result = false;
  error = err;
  message = msg;
  loop.exit();
}
