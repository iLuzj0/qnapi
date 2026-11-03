# QNapi [![build](https://github.com/QNapi/qnapi/actions/workflows/build.yml/badge.svg)](https://github.com/QNapi/qnapi/actions/workflows/build.yml) [![GitHub release](https://img.shields.io/github/release/QNapi/qnapi.svg)](https://github.com/QNapi/qnapi/releases) [![Github All Releases](https://img.shields.io/github/downloads/QNapi/qnapi/total.svg)](https://qnapi.github.io/#download)

QNapi is free software for automatic fetching subtitles for given movie file.
It uses online databases such as NapiProjekt, OpenSubtitles and Napisy24.
It is based on the Qt 6 library, so it can be launched on any supported operating
system, including Windows, macOS and Linux.

When no service has subtitles for a file, QNapi can optionally generate them
locally: the audio is transcribed with [whisper.cpp](https://github.com/ggml-org/whisper.cpp)
and translated by a local language model running in
[llama.cpp](https://github.com/ggml-org/llama.cpp). See [AI subtitles](#ai-subtitles).

## Binary packages

Latest binary packages are available at http://qnapi.github.io/#download

## Building from source

#### Prerequisites

* C++ compiler with C++17 support installed (`clang++`, `g++` or *MinGW* for Windows), present in `PATH`
* Qt 6.4+ with the *Qt 5 Compatibility Module* (Core5Compat), with `qmake` present in `PATH` (you can find one at https://www.qt.io/download-open-source/).
  On Debian/Ubuntu: `sudo apt install qt6-base-dev libqt6core5compat6-dev libmediainfo-dev`

#### Binary prerequisites

QNapi requires these binary dependencies:

* p7zip (7z, 7za) - to unpack subtitles, which are commonly compressed with 7zip
* libmediainfo - to retrieve movie info such as dimensions, duration and frame rate

Linux/UNIX users can find these dependencies in separate packages.

Statically compiled **p7zip** binaries are provided in this repository for Windows/macOS
users at `win32/content/7za.exe` and `macx/content/7za`, respectively. Similarly,
compiled **libmediainfo** libraries are provided for Windows/macOS in
`deps/libmediainfo/`.

> **WARNING!** Precompiled binaries are stripped from the source archive!

#### Cloning the source code

First, you have to clone project source code using git client, together with the two
submodules needed to compile it:

```
$ git clone https://github.com/QNapi/qnapi.git
$ cd qnapi
$ git submodule update --init deps/libmaia deps/qt-maybe
```

The other two submodules, `deps/whisper.cpp` and `deps/llama.cpp`, are only needed to
build the AI subtitles tools (see below); `git clone --recursive` fetches them as well.

#### Compiling

To compile the application, you have to execute two following commands in `qnapi` root directory:

`$ qmake`

This will produce `Makefile`.

> By appending `CONFIG+=no_cli` or `CONFIG+=no_gui` to qmake invocation you can disable building
> command-line or graphical interface binaries. On Windows, `CONFIG+=no_ai` skips building
> the AI subtitles tools during `make install`.


`$ make` (or `mingw32-make` on Windows)

This will compile the sources and build executable binary (or app bundle on macOS).

> **Important!** Windows users have to execute one more command:
>
> `$ make install` (or `mingw32-make install`)
>
> This one will copy all binaries, libraries and other dependencies to `win32/out` directory.

#### Running

By default, output binaries are placed by `make` in different locations, depending on your operating system:

* Linux - `qnapi` in root project directory
* macOS - `macx/QNapi.app` bundle
* Windows - `win32/out/qnapi.exe` executable

After you locate your binaries, you can run the application.

> ##### 7zip note #####
> For proper subtitle extraction after download, *7zip* executable is required to be passed in application's settings.
> Linux users have to install 7zip binary package from distribution repositories or compile on its own.
> For Windows and macOS there are pre-built binaries included in this repository, in `win32` and `macx` directories appropriately and should be automatically detected by the application.

## Command-line usage

`qnapic --help` (or `qnapi --help`) lists all options. Some useful ones:

* `-ls 3` - name the subtitles after their language (`Movie.pol.srt`), which is how
  Jellyfin, Plex and Emby find external subtitles; `-ls 2` gives `Movie.pl.srt`
* `-j 8` - process up to 8 files in parallel. The subtitle services throttle requests
  that arrive one after another but let a burst of concurrent ones through, so a whole
  season downloads in seconds instead of minutes; files that still get rate-limited are
  retried together after a short wait
* `-ai` - generate subtitles locally for files no service has subtitles for

The language suffix, parallel jobs and AI subtitles can also be set in the settings
window, so they apply to files sent to QNapi from the file manager.

## AI subtitles

With AI subtitles enabled (`-ai`, or the *AI subtitles* tab in the settings), files that
end up without subtitles in the preferred language are processed in two phases, so the
speech recognition and the language model never share the graphics card's memory:

1. ffmpeg extracts the audio and whisper.cpp transcribes it, with timings taken from
   word-level timestamps;
2. llama-server is started once and translates every transcription in chunks. The model
   only sees text keyed by line number and the cue timings are copied from the source,
   so a misbehaving model can garble a line but never desynchronise the subtitles.

The result goes through the usual post-processing and is saved like downloaded
subtitles, starting with a cue saying it was generated by AI. Any GGUF instruct model
that llama.cpp can run works; the default is
[Bielik 11B](https://huggingface.co/speakleash), which handles Polish well. The style
of the translation is set by editable guidelines in the settings.

On Windows, `scripts/build-ai-tools.bat` builds whisper-cli and llama-server with CUDA
(Visual Studio 2022 and the CUDA toolkit are required; `make install` runs it unless
`CONFIG+=no_ai` is given), and `scripts/download-ai-models.ps1` downloads the models into
`win32/out/models/`. On other systems, build whisper.cpp and llama.cpp following their own
instructions and point QNapi at the binaries and models in the settings.

## Making redistributable package

### macOS

#### Prerequisites

You need `appdmg` script installed. You can found it at https://github.com/LinusU/node-appdmg

#### Building .dmg image

To build .dmg image for macOS with nice, drag&drop installer, you have to execute:

`$ make appdmg`

`QNapi-x.y.z.dmg` will appear in `macx` directory when command is completed.

### Windows

#### Prerequisites

You need to have **NSIS** 2.x installed. You can found it at http://nsis.sourceforge.net

Also, you will need to manually install NSIS plugin **nsProcess**. It can be found at
http://nsis.sourceforge.net/NsProcess_plugin

#### Building Windows installer

Installer script is placed at `win32/QNapi-setup.nsi`. You can build binary exe package using NSIS user interface (by `right mouse button -> compile NSIS script`) or from command line:

`$ C:\Path\To\makensis.exe QNapi-setup.nsi`

After a while, `QNapi-x.y.z-setup.exe` file will appear in `win32` directory.
