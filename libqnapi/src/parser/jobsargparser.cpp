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

#include "jobsargparser.h"

JobsArgParser::JobsArgParser() {}

QVariant JobsArgParser::parse(const QStringList& args,
                              const QNapiConfig& config) const {
  int idx = args.indexOf("-j");
  if (idx == -1) {
    idx = args.indexOf("--jobs");
  }
  if (idx == -1) {
    return QVariant::fromValue(NothingParsed());
  }

  if (idx == args.size() - 1) {
    return QVariant::fromValue(ParseError{
        tr("You need to pass the number of parallel downloads after %1 "
           "argument")
            .arg(args[idx])});
  }

  bool valid = false;
  const int jobs = args[idx + 1].toInt(&valid);

  if (!valid || jobs < 1 || jobs > GeneralConfig::maxJobs) {
    return QVariant::fromValue(ParseError{
        tr("Invalid number of parallel downloads passed: %1 (expected 1 to %2)")
            .arg(args[idx + 1])
            .arg(GeneralConfig::maxJobs)});
  }

  return QVariant::fromValue(ParsedModifier{
      config.setGeneralConfig(config.generalConfig().setJobs(jobs))});
}

Maybe<CliArgParser::HelpInfo> JobsArgParser::helpInfo() const {
  return just(HelpInfo{
      "-j", "--jobs", tr("count"),
      tr("Number of files to process in parallel (1 to %1, default %2). "
         "Subtitle services throttle requests that arrive one after another, "
         "so processing several files at once is dramatically faster")
          .arg(GeneralConfig::maxJobs)
          .arg(GeneralConfig::defaultJobs)});
}
