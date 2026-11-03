/*****************************************************************************
**  Copyright (C) 2005 by Iulian M
**  eti@erata.net
**
**  modifications by Piotr Krzemiński
**  Copyright (C) 2008-2017
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

#include "synchttp.h"

#include <QTimer>

SyncHTTP::SyncHTTP(int timeoutMsecs)
    : manager(this), timeoutMsecs(timeoutMsecs), timeout(false) {}

QNetworkReply* SyncHTTP::awaitReply(QNetworkReply* reply) {
  timeout = false;

  // QNetworkReply::finished() is emitted for failures too (including abort),
  // so it alone is enough to leave the loop. Note that the old
  // QNetworkReply::error() signal no longer exists in Qt6 - it was renamed to
  // errorOccurred(), and connecting to the old name silently did nothing.
  connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

  QTimer timer;
  timer.setSingleShot(true);
  connect(&timer, &QTimer::timeout, this, [this, reply]() {
    timeout = true;
    reply->abort();
  });

  if (timeoutMsecs > 0) timer.start(timeoutMsecs);

  loop.exec();

  return reply;
}

QNetworkReply* SyncHTTP::syncGet(const QNetworkRequest& req) {
  return awaitReply(manager.get(req));
}

QNetworkReply* SyncHTTP::syncPost(const QNetworkRequest& req,
                                  const QByteArray& data) {
  return awaitReply(manager.post(req, data));
}
