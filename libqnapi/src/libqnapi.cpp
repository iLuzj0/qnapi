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

#include "libqnapi.h"
#include "movieinfo/libmediainfomovieinfoprovider.h"
#include "version.h"

#include <QDir>
#include <QFileInfo>
#include <QLocale>

QString LibQNapi::appExecutableFilePath = "";
QString LibQNapi::appExecutableDir = "";

void LibQNapi::init(const QString& appExecutablePath) {
  LibQNapi::appExecutableFilePath =
      QFileInfo(appExecutablePath).absoluteFilePath();
  LibQNapi::appExecutableDir =
      QFileInfo(appExecutableFilePath).absoluteDir().path();
}

QString LibQNapi::version() { return QString(QNAPI_VERSION); }

QString LibQNapi::displayableVersion() {
  return QString(QNAPI_DISPLAYABLE_VERSION);
}

QString LibQNapi::webpageUrl() { return QString(QNAPI_URL); }

QSharedPointer<const StaticConfig> LibQNapi::staticConfig() {
  return QSharedPointer<const StaticConfig>(new StaticConfig);
}

QSharedPointer<const ConfigReader> LibQNapi::configReader() {
  return QSharedPointer<const ConfigReader>(new ConfigReader(
      appExecutableDir, staticConfig(), subtitleDownloadEngineRegistry()));
}

QSharedPointer<const ConfigWriter> LibQNapi::configWriter() {
  QString ver = version();
  return QSharedPointer<const ConfigWriter>(new ConfigWriter(ver));
}

const QNapiConfig LibQNapi::loadConfig() {
  if (LibQNapi::isPortableMode())
    return LibQNapi::configReader()->readPortableConfig(
        LibQNapi::portableConfigPath());
  else
    return LibQNapi::configReader()->readUserConfig();
}

void LibQNapi::writeConfig(const QNapiConfig& config) {
  if (LibQNapi::isPortableMode())
    LibQNapi::configWriter()->writePortableConfig(
        LibQNapi::portableConfigPath(), config);
  else
    LibQNapi::configWriter()->writeUserConfig(config);
}

bool LibQNapi::isPortableMode() {
  return QFileInfo(portableConfigPath()).exists();
}

QString LibQNapi::portableConfigPath() {
  return appExecutableDir + QDir::separator() + "qnapi.ini";
}

QString LibQNapi::uiLanguage(const GeneralConfig& generalConfig) {
  if (generalConfig.uiLanguage().isEmpty()) {
    return QLocale::system().name();
  } else {
    return generalConfig.uiLanguage();
  }
}

QSharedPointer<const SubtitleDownloadEnginesRegistry>
LibQNapi::subtitleDownloadEngineRegistry() {
  return QSharedPointer<const SubtitleDownloadEnginesRegistry>(
      new SubtitleDownloadEnginesRegistry(
          LibQNapi::displayableVersion(),
          LibQNapi::staticConfig()->subtitleExtensions(),
          &LibQNapi::p7zipDecoder));
}

QSharedPointer<const MovieInfoProvider> LibQNapi::movieInfoProvider() {
  return QSharedPointer<const MovieInfoProvider>(
      new LibmediainfoMovieInfoProvider);
}

QSharedPointer<const SubtitleConverter> LibQNapi::subtitleConverter(
    const PostProcessingConfig& ppConfig) {
  return QSharedPointer<const SubtitleConverter>(new SubtitleConverter(
      LibQNapi::subtitleFormatsRegistry(), LibQNapi::movieInfoProvider(),
      ppConfig.skipConvertAds()));
}

QSharedPointer<const SubtitleFormatsRegistry>
LibQNapi::subtitleFormatsRegistry() {
  return QSharedPointer<SubtitleFormatsRegistry>(new SubtitleFormatsRegistry);
}

QSharedPointer<const P7ZipDecoder> LibQNapi::p7zipDecoder(
    const QString& p7zipPath) {
  return QSharedPointer<const P7ZipDecoder>(new P7ZipDecoder(p7zipPath));
}

QSharedPointer<const SubtitleMatcher> LibQNapi::subtitleMatcher(
    const QNapiConfig& config) {
  return QSharedPointer<const SubtitleMatcher>(
      new SubtitleMatcher(config.generalConfig().noBackup(),
                          config.postProcessingConfig().enabled(),
                          config.postProcessingConfig().subFormat(),
                          config.postProcessingConfig().subExtension(),
                          config.generalConfig().changePermissionsEnabled(),
                          config.generalConfig().changePermissionsTo(),
                          LibQNapi::subtitleFormatsRegistry(),
                          config.generalConfig().languageSuffix()));
}

QSharedPointer<const SubtitlePostProcessor> LibQNapi::subtitlePostProcessor(
    const PostProcessingConfig& config) {
  return QSharedPointer<const SubtitlePostProcessor>(
      new SubtitlePostProcessor(config, LibQNapi::subtitleConverter(config)));
}

QString LibQNapi::resolveAiPath(const QString& configuredPath,
                                const QString& defaultPath) {
  const QString path = configuredPath.isEmpty() ? defaultPath : configuredPath;
  if (QFileInfo(path).isAbsolute()) return QDir::cleanPath(path);
  return QDir::cleanPath(appExecutableDir + "/" + path);
}

QSharedPointer<const AudioExtractor> LibQNapi::audioExtractor(
    const AiConfig& config) {
  return QSharedPointer<const AudioExtractor>(new AudioExtractor(
      resolveAiPath(config.ffmpegPath(), AiConfig::defaultFfmpegPath())));
}

QSharedPointer<const WhisperTranscriber> LibQNapi::whisperTranscriber(
    const AiConfig& config) {
  return QSharedPointer<const WhisperTranscriber>(new WhisperTranscriber(
      resolveAiPath(config.whisperPath(), AiConfig::defaultWhisperPath()),
      resolveAiPath(config.whisperModel(), AiConfig::defaultWhisperModel())));
}

QSharedPointer<LlamaServer> LibQNapi::llamaServer(const AiConfig& config,
                                                  const QString& logPath) {
  return QSharedPointer<LlamaServer>(new LlamaServer(
      resolveAiPath(config.llamaServerPath(),
                    AiConfig::defaultLlamaServerPath()),
      resolveAiPath(config.llmModel(), AiConfig::defaultLlmModel()), logPath));
}

QSharedPointer<const SubtitleTranslator> LibQNapi::subtitleTranslator(
    const QSharedPointer<LlamaServer>& server, const AiConfig& config,
    const QString& sourceLanguage, const QString& targetLanguage) {
  return QSharedPointer<const SubtitleTranslator>(new SubtitleTranslator(
      server, sourceLanguage, targetLanguage, config.chunkSize()));
}

QSharedPointer<const BatchDownloader> LibQNapi::batchDownloader(
    const QNapiConfig& config, const Maybe<QString>& specificEngine) {
  return QSharedPointer<const BatchDownloader>(
      new BatchDownloader(config, specificEngine));
}

QSharedPointer<const AiSubtitleGenerator> LibQNapi::aiSubtitleGenerator(
    const QNapiConfig& config) {
  return QSharedPointer<const AiSubtitleGenerator>(
      new AiSubtitleGenerator(config));
}
