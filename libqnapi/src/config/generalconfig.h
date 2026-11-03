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

#ifndef GENERALCONFIG_H
#define GENERALCONFIG_H

#include <QString>

enum SearchPolicy {
  SP_BREAK_IF_FOUND = 0,
  SP_SEARCH_ALL = 1,
  SP_SEARCH_ALL_WITH_BACKUP_LANG = 2
};

enum DownloadPolicy {
  DP_ALWAYS_SHOW_LIST = 0,
  DP_SHOW_LIST_IF_NEEDED = 1,
  DP_NEVER_SHOW_LIST = 2
};

// Media servers (Jellyfin, Plex, Emby) expect the subtitle language in the
// file name, e.g. Movie.pol.srt. Which code to append, if any.
enum LanguageSuffix { LS_NONE = 0, LS_TWO_LETTER = 1, LS_THREE_LETTER = 2 };

class GeneralConfig {
 public:
  // NapiProjekt's rate limiter only blocks requests that arrive one after
  // another; a handful in flight at once sail through. Downloading a season
  // therefore takes seconds instead of minutes. Keep it modest so we stay a
  // well-behaved client.
  static const int defaultJobs = 8;
  static const int maxJobs = 8;

 private:
  QString uiLanguage_;
  QString p7zipPath_;
  QString tmpPath_;
  QString language_;
  QString backupLanguage_;
  bool noBackup_;
  bool quietBatch_;
  SearchPolicy searchPolicy_;
  DownloadPolicy downloadPolicy_;
  bool changePermissionsEnabled_;
  QString changePermissionsTo_;
  LanguageSuffix languageSuffix_;
  int jobs_;

 public:
  GeneralConfig() {}
  GeneralConfig(const QString& uiLanguage, const QString& p7zipPath,
                const QString& tmpPath, const QString& language,
                const QString& backupLanguage, const bool& noBackup,
                const bool& quietBatch, const SearchPolicy& searchPolicy,
                const DownloadPolicy& downloadPolicy,
                const bool& changePermissionsEnabled,
                const QString& changePermissionsTo,
                const LanguageSuffix& languageSuffix = LS_NONE,
                const int& jobs = defaultJobs)
      : uiLanguage_(uiLanguage),
        p7zipPath_(p7zipPath),
        tmpPath_(tmpPath),
        language_(language),
        backupLanguage_(backupLanguage),
        noBackup_(noBackup),
        quietBatch_(quietBatch),
        searchPolicy_(searchPolicy),
        downloadPolicy_(downloadPolicy),
        changePermissionsEnabled_(changePermissionsEnabled),
        changePermissionsTo_(changePermissionsTo),
        languageSuffix_(languageSuffix),
        jobs_(jobs) {}

  QString uiLanguage() const { return uiLanguage_; }
  QString p7zipPath() const { return p7zipPath_; }
  QString tmpPath() const { return tmpPath_; }
  QString language() const { return language_; }
  QString backupLanguage() const { return backupLanguage_; }
  bool noBackup() const { return noBackup_; }
  bool quietBatch() const { return quietBatch_; }
  SearchPolicy searchPolicy() const { return searchPolicy_; }
  DownloadPolicy downloadPolicy() const { return downloadPolicy_; }
  bool changePermissionsEnabled() const { return changePermissionsEnabled_; }
  QString changePermissionsTo() const { return changePermissionsTo_; }
  LanguageSuffix languageSuffix() const { return languageSuffix_; }
  int jobs() const { return jobs_ > 0 ? jobs_ : defaultJobs; }

  const GeneralConfig setUiLanguage(const QString& uiLanguage) const {
    return GeneralConfig(uiLanguage, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setP7zipPath(const QString& p7zipPath) const {
    return GeneralConfig(uiLanguage_, p7zipPath, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setTmpPath(const QString& tmpPath) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setLanguage(const QString& language) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setBackupLanguage(const QString& backupLanguage) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setNoBackup(const bool& noBackup) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setQuietBatch(const bool& quietBatch) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setSearchPolicy(const SearchPolicy& searchPolicy) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setDownloadPolicy(
      const DownloadPolicy& downloadPolicy) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setChangePermissionsEnabled(
      const bool& changePermissionsEnabled) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled,
                         changePermissionsTo_, languageSuffix_, jobs_);
  }
  const GeneralConfig setChangePermissionsTo(
      const QString& changePermissionsTo) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo, languageSuffix_, jobs_);
  }
  const GeneralConfig setLanguageSuffix(
      const LanguageSuffix& languageSuffix) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix, jobs_);
  }
  const GeneralConfig setJobs(const int& jobs) const {
    return GeneralConfig(uiLanguage_, p7zipPath_, tmpPath_, language_,
                         backupLanguage_, noBackup_, quietBatch_, searchPolicy_,
                         downloadPolicy_, changePermissionsEnabled_,
                         changePermissionsTo_, languageSuffix_, jobs);
  }

  QString toString() const;
};

#endif
