@echo off
rem Builds the native tools used by `qnapic --ai-fallback` and copies them,
rem together with ffmpeg and the runtime DLLs they need, into the output dir:
rem
rem   whisper-cli.exe   (deps\whisper.cpp)  speech recognition
rem   llama-server.exe  (deps\llama.cpp)    local LLM used for translation
rem   ffmpeg.exe        (FFMPEG_DIR)        audio extraction
rem
rem Usage: scripts\build-ai-tools.bat [output dir]   (default: win32\out)
rem
rem Requires Visual Studio 2022 (C++ workload) and the CUDA toolkit.
rem Overridable through environment variables:
rem   CUDA_DIR         CUDA toolkit         (default: %CUDA_PATH_V12_4%)
rem   CUDA_ARCH        GPU architectures    (default: native, i.e. this GPU)
rem   FFMPEG_DIR       dir with ffmpeg.exe  (default: C:\ffmpeg\bin)
rem   QNAPI_SKIP_AI=1  skip everything (e.g. on machines without CUDA)
rem
rem whisper.cpp and llama.cpp each vendor their own, different version of ggml,
rem so both are linked statically: their ggml DLLs would otherwise collide in
rem the shared output directory. Rebuilding is incremental.

setlocal EnableExtensions

if "%QNAPI_SKIP_AI%"=="1" (
    echo build-ai-tools: QNAPI_SKIP_AI=1, skipping
    exit /b 0
)

set "ROOT=%~dp0.."
for %%I in ("%ROOT%") do set "ROOT=%%~fI"

set "OUT=%~1"
if "%OUT%"=="" set "OUT=%ROOT%\win32\out"
for %%I in ("%OUT%") do set "OUT=%%~fI"

set "BUILD=%ROOT%\build\ai"

if "%CUDA_DIR%"=="" set "CUDA_DIR=%CUDA_PATH_V12_4%"
if "%CUDA_ARCH%"=="" set "CUDA_ARCH=native"
if "%FFMPEG_DIR%"=="" set "FFMPEG_DIR=C:\ffmpeg\bin"

if not exist "%CUDA_DIR%\bin\nvcc.exe" (
    echo build-ai-tools: CUDA toolkit not found at "%CUDA_DIR%" - set CUDA_DIR
    exit /b 1
)
if not exist "%ROOT%\deps\whisper.cpp\CMakeLists.txt" (
    echo build-ai-tools: deps\whisper.cpp missing - run: git submodule update --init deps/whisper.cpp
    exit /b 1
)
if not exist "%ROOT%\deps\llama.cpp\CMakeLists.txt" (
    echo build-ai-tools: deps\llama.cpp missing - run: git submodule update --init deps/llama.cpp
    exit /b 1
)

rem --- Visual Studio environment ------------------------------------------
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo build-ai-tools: vswhere.exe not found - is Visual Studio 2022 installed?
    exit /b 1
)
set "VSDIR="
for /f "usebackq delims=" %%I in (`"%VSWHERE%" -version [17.0^,18.0^) -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSDIR=%%I"
if "%VSDIR%"=="" (
    echo build-ai-tools: Visual Studio 2022 with the C++ workload not found
    exit /b 1
)

rem Called from mingw32-make the PATH starts with MinGW; vcvars puts MSVC first.
call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1

set "CUDA_PATH=%CUDA_DIR%"
set "PATH=%CUDA_DIR%\bin;%PATH%"

rem Prefer the CMake/Ninja bundled with Visual Studio, which vcvars put on PATH.
where cmake >nul 2>&1 || (
    echo build-ai-tools: cmake not found
    exit /b 1
)
where ninja >nul 2>&1 || (
    echo build-ai-tools: ninja not found
    exit /b 1
)

set CMAKE_COMMON=-G Ninja -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl ^
    "-DCMAKE_CUDA_COMPILER=%CUDA_DIR%\bin\nvcc.exe" ^
    "-DCUDAToolkit_ROOT=%CUDA_DIR%" ^
    -DCMAKE_CUDA_ARCHITECTURES=%CUDA_ARCH% ^
    -DBUILD_SHARED_LIBS=OFF -DGGML_CUDA=ON -DGGML_NATIVE=ON

rem --- whisper.cpp ----------------------------------------------------------
rem Fixes not yet upstream, from scripts\patches; skipped once applied.
for %%P in ("%ROOT%\scripts\patches\whisper.cpp-*.patch") do (
    git -C "%ROOT%\deps\whisper.cpp" apply --reverse --check "%%~fP" >nul 2>&1 || (
        echo build-ai-tools: applying %%~nxP
        git -C "%ROOT%\deps\whisper.cpp" apply "%%~fP" || exit /b 1
    )
)

echo build-ai-tools: configuring whisper.cpp
cmake -S "%ROOT%\deps\whisper.cpp" -B "%BUILD%\whisper.cpp" %CMAKE_COMMON% ^
    -DWHISPER_BUILD_TESTS=OFF -DWHISPER_BUILD_SERVER=OFF -DWHISPER_SDL2=OFF ^
    -DWHISPER_CURL=OFF || exit /b 1
echo build-ai-tools: building whisper-cli
cmake --build "%BUILD%\whisper.cpp" --target whisper-cli || exit /b 1

rem --- llama.cpp ------------------------------------------------------------
echo build-ai-tools: configuring llama.cpp
cmake -S "%ROOT%\deps\llama.cpp" -B "%BUILD%\llama.cpp" %CMAKE_COMMON% ^
    -DLLAMA_BUILD_TESTS=OFF -DLLAMA_BUILD_EXAMPLES=OFF -DLLAMA_BUILD_APP=OFF ^
    -DLLAMA_BUILD_SERVER=ON -DLLAMA_BUILD_UI=OFF -DLLAMA_USE_PREBUILT_UI=OFF ^
    -DLLAMA_OPENSSL=OFF || exit /b 1
echo build-ai-tools: building llama-server
cmake --build "%BUILD%\llama.cpp" --target llama-server || exit /b 1

rem --- collect ----------------------------------------------------------------
if not exist "%OUT%" mkdir "%OUT%" || exit /b 1
echo build-ai-tools: copying binaries to %OUT%

call :copyfirst whisper-cli.exe "%BUILD%\whisper.cpp\bin" || exit /b 1
call :copyfirst llama-server.exe "%BUILD%\llama.cpp\bin" || exit /b 1

rem CUDA runtime and cuBLAS, used by both tools
for %%F in (cudart64_12.dll cublas64_12.dll cublasLt64_12.dll) do (
    call :copyfirst %%F "%CUDA_DIR%\bin" || exit /b 1
)

rem MSVC runtime, so the tools also run where Visual Studio is not installed
set "CRTDIR="
for /d %%D in ("%VCToolsRedistDir%x64\Microsoft.VC*.CRT") do set "CRTDIR=%%~fD"
if "%CRTDIR%"=="" (
    echo build-ai-tools: MSVC runtime redistributables not found
    exit /b 1
)
for %%F in (msvcp140.dll msvcp140_1.dll vcruntime140.dll vcruntime140_1.dll) do (
    call :copyfirst %%F "%CRTDIR%" || exit /b 1
)

if exist "%FFMPEG_DIR%\ffmpeg.exe" (
    call :copyfirst ffmpeg.exe "%FFMPEG_DIR%" || exit /b 1
) else (
    echo build-ai-tools: WARNING: ffmpeg.exe not found in "%FFMPEG_DIR%" - set FFMPEG_DIR
)

if not exist "%OUT%\models" mkdir "%OUT%\models"
echo build-ai-tools: done. Models: powershell -ExecutionPolicy Bypass -File scripts\download-ai-models.ps1
exit /b 0

rem copyfirst <file> <dir>: copy <dir>\<file> to OUT, only when it changed
:copyfirst
if not exist "%~2\%~1" (
    echo build-ai-tools: %~1 not found in %~2
    exit /b 1
)
xcopy /y /d /q "%~2\%~1" "%OUT%\" >nul || exit /b 1
exit /b 0
