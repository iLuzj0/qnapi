TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = libqnapi cli gui

no_cli:message(will skip building qnapic cli application)
no_cli:SUBDIRS -= cli

no_gui:message(will skip building qnapi gui application)
no_gui:SUBDIRS -= gui

TRANSLATIONS += translations/qnapi_it.ts translations/qnapi_pl.ts

include(qnapi.pri)

unix {
    DATADIR=$${INSTALL_PREFIX}/share

    doc.path = $${INSTALL_PREFIX}/share/doc/qnapi
    doc.files = doc/ChangeLog \
        doc/LICENSE \
        doc/LICENSE-pl \
        doc/COPYRIGHT \
        doc/qnapi-download.desktop \
        doc/qnapi-scan.desktop \
        doc/qnapi-download.schemas \
        doc/qnapi-scan.schemas
    man.path = $${INSTALL_PREFIX}/share/man/man1/
    man.files = doc/man/qnapi.1.gz
    man_it.path = $${INSTALL_PREFIX}/share/man/it/man1/
    man_it.files = doc/man/it/qnapi.1.gz
    man_pl.path = $${INSTALL_PREFIX}/share/man/pl/man1/
    man_pl.files = doc/man/pl/qnapi.1.gz
    icons16.files = gui/res/icons/16x16/apps/qnapi.png
    icons16.path = $${DATADIR}/icons/hicolor/16x16/apps/
    icons32.files = gui/res/icons/32x32/apps/qnapi.png
    icons32.path = $${DATADIR}/icons/hicolor/32x32/apps/
    icons48.files = gui/res/icons/48x48/apps/qnapi.png
    icons48.path = $${DATADIR}/icons/hicolor/48x48/apps/
    icons128.files = gui/res/icons/128x128/apps/qnapi.png
    icons128.path = $${DATADIR}/icons/hicolor/128x128/apps/
    icons512.files = gui/res/icons/512x512/apps/qnapi.png
    icons512.path = $${DATADIR}/icons/hicolor/512x512/apps/
    INSTALLS += icons16 icons32 icons48 icons128 icons512

    desktop.path = $${INSTALL_PREFIX}/share/applications
    desktop.files = doc/qnapi.desktop
    INSTALLS += doc man man_it man_pl desktop
}

macx:!no_gui {
    macdeploy.commands = macdeployqt macx/QNapi.app
    appdmg.depends = macdeploy
    appdmg.commands = appdmg macx/appdmg.json macx/QNapi.dmg

    QMAKE_EXTRA_TARGETS += macdeploy appdmg
}

win32 {
    QMAKE_STRIP = echo

    p7zip.files = win32/content/7za.exe
    p7zip.path = $${INSTALL_PREFIX}

    doc.files = doc/ChangeLog \
                doc/LICENSE \
                doc/LICENSE-pl
    doc.path = $${INSTALL_PREFIX}

    lessThan(QT_MAJOR_VERSION, 6) {
        icudlls.files = $$[QT_INSTALL_BINS]/icuin54.dll \
                        $$[QT_INSTALL_BINS]/icuuc54.dll \
                        $$[QT_INSTALL_BINS]/icudt54.dll
        icudlls.path = $${INSTALL_PREFIX}
        INSTALLS += icudlls
    }

    libmediainfodlls.files += deps/libmediainfo/bin/MediaInfo.dll
    libmediainfodlls.path = $${INSTALL_PREFIX}

    DEPLOYWIN_FLAGS = --no-translations --no-quick-import --no-system-d3d-compiler
    lessThan(QT_MAJOR_VERSION, 6) {
        DEPLOYWIN_FLAGS += --no-angle --no-webkit --no-webkit2
    }
    greaterThan(QT_MAJOR_VERSION, 5) {
        DEPLOYWIN_FLAGS += --no-opengl-sw --compiler-runtime
    }
    deploywin.commands += windeployqt $${DEPLOYWIN_FLAGS}
    !no_cli:deploywin.commands += $${INSTALL_PREFIX}/qnapic.exe
    !no_gui:deploywin.commands += $${INSTALL_PREFIX}/qnapi.exe

    platform.files = $$[QT_INSTALL_PLUGINS]/platforms/qwindows.dll
    platform.path = $${INSTALL_PREFIX}/platforms
    platform.depends = deploywin

    QMAKE_EXTRA_TARGETS += deploywin platform

    INSTALLS += p7zip doc libmediainfodlls platform

    # Tools for qnapic --ai-fallback: whisper-cli, llama-server (built with
    # Visual Studio 2022 + CUDA from deps/whisper.cpp and deps/llama.cpp),
    # ffmpeg and their DLLs. Skip with CONFIG+=no_ai or QNAPI_SKIP_AI=1.
    # The models are downloaded separately: scripts/download-ai-models.ps1
    !no_ai {
        aitools.path = $${INSTALL_PREFIX}
        aitools.extra = cmd /c $$shell_quote($$shell_path($${PWD}/scripts/build-ai-tools.bat)) $$shell_quote($$shell_path($${INSTALL_PREFIX}))
        INSTALLS += aitools
    }
}

no_ai:message(will skip building the AI fallback tools)

doxygen.commands = rm -fr doc/doxygen/* && doxygen Doxyfile && echo "Doxygen documentation generated in doc/doxygen/index.html"
QMAKE_EXTRA_TARGETS += doxygen

