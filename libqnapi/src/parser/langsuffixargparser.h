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

#ifndef LANGSUFFIXARGPARSER_H
#define LANGSUFFIXARGPARSER_H

#include <QCoreApplication>
#include "parser/cliargparser.h"
#include "tr.h"

class LangSuffixArgParser : public CliArgParser {
  Q_DECLARE_CLASS_TR(LangSuffixArgParser)

 public:
  LangSuffixArgParser();

  virtual QVariant parse(const QStringList& args,
                         const QNapiConfig& config) const;

  virtual Maybe<HelpInfo> helpInfo() const;
};

#endif  // LANGSUFFIXARGPARSER_H
