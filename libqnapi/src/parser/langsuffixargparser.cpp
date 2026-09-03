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

#include "langsuffixargparser.h"

LangSuffixArgParser::LangSuffixArgParser() {}

QVariant LangSuffixArgParser::parse(const QStringList& args,
                                    const QNapiConfig& config) const {
  int idx = args.indexOf("-ls");
  if (idx == -1) {
    idx = args.indexOf("--lang-suffix");
  }
  if (idx == -1) {
    return QVariant::fromValue(NothingParsed());
  }

  if (idx == args.size() - 1) {
    return QVariant::fromValue(ParseError{
        tr("You need to pass the language suffix mode after %1 argument")
            .arg(args[idx])});
  }

  const QString mode = args[idx + 1].toLower();

  LanguageSuffix languageSuffix;

  if (mode == "none" || mode == "off") {
    languageSuffix = LS_NONE;
  } else if (mode == "2" || mode == "two") {
    languageSuffix = LS_TWO_LETTER;
  } else if (mode == "3" || mode == "three") {
    languageSuffix = LS_THREE_LETTER;
  } else {
    return QVariant::fromValue(ParseError{
        tr("Invalid language suffix mode passed: %1 (expected none, 2 or 3)")
            .arg(args[idx + 1])});
  }

  return QVariant::fromValue(ParsedModifier{config.setGeneralConfig(
      config.generalConfig().setLanguageSuffix(languageSuffix))});
}

Maybe<CliArgParser::HelpInfo> LangSuffixArgParser::helpInfo() const {
  return just(HelpInfo{
      "-ls", "--lang-suffix", tr("mode"),
      tr("Append the subtitles language to the file name, as media servers "
         "expect (none, 2 for two-letter e.g. movie.pl.srt, 3 for "
         "three-letter e.g. movie.pol.srt)")});
}
