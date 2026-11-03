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

#ifndef SYNCHTTP_H
#define SYNCHTTP_H

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class SyncHTTP : public QObject {
  Q_OBJECT
 public:
  // a request that never completes used to block the whole search forever
  static const int defaultTimeoutMsecs = 20000;

  explicit SyncHTTP(int timeoutMsecs = defaultTimeoutMsecs);

  QNetworkReply* syncGet(const QNetworkRequest& req);
  QNetworkReply* syncPost(const QNetworkRequest& req, const QByteArray& data);

  // true when the last request was aborted because it ran out of time
  bool timedOut() const { return timeout; }

 private:
  QNetworkReply* awaitReply(QNetworkReply* reply);

  QEventLoop loop;
  QNetworkAccessManager manager;
  int timeoutMsecs;
  bool timeout;
};

#endif
