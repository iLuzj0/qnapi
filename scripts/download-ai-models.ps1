# Downloads the models used by `qnapic --ai-fallback` into win32\out\models.
# About 8.4 GB in total; interrupted downloads resume where they stopped.
#
#   powershell -ExecutionPolicy Bypass -File scripts\download-ai-models.ps1 [-OutDir <dir>]

param(
    [string]$OutDir = (Join-Path $PSScriptRoot "..\win32\out\models")
)

$ErrorActionPreference = "Stop"

$models = @(
    @{
        # speech recognition, used to transcribe the audio track
        File = "ggml-large-v3-turbo.bin"
        Url  = "https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-large-v3-turbo.bin"
        Size = 1624555275
    },
    @{
        # Polish-trained LLM, used to translate the transcription
        File = "Bielik-11B-v3.0-Instruct.Q4_K_M.gguf"
        Url  = "https://huggingface.co/speakleash/Bielik-11B-v3.0-Instruct-GGUF/resolve/main/Bielik-11B-v3.0-Instruct.Q4_K_M.gguf"
        Size = 6724051584
    }
)

New-Item -ItemType Directory -Force $OutDir | Out-Null
$OutDir = (Resolve-Path $OutDir).Path

foreach ($m in $models) {
    $target = Join-Path $OutDir $m.File
    $partial = "$target.part"

    if (Test-Path $target) {
        Write-Host "$($m.File): already present"
        continue
    }

    Write-Host "$($m.File): downloading to $OutDir"
    # curl.exe ships with Windows 10+; -C - resumes a previous partial download
    & curl.exe -L --fail --retry 5 --retry-delay 5 -C - -o $partial $m.Url
    if ($LASTEXITCODE -ne 0) {
        throw "Download of $($m.File) failed (curl exit code $LASTEXITCODE)"
    }

    if ($m.Size -gt 0 -and (Get-Item $partial).Length -ne $m.Size) {
        throw "$($m.File): unexpected size $((Get-Item $partial).Length), expected $($m.Size)"
    }

    Move-Item $partial $target
    Write-Host "$($m.File): done"
}
