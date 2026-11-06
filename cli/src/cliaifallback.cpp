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

#include "cliaifallback.h"

#include <QDir>
#include <QFileInfo>

#include "libqnapi.h"

namespace CliAiFallback {

namespace {

QString formatDuration(qint64 msecs) {
  const qint64 secs = msecs / 1000;
  if (secs < 60) return QString("%1 s").arg(secs);
  return QString("%1 min %2 s").arg(secs / 60).arg(secs % 60);
}

// Progress is redrawn in place with \r; every finished step gets a line.
class ConsoleListener : public AiGenerationListener {
 public:
  ConsoleListener(const Console& c, const QStringList& movieFilePaths)
      : c(c), paths(movieFilePaths) {}

  void started(int fileCount, const QString& targetLanguageName) override {
    c.printLine();
    c.printLineHighlihted(
        tr("AI fallback: generating %1 subtitles for %n file(s)", "", fileCount)
            .arg(targetLanguageName));
  }

  void toolsMissing(const QStringList& missing) override {
    c.printLineError(tr("AI fallback is unavailable, missing:"));
    foreach (const QString& path, missing) {
      c.printLineError("   " + QDir::toNativeSeparators(path));
    }
    c.printLineError(
        tr("Build the tools with scripts/build-ai-tools.bat and download the "
           "models with scripts/download-ai-models.ps1"));
  }

  void transcriptionStarted(int index) override {
    lastPercent = -1;
    c.printOrdinary(transcribingLabel(index) + "...\r");
  }

  void transcriptionProgress(int index, int percent) override {
    if (percent / 5 == lastPercent / 5) return;
    lastPercent = percent;
    c.printOrdinary(
        QString("%1... %2%\r").arg(transcribingLabel(index)).arg(percent));
  }

  void transcriptionFinished(int index, int lines, const QString& error,
                             qint64 msecs) override {
    c.printLine();
    if (lines == 0) {
      c.printLineWarning(prefix(index) + name(index) + " - " + error);
      return;
    }
    c.printLineOrdinary(prefix(index) +
                        tr("%n line(s) transcribed in %1", "", lines)
                            .arg(formatDuration(msecs)));
  }

  void nothingTranscribed() override {
    c.printLineWarning(tr("AI fallback: nothing was transcribed"));
  }

  void modelLoading() override {
    c.printLineOrdinary(tr("Loading the translation model..."));
  }

  void modelLoaded(qint64 msecs) override {
    c.printLineOrdinary(tr("Model loaded in %1").arg(formatDuration(msecs)));
  }

  void modelFailed(const QString& error, const QString& logPath) override {
    c.printLineError(tr("AI fallback: %1").arg(error));
    c.printLineError(
        tr("Server log: %1").arg(QDir::toNativeSeparators(logPath)));
  }

  void modelRestarting(int index, const QString& error) override {
    c.printLine();
    c.printLineWarning(prefix(index) +
                       tr("llama-server stopped (%1)").arg(error));
    c.printLineOrdinary(tr("Reloading the translation model and translating "
                           "%1 again...")
                            .arg(name(index)));
  }

  void translationProgress(int index, int done, int count) override {
    c.printOrdinary(
        QString("%1... %2/%3\r")
            .arg(prefix(index) + tr("Translating %1").arg(name(index)))
            .arg(done)
            .arg(count));
  }

  void translationFinished(int index, const AiFileResult& r) override {
    c.printLine();
    if (!r.saved) {
      c.printLineError(prefix(index) + name(index) + " - " + r.error);
      return;
    }

    QString summary =
        prefix(index) + name(index) + " - " +
        tr("AI subtitles saved (%1)").arg(formatDuration(r.msecs));
    if (r.untranslatedLines > 0) {
      summary +=
          ", " + tr("%n line(s) left untranslated", "", r.untranslatedLines);
    }
    c.printLineHighlihted(summary);
  }

 private:
  QString name(int index) const { return QFileInfo(paths[index]).fileName(); }

  QString prefix(int index) const {
    return QString("[AI %1/%2] ").arg(index + 1).arg(paths.size());
  }

  QString transcribingLabel(int index) const {
    return prefix(index) + tr("Transcribing %1").arg(name(index));
  }

  const Console& c;
  const QStringList paths;
  int lastPercent = -1;
};

}  // namespace

int generateSubtitles(const Console& c, const QStringList& movieFilePaths,
                      const QNapiConfig& config) {
  ConsoleListener listener(c, movieFilePaths);
  return LibQNapi::aiSubtitleGenerator(config)->generate(movieFilePaths,
                                                         listener);
}

}  // namespace CliAiFallback
