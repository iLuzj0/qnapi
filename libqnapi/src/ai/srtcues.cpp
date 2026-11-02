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

#include "ai/srtcues.h"

#include <QFile>
#include <QRegularExpression>
#include <QStringList>
#include <climits>

namespace SrtCues {

namespace {

qint64 toMs(const QRegularExpressionMatch& m, int first) {
  return m.captured(first).toLongLong() * 3600000LL +
         m.captured(first + 1).toLongLong() * 60000LL +
         m.captured(first + 2).toLongLong() * 1000LL +
         m.captured(first + 3).toLongLong();
}

QString fromMs(qint64 ms) {
  if (ms < 0) ms = 0;
  return QString("%1:%2:%3,%4")
      .arg(ms / 3600000, 2, 10, QLatin1Char('0'))
      .arg((ms / 60000) % 60, 2, 10, QLatin1Char('0'))
      .arg((ms / 1000) % 60, 2, 10, QLatin1Char('0'))
      .arg(ms % 1000, 3, 10, QLatin1Char('0'));
}

}  // namespace

SrtCueList parse(const QString& content) {
  static const QRegularExpression timing(
      "^\\s*(\\d+):(\\d{2}):(\\d{2})[,.](\\d{3})\\s*-->\\s*"
      "(\\d+):(\\d{2}):(\\d{2})[,.](\\d{3})");

  SrtCueList cues;
  QStringList lines = content.split(QRegularExpression("\\r?\\n"));

  bool inCue = false;
  SrtCue cue;
  QStringList textLines;

  auto flush = [&]() {
    if (inCue) {
      // the cue number of the next cue may have been taken for text
      while (!textLines.isEmpty() && textLines.last().trimmed().isEmpty())
        textLines.removeLast();
      cue.text = textLines.join('\n').trimmed();
      if (!cue.text.isEmpty()) cues << cue;
    }
    textLines.clear();
    inCue = false;
  };

  for (int i = 0; i < lines.size(); ++i) {
    const QString& line = lines[i];
    QRegularExpressionMatch m = timing.match(line);
    if (m.hasMatch()) {
      // a number on the line before the timing is the cue index
      if (inCue && !textLines.isEmpty() &&
          QRegularExpression("^\\s*\\d+\\s*$")
              .match(textLines.last())
              .hasMatch()) {
        textLines.removeLast();
      }
      flush();
      cue = SrtCue();
      cue.startMs = toMs(m, 1);
      cue.endMs = toMs(m, 5);
      inCue = true;
    } else if (inCue) {
      if (line.trimmed().isEmpty()) {
        flush();
      } else {
        textLines << line.trimmed();
      }
    }
  }
  flush();

  return cues;
}

QString format(const SrtCueList& cues) {
  QString out;
  int n = 1;
  foreach (const SrtCue& cue, cues) {
    out += QString::number(n++) + "\n";
    out += fromMs(cue.startMs) + " --> " + fromMs(cue.endMs) + "\n";
    out += cue.text + "\n\n";
  }
  return out;
}

bool readFile(const QString& path, SrtCueList& cues) {
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly)) return false;
  cues = parse(QString::fromUtf8(f.readAll()));
  return true;
}

bool writeFile(const QString& path, const SrtCueList& cues) {
  QFile f(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
  const QByteArray data = format(cues).toUtf8();
  return f.write(data) == data.size();
}

QString wrap(const QString& text, int maxLineLength) {
  const QString t = text.simplified();
  if (t.length() <= maxLineLength) return t;

  // split at the space closest to the middle, preferring one right after a
  // punctuation mark so a sentence boundary stays on the line it belongs to
  const int middle = t.length() / 2;
  int best = -1;
  int bestScore = INT_MAX;
  for (int i = 1; i < t.length() - 1; ++i) {
    if (t[i] != QLatin1Char(' ')) continue;
    int score = qAbs(i - middle);
    if (QString(".,!?;:").contains(t[i - 1])) score -= 6;
    if (score < bestScore) {
      bestScore = score;
      best = i;
    }
  }

  if (best < 0) return t;
  return t.left(best) + "\n" + t.mid(best + 1);
}

}  // namespace SrtCues
