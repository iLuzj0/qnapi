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

#include "frmoptions.h"

#include "config/qnapiconfig.h"

#include "engines/napiprojektdownloadengine.h"
#include "engines/napisy24downloadengine.h"
#include "engines/opensubtitlesdownloadengine.h"

#include "forms/frmnapiprojektconfig.h"
#include "forms/frmnapisy24config.h"
#include "forms/frmopensubtitlesconfig.h"

#include "libqnapi.h"

#include <algorithm>
#include <QLocale>

#ifdef Q_OS_MAC
#include "utils/infoplistdockicon.h"
#endif

frmOptions::frmOptions(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      subtitleFormatsRegistry(LibQNapi::subtitleFormatsRegistry()),
      enginesRegistry(LibQNapi::subtitleDownloadEngineRegistry()) {
  ui.setupUi(this);

  ui.cbUiLanguage->setItemData(0, "", Qt::UserRole);
  ui.cbUiLanguage->setItemData(1, QLocale(QLocale::English).name(),
                               Qt::UserRole);
  ui.cbUiLanguage->setItemData(2, QLocale(QLocale::Italian).name(),
                               Qt::UserRole);
  ui.cbUiLanguage->setItemData(3, QLocale(QLocale::Polish).name(),
                               Qt::UserRole);
  ui.lbUiLangChangeNotice->setVisible(false);

#ifdef Q_OS_MAC
  ui.cbQuietBatch->hide();
#endif

#ifndef Q_OS_MAC
  ui.cbShowDockIcon->hide();
#endif

#ifdef Q_OS_WIN
  ui.cbChangePermissions->hide();
  ui.sbOPerm->hide();
  ui.sbGPerm->hide();
  ui.sbUPerm->hide();
#endif

  QString tlcode;
  ui.cbLangBackup->addItem("None", QVariant(""));
  foreach (QString lang, SubtitleLanguage("").listLanguages()) {
    tlcode = SubtitleLanguage(lang).toTwoLetter();
    ui.cbLang->addItem(QIcon(QString(":/languages/%1.png").arg(tlcode)), lang,
                       QVariant(tlcode));
    ui.cbLangBackup->addItem(QIcon(QString(":/languages/%1.png").arg(tlcode)),
                             lang, QVariant(tlcode));
  }

  ui.cbAiSourceLang->addItem(tr("Detect automatically"), QVariant("auto"));
  foreach (QString lang, SubtitleLanguage("").listLanguages()) {
    tlcode = SubtitleLanguage(lang).toTwoLetter();
    ui.cbAiSourceLang->addItem(
        QIcon(QString(":/languages/%1.png").arg(tlcode)), lang,
        QVariant(tlcode));
  }

  ui.leAiFfmpeg->setPlaceholderText(AiConfig::defaultFfmpegPath());
  ui.leAiWhisper->setPlaceholderText(AiConfig::defaultWhisperPath());
  ui.leAiWhisperModel->setPlaceholderText(AiConfig::defaultWhisperModel());
  ui.leAiLlamaServer->setPlaceholderText(AiConfig::defaultLlamaServerPath());
  ui.leAiLlmModel->setPlaceholderText(AiConfig::defaultLlmModel());
  ui.sbAiChunkSize->setRange(AiConfig::minChunkSize, AiConfig::maxChunkSize);

  const QList<QPair<QPushButton *, QLineEdit *>> aiPaths = {
      {ui.pbAiFfmpeg, ui.leAiFfmpeg},
      {ui.pbAiWhisper, ui.leAiWhisper},
      {ui.pbAiWhisperModel, ui.leAiWhisperModel},
      {ui.pbAiLlamaServer, ui.leAiLlamaServer},
      {ui.pbAiLlmModel, ui.leAiLlmModel}};
  for (const auto &aiPath : aiPaths) {
    QLineEdit *edit = aiPath.second;
    connect(aiPath.first, &QPushButton::clicked, this, [this, edit]() {
      const QString current =
          LibQNapi::resolveAiPath(edit->text(), edit->placeholderText());
      const QString path = QFileDialog::getOpenFileName(
          this, tr("Select a file"), QFileInfo(current).path());
      if (!path.isEmpty()) edit->setText(QDir::toNativeSeparators(path));
    });
    connect(edit, &QLineEdit::textChanged, this, &frmOptions::updateAiStatus);
  }
  connect(ui.cbAiEnabled, &QCheckBox::toggled, this,
          &frmOptions::updateAiStatus);
  connect(ui.pbAiGuidelinesDefault, &QPushButton::clicked, this, [this]() {
    ui.teAiGuidelines->setPlainText(currentDefaultGuidelines());
  });
  connect(ui.cbAiSourceLang,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &frmOptions::aiLanguagesChanged);
  connect(ui.cbLang,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &frmOptions::aiLanguagesChanged);

  setAttribute(Qt::WA_QuitOnClose, false);

  connect(ui.le7zPath, SIGNAL(textChanged(const QString &)), this,
          SLOT(le7zPathChanged()));
  connect(ui.pb7zPathSelect, SIGNAL(clicked()), this, SLOT(select7zPath()));
  connect(ui.leTmpPath, SIGNAL(textChanged(const QString &)), this,
          SLOT(leTmpPathChanged()));
  connect(ui.pbTmpPathSelect, SIGNAL(clicked()), this, SLOT(selectTmpPath()));

  connect(ui.twEngines, SIGNAL(itemSelectionChanged()), this,
          SLOT(twEnginesSelectionChanged()));
  connect(ui.twEngines, SIGNAL(itemChanged(QTableWidgetItem *)), this,
          SLOT(twEnginesItemChanged(QTableWidgetItem *)));

  connect(ui.pbMoveUp, SIGNAL(clicked()), this, SLOT(pbMoveUpClicked()));
  connect(ui.pbMoveDown, SIGNAL(clicked()), this, SLOT(pbMoveDownClicked()));
  connect(ui.pbEngineConf, SIGNAL(clicked()), this,
          SLOT(pbEngineConfClicked()));
  connect(ui.pbEngineInfo, SIGNAL(clicked()), this,
          SLOT(pbEngineInfoClicked()));

  connect(ui.cbSubFormat, SIGNAL(currentIndexChanged(int)), this,
          SLOT(subFormatChanged(int)));
  connect(ui.cbEncodingMethod, SIGNAL(currentIndexChanged(int)), this,
          SLOT(encodingMethodChanged(int)));
  connect(ui.cbAutoDetectEncoding, SIGNAL(clicked()), this,
          SLOT(autoDetectEncodingClicked()));
  connect(ui.cbShowAllEncodings, SIGNAL(clicked()), this,
          SLOT(showAllEncodingsClicked()));

  connect(ui.pbRestoreDefaults, SIGNAL(clicked()), this,
          SLOT(restoreDefaults()));

  showAllEncodings();

  foreach (QString format, subtitleFormatsRegistry->listFormatNames()) {
    ui.cbSubFormat->addItem(format);
  }

  QRect position = frameGeometry();
  position.moveCenter(QGuiApplication::primaryScreen()->availableGeometry().center());
  move(position.topLeft());
}

frmOptions::~frmOptions() {}

void frmOptions::le7zPathChanged() {
  QFileInfo f(ui.le7zPath->text());
  ui.le7zPath->setStyleSheet(f.isFile() && f.isExecutable() ? ""
                                                            : "color:red;");
}

void frmOptions::select7zPath() {
  QString path7z = QFileDialog::getOpenFileName(
      this, tr("Specify the path for 7z executable"),
      QFileInfo(ui.le7zPath->text()).path());
  if (!path7z.isEmpty()) {
    if (!QFileInfo(path7z).isExecutable())
      QMessageBox::warning(
          this, tr("Invalid path"),
          tr("Defined path to 7z executable is invalid. If you can not "
             "locate 7z executable, try installing p7zip-full package."));
    else
      ui.le7zPath->setText(path7z);
  }
}

void frmOptions::leTmpPathChanged() {
  QFileInfo f(ui.leTmpPath->text());
  ui.leTmpPath->setStyleSheet(f.isDir() && f.isWritable() ? "" : "color:red;");
}

void frmOptions::selectTmpPath() {
  QString tmpDir = QFileDialog::getExistingDirectory(
      this, tr("Specify temporary directory"),
      QFileInfo(ui.leTmpPath->text()).path(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!tmpDir.isEmpty()) ui.leTmpPath->setText(QFileInfo(tmpDir).path());
}

void frmOptions::twEnginesSelectionChanged() {
  if (ui.twEngines->selectedItems().size() < 1) return;

  int currentRow = ui.twEngines->row(ui.twEngines->selectedItems().at(0));

  ui.pbMoveUp->setEnabled(currentRow > 0);
  ui.pbMoveDown->setEnabled(currentRow < ui.twEngines->rowCount() - 1);
  ui.pbEngineConf->setEnabled(true);
  ui.pbEngineInfo->setEnabled(true);
}

void frmOptions::twEnginesItemChanged(QTableWidgetItem *item) {
  bool foundActive = false;

  for (int i = 0; i < ui.twEngines->rowCount(); ++i) {
    if (!ui.twEngines->item(i, 0)) {
      return;
    }

    if (ui.twEngines->item(i, 0)->checkState() == Qt::Checked) {
      foundActive = true;
      break;
    }
  }

  if (!foundActive) {
    item->setCheckState(Qt::Checked);
    QMessageBox::warning(
        this, tr("Warning"),
        tr("At least one subtitles engine must remain active!"));
  }
}

void frmOptions::pbMoveUpClicked() {
  int currentRow = ui.twEngines->row(ui.twEngines->selectedItems().at(0));

  QTableWidgetItem *current, *above;
  current = ui.twEngines->item(currentRow, 0);
  above = ui.twEngines->item(currentRow - 1, 0);

  QTableWidgetItem tmp = *current;
  *current = *above;
  *above = tmp;

  ui.twEngines->selectRow(currentRow - 1);
}

void frmOptions::pbMoveDownClicked() {
  int currentRow = ui.twEngines->row(ui.twEngines->selectedItems().at(0));

  QTableWidgetItem *current, *below;
  current = ui.twEngines->item(currentRow, 0);
  below = ui.twEngines->item(currentRow + 1, 0);

  QTableWidgetItem tmp = *current;
  *current = *below;
  *below = tmp;

  ui.twEngines->selectRow(currentRow + 1);
}

void frmOptions::pbEngineConfClicked() {
  QString engineName = ui.twEngines->selectedItems().at(0)->text();

  const QNapiConfig config = LibQNapi::loadConfig();
  auto enginesConfig = config.enginesConfig();

  EngineConfig engineCfg = enginesConfig[engineName];

  if (engineName == NapiProjektDownloadEngine::metadata.name()) {
    frmNapiProjektConfig configDialog(engineCfg, this);
    if (configDialog.exec() == QDialog::Accepted) {
      engineCfg = configDialog.getConfig();
    }
  } else if (engineName == OpenSubtitlesDownloadEngine::metadata.name()) {
    frmOpenSubtitlesConfig configDialog(engineCfg, this);
    if (configDialog.exec() == QDialog::Accepted) {
      engineCfg = configDialog.getConfig();
    }
  } else if (engineName == Napisy24DownloadEngine::metadata.name()) {
    frmNapisy24Config configDialog(engineCfg, this);
    if (configDialog.exec() == QDialog::Accepted) {
      engineCfg = configDialog.getConfig();
    }
  }

  enginesConfig[engineName] = engineCfg;

  LibQNapi::writeConfig(config.setEnginesConfig(enginesConfig));
}

void frmOptions::pbEngineInfoClicked() {
  QString engineName = ui.twEngines->selectedItems().at(0)->text();
  QString engineDescription =
      enginesRegistry->engineMetadata(engineName).description();

  QMessageBox::information(this, tr("%1 subtitles engine info").arg(engineName),
                           engineDescription);
}

void frmOptions::subFormatChanged(int format) {
  if (format == 0) {
    ui.cbSubExtension->setItemText(0, tr("Default"));
  } else {
    QString targetFormatName = ui.cbSubFormat->currentText();
    QSharedPointer<const SubtitleFormat> targetSF =
        subtitleFormatsRegistry->select(targetFormatName);
    QString targetDefaultExt = targetSF->defaultExtension();
    ui.cbSubExtension->setItemText(0, tr("Default (%1)").arg(targetDefaultExt));
  }
}

void frmOptions::encodingMethodChanged(int method) {
  EncodingChangeMethod cemMethod = static_cast<EncodingChangeMethod>(method);
  bool enableEncodingSettings = cemMethod == ECM_CHANGE;
  ui.cbEncFrom->setEnabled(enableEncodingSettings);
  ui.lbConvert->setEnabled(enableEncodingSettings);
  ui.lbConvertFrom->setEnabled(enableEncodingSettings);
  ui.cbEncTo->setEnabled(enableEncodingSettings);
  ui.cbAutoDetectEncoding->setEnabled(enableEncodingSettings);
  ui.cbShowAllEncodings->setEnabled(enableEncodingSettings);
  autoDetectEncodingClicked();
}

void frmOptions::autoDetectEncodingClicked() {
  bool checkedCE = static_cast<EncodingChangeMethod>(
                       ui.cbEncodingMethod->currentIndex()) == ECM_CHANGE;
  bool checkedADE = ui.cbAutoDetectEncoding->isChecked();
  ui.cbEncFrom->setEnabled(checkedCE && !checkedADE);
  ui.lbConvertFrom->setEnabled(checkedCE && !checkedADE);
}

void frmOptions::showAllEncodingsClicked() {
  QString encFrom = ui.cbEncFrom->currentText();
  QString encTo = ui.cbEncTo->currentText();

  if (ui.cbShowAllEncodings->isChecked()) {
    showAllEncodings();
  } else {
    ui.cbEncFrom->clear();
    ui.cbEncTo->clear();

    QStringList codecs;
    codecs << "windows-1250"
           << "windows-1257"
           << "ISO-8859-2"
           << "ISO-8859-13"
           << "ISO-8859-16"
           << "UTF-8";

    ui.cbEncFrom->addItems(codecs);
    ui.cbEncTo->addItems(codecs);
  }

  ui.cbEncFrom->setCurrentIndex(ui.cbEncFrom->findText(encFrom));
  ui.cbEncTo->setCurrentIndex(ui.cbEncTo->findText(encTo));
}

void frmOptions::showAllEncodings() {
  ui.cbEncFrom->clear();
  ui.cbEncTo->clear();

  QList<QByteArray> codecs = QTextCodec::availableCodecs();
  std::sort(codecs.begin(), codecs.end());
  for (QList<QByteArray>::iterator i = codecs.begin(); i != codecs.end(); i++) {
    ui.cbEncFrom->addItem(*i);
    ui.cbEncTo->addItem(*i);
  }
}

void frmOptions::writeConfig() {
#ifdef Q_OS_MAC
  InfoPlistDockIcon::setShowDockIcon(ui.cbShowDockIcon->isChecked());
#endif

  const QNapiConfig config = LibQNapi::loadConfig();

  QString permissions = QString("%1%2%3")
                            .arg(ui.sbUPerm->value())
                            .arg(ui.sbGPerm->value())
                            .arg(ui.sbOPerm->value());

  auto updatedGeneralConfig =
      config.generalConfig()
          .setUiLanguage(
              ui.cbUiLanguage->itemData(ui.cbUiLanguage->currentIndex())
                  .toString())
          .setP7zipPath(ui.le7zPath->text())
          .setTmpPath(ui.leTmpPath->text())
          .setLanguage(
              ui.cbLang->itemData(ui.cbLang->currentIndex()).toString())
          .setBackupLanguage(
              ui.cbLangBackup->itemData(ui.cbLangBackup->currentIndex())
                  .toString())
          .setNoBackup(ui.cbNoBackup->isChecked())
          .setQuietBatch(ui.cbQuietBatch->isChecked())
          .setSearchPolicy(
              static_cast<SearchPolicy>(ui.cbSearchPolicy->currentIndex()))
          .setDownloadPolicy(
              static_cast<DownloadPolicy>(ui.cbDownloadPolicy->currentIndex()))
          .setChangePermissionsEnabled(ui.cbChangePermissions->isChecked())
          .setChangePermissionsTo(permissions)
          .setLanguageSuffix(
              static_cast<LanguageSuffix>(ui.cbLangSuffix->currentIndex()))
          .setJobs(ui.sbJobs->value());

  QList<QPair<QString, bool> > updatedEnabledEngines;
  for (int i = 0; i < ui.twEngines->rowCount(); ++i) {
    updatedEnabledEngines << qMakePair(
        ui.twEngines->item(i, 0)->text(),
        ui.twEngines->item(i, 0)->checkState() == Qt::Checked);
  }

  QString targetSubFormat = ui.cbSubFormat->currentIndex() == 0
                                ? ""
                                : subtitleFormatsRegistry->listFormatNames().at(
                                      ui.cbSubFormat->currentIndex() - 1);
  QString targetSubExt = ui.cbSubExtension->currentIndex() == 0
                             ? ""
                             : ui.cbSubExtension->currentText();

  auto updatedPostProcessingConfig =
      config.postProcessingConfig()
          .setEnabled(ui.gbPpEnable->isChecked())
          .setEncodingChangeMethod(static_cast<EncodingChangeMethod>(
              ui.cbEncodingMethod->currentIndex()))
          .setEncodingAutoDetectFrom(ui.cbAutoDetectEncoding->isChecked())
          .setEncodingFrom(ui.cbEncFrom->currentText())
          .setEncodingTo(ui.cbEncTo->currentText())
          .setShowAllEncodings(ui.cbShowAllEncodings->isChecked())
          .setRemoveLines(ui.cbRemoveLines->isChecked())
          .setRemoveLinesWords(ui.teRemoveLinesWords->toPlainText().split("\n"))
          .setSubFormat(targetSubFormat)
          .setSubExtension(targetSubExt)
          .setSkipConvertAds(ui.cbSkipConvertAds->isChecked());

  auto updatedConfig =
      config.setGeneralConfig(updatedGeneralConfig)
          .setEnabledEngines(updatedEnabledEngines)
          .setPostProcessingConfig(updatedPostProcessingConfig)
          .setAiConfig(aiConfigFromForm(config.aiConfig()));

  LibQNapi::writeConfig(updatedConfig);
}

void frmOptions::readConfig(const QNapiConfig &config) {
#ifdef Q_OS_MAC
  ui.cbShowDockIcon->setChecked(InfoPlistDockIcon::readShowDockIcon());
#endif

  QString systemLang = QLocale::languageToString(QLocale::system().language());
  QString localizedSystemLang = QCoreApplication::translate(
      "frmOptions", systemLang.toStdString().data());
  ui.cbUiLanguage->setItemText(0,
                               tr("Auto-detected based on system language (%1)")
                                   .arg(localizedSystemLang));
  ui.cbUiLanguage->setCurrentIndex(ui.cbUiLanguage->findData(
      config.generalConfig().uiLanguage(), Qt::UserRole, Qt::MatchStartsWith));

  connect(
      ui.cbUiLanguage,
      static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
      [=]() { ui.lbUiLangChangeNotice->setVisible(true); });

  ui.le7zPath->setText(config.generalConfig().p7zipPath());
  ui.leTmpPath->setText(config.generalConfig().tmpPath());
  ui.cbLang->setCurrentIndex(ui.cbLang->findData(
      SubtitleLanguage(config.generalConfig().language()).toTwoLetter()));
  ui.cbLangBackup->setCurrentIndex(ui.cbLangBackup->findData(
      SubtitleLanguage(config.generalConfig().backupLanguage()).toTwoLetter()));
  ui.cbLangSuffix->setCurrentIndex(config.generalConfig().languageSuffix());
  ui.cbNoBackup->setChecked(config.generalConfig().noBackup());
  ui.cbQuietBatch->setChecked(config.generalConfig().quietBatch());
  ui.sbJobs->setMaximum(GeneralConfig::maxJobs);
  ui.sbJobs->setValue(config.generalConfig().jobs());

  ui.twEngines->clear();

  QList<QPair<QString, bool> > engines = config.enabledEngines();
  ui.twEngines->setColumnCount(1);
  ui.twEngines->setRowCount(engines.size());

  for (int i = 0; i < engines.size(); ++i) {
    QPair<QString, bool> e = engines.at(i);
    QIcon engineIcon =
        QIcon(QPixmap(enginesRegistry->enginePixmapData(e.first)));
    QTableWidgetItem *item = new QTableWidgetItem(engineIcon, e.first);
    item->setCheckState(e.second ? Qt::Checked : Qt::Unchecked);
    ui.twEngines->setItem(i, 0, item);
  }

  ui.twEngines->horizontalHeader()->hide();
  ui.twEngines->verticalHeader()->hide();
  ui.twEngines->verticalHeader()->setDefaultSectionSize(20);
  ui.twEngines->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui.twEngines->setColumnWidth(0, 300);

  ui.cbSearchPolicy->setCurrentIndex(config.generalConfig().searchPolicy());
  ui.cbDownloadPolicy->setCurrentIndex(config.generalConfig().downloadPolicy());

  ui.cbEncodingMethod->setCurrentIndex(
      config.postProcessingConfig().encodingChangeMethod());
  ui.cbAutoDetectEncoding->setChecked(
      config.postProcessingConfig().encodingAutoDetectFrom());
  ui.cbEncFrom->setCurrentIndex(
      ui.cbEncFrom->findText(config.postProcessingConfig().encodingFrom()));
  ui.cbEncTo->setCurrentIndex(
      ui.cbEncTo->findText(config.postProcessingConfig().encodingTo()));
  ui.cbShowAllEncodings->setChecked(
      config.postProcessingConfig().showAllEncodings());
  ui.cbRemoveLines->setChecked(
      config.postProcessingConfig().removeLines());
  ui.teRemoveLinesWords->setText(
      config.postProcessingConfig().removeLinesWords().join("\n"));

  ui.cbSubFormat->setCurrentIndex(0);
  ui.cbSubExtension->setCurrentIndex(0);

  int formatIdx = 1;
  foreach (QString format, subtitleFormatsRegistry->listFormatNames()) {
    if (config.postProcessingConfig().subFormat() == format) {
      ui.cbSubFormat->setCurrentIndex(formatIdx);
    }
    ++formatIdx;
  }

  ui.cbSubExtension->setCurrentText(
      config.postProcessingConfig().subExtension());
  ui.cbSkipConvertAds->setChecked(
      config.postProcessingConfig().skipConvertAds());

  ui.cbChangePermissions->setChecked(
      config.generalConfig().changePermissionsEnabled());
  QString permissions = config.generalConfig().changePermissionsTo();
  char o, g, u;
  o = permissions.at(0).toLatin1() - '0';
  g = permissions.at(1).toLatin1() - '0';
  u = permissions.at(2).toLatin1() - '0';
  ui.sbUPerm->setValue((0 <= o && o <= 7) ? o : 6);
  ui.sbGPerm->setValue((0 <= g && g <= 7) ? g : 4);
  ui.sbOPerm->setValue((0 <= u && u <= 7) ? u : 4);

  encodingMethodChanged(
      static_cast<int>(config.postProcessingConfig().encodingChangeMethod()));
  showAllEncodingsClicked();

  ui.gbPpEnable->setChecked(config.postProcessingConfig().enabled());

  const AiConfig ai = config.aiConfig();
  ui.cbAiEnabled->setChecked(ai.fallbackEnabled());
  ui.leAiFfmpeg->setText(ai.ffmpegPath());
  ui.leAiWhisper->setText(ai.whisperPath());
  ui.leAiWhisperModel->setText(ai.whisperModel());
  ui.leAiLlamaServer->setText(ai.llamaServerPath());
  ui.leAiLlmModel->setText(ai.llmModel());
  const QString source = ai.sourceLanguage().toLower() == "auto"
                             ? QString("auto")
                             : SubtitleLanguage(ai.sourceLanguage()).toTwoLetter();
  ui.cbAiSourceLang->setCurrentIndex(
      qMax(0, ui.cbAiSourceLang->findData(source)));
  ui.leAiLlamaArgs->setText(ai.llamaServerArgs());
  ui.sbAiChunkSize->setValue(ai.chunkSize());
  shownDefaultGuidelines = currentDefaultGuidelines();
  ui.teAiGuidelines->setPlainText(ai.translationGuidelines().trimmed().isEmpty()
                                      ? shownDefaultGuidelines
                                      : ai.translationGuidelines());
  updateAiStatus();
}

QString frmOptions::currentDefaultGuidelines() const {
  return AiSubtitleGenerator::defaultGuidelines(
      ui.cbAiSourceLang->currentData().toString(),
      ui.cbLang->currentData().toString());
}

void frmOptions::aiLanguagesChanged() {
  const QString fresh = currentDefaultGuidelines();
  if (ui.teAiGuidelines->toPlainText().trimmed() ==
      shownDefaultGuidelines.trimmed()) {
    ui.teAiGuidelines->setPlainText(fresh);
  }
  shownDefaultGuidelines = fresh;
}

AiConfig frmOptions::aiConfigFromForm(const AiConfig &base) const {
  // an unchanged default is stored as empty, so it follows the language
  // settings and later improvements to the built-in text
  QString guidelines = ui.teAiGuidelines->toPlainText().trimmed();
  if (guidelines == currentDefaultGuidelines().trimmed()) guidelines.clear();

  return base.setFallbackEnabled(ui.cbAiEnabled->isChecked())
      .setFfmpegPath(ui.leAiFfmpeg->text().trimmed())
      .setWhisperPath(ui.leAiWhisper->text().trimmed())
      .setWhisperModel(ui.leAiWhisperModel->text().trimmed())
      .setLlamaServerPath(ui.leAiLlamaServer->text().trimmed())
      .setLlmModel(ui.leAiLlmModel->text().trimmed())
      .setSourceLanguage(ui.cbAiSourceLang->currentData().toString())
      .setChunkSize(ui.sbAiChunkSize->value())
      .setTranslationGuidelines(guidelines)
      .setLlamaServerArgs(ui.leAiLlamaArgs->text().trimmed());
}

void frmOptions::updateAiStatus() {
  const QNapiConfig formConfig =
      QNapiConfig().setAiConfig(aiConfigFromForm(AiConfig()));
  const QStringList missing = AiSubtitleGenerator(formConfig).missingTools();

  if (missing.isEmpty()) {
    ui.lbAiStatus->setText(tr("All tools and models were found."));
    ui.lbAiStatus->setStyleSheet("");
    return;
  }

  QStringList lines;
  foreach (const QString &path, missing) {
    lines << QDir::toNativeSeparators(path);
  }
  ui.lbAiStatus->setText(tr("Not found:") + "\n" + lines.join("\n"));
  ui.lbAiStatus->setStyleSheet(ui.cbAiEnabled->isChecked() ? "color:red;"
                                                           : "");
}

void frmOptions::restoreDefaults() {
  QSettings settings;
  settings.clear();
  const QNapiConfig defaultConfig =
      LibQNapi::configReader()->readConfig(settings);

#ifdef Q_OS_MAC
  InfoPlistDockIcon::setShowDockIcon(true);
#endif

  readConfig(defaultConfig);
}
