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

#include "ai/childprocess.h"

#include <QDeadlineTimer>
#include <QRegularExpression>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace ChildProcess {

#ifdef Q_OS_WIN
namespace {

// One job for all children; it is closed by the OS when QNapi exits, however
// it exits, and KILL_ON_JOB_CLOSE then terminates everything still in it.
HANDLE killOnCloseJob() {
  static HANDLE job = []() -> HANDLE {
    HANDLE j = CreateJobObjectW(nullptr, nullptr);
    if (j == nullptr) return nullptr;

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    ZeroMemory(&info, sizeof(info));
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(j, JobObjectExtendedLimitInformation, &info,
                                 sizeof(info))) {
      CloseHandle(j);
      return nullptr;
    }
    return j;
  }();
  return job;
}

}  // namespace
#endif

void bindToParentLifetime(QProcess& process) {
#ifdef Q_OS_WIN
  HANDLE job = killOnCloseJob();
  if (job == nullptr || process.processId() == 0) return;

  HANDLE child = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE,
                             static_cast<DWORD>(process.processId()));
  if (child == nullptr) return;

  AssignProcessToJobObject(job, child);
  CloseHandle(child);
#else
  Q_UNUSED(process);
#endif
}

WaitResult waitForFinished(QProcess& process, int timeoutMsecs,
                           const AiCancelCheck& cancelled) {
  QDeadlineTimer deadline(timeoutMsecs);
  while (!process.waitForFinished(250)) {
    if (process.state() == QProcess::NotRunning) break;

    const bool stop = cancelled && cancelled();
    if (stop || deadline.hasExpired()) {
      process.kill();
      process.waitForFinished();
      return stop ? WaitResult::Cancelled : WaitResult::TimedOut;
    }
  }
  return WaitResult::Finished;
}

QString tail(const QByteArray& output, int lines) {
  QStringList all = QString::fromLocal8Bit(output).split(
      QRegularExpression("[\\r\\n]+"), Qt::SkipEmptyParts);
  return all.mid(qMax(0, all.size() - lines)).join("\n");
}

QString commandLine(const QString& program, const QStringList& args) {
  QStringList parts;
  parts << program;
  foreach (const QString& a, args) {
    parts << (a.contains(' ') ? "\"" + a + "\"" : a);
  }
  return parts.join(' ');
}

}  // namespace ChildProcess
