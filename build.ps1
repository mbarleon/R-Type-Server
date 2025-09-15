#!/usr/bin/env pwsh

$ErrorActionPreference = "Stop"

function Write-ErrorBlock([string]$msg, [string]$hint = "") {
    Write-Host "`e[1;31m[❌] ERROR:`e[0m"
    Write-Host "`t$msg"
    if ($hint -ne "") { Write-Host "`t`e[3m""$hint""`e[0m" }
    exit 84
}

function Write-Success([string]$msg) {
    Write-Host "`e[1;32m[✅] SUCCESS:`e[0m `e[3m$msg`e[0m"
}

function Write-Info([string]$msg) {
    Write-Host "`e[1;33m[🚧] RUNNING:`e[0m `e[3m$msg`e[0m"
}

function Assert-Tool([string]$tool) {
    if (-not (Get-Command $tool -ErrorAction SilentlyContinue)) {
        Write-ErrorBlock "command '$tool' not found" "please install '$tool' 🤓"
    }
}

function Ensure-Tools() {
    @('git','cmake','ninja') | ForEach-Object { Assert-Tool $_ }
    Write-Success "required tools found (git, cmake, ninja)"
}

function CpuCount() {
    try {
        $env:NUMBER_OF_PROCESSORS
    } catch {
        4
    }
}

function Configure-And-Build([string]$buildType, [string[]]$extraFlags) {
    Ensure-Tools

    Write-Info "updating external submodules..."
    git submodule update --init --recursive
    Write-Success "updated external submodules !"

    if (-not (Test-Path ./build)) { New-Item -ItemType Directory -Path ./build | Out-Null }
    Set-Location ./build

    Write-Info "configuring with CMake (Ninja, $buildType)…"
    $args = @("..","-G","Ninja","-DCMAKE_BUILD_TYPE=$buildType") + $extraFlags
    cmake @args | Out-Host

    Write-Info "building target r-type_server with Ninja…"
    ninja -j (CpuCount) r-type_server | Out-Host

    Write-Success "compiled r-type_server"
    exit 0
}

function Build-Release() {
    Configure-And-Build "Release" @()
}

function Build-Debug() {
    Configure-And-Build "Debug" @("-DZAP_SRV_ENABLE_DEBUG=ON")
}

function Run-Tests() {
    Ensure-Tools

    if (-not (Test-Path ./build)) { New-Item -ItemType Directory -Path ./build | Out-Null }
    Set-Location ./build

    Write-Info "configuring tests with CMake (Ninja, Debug)…"
    cmake ".." -G "Ninja" -DCMAKE_BUILD_TYPE=Debug | Out-Host

    Write-Info "building unit tests (zap_srv_unit_tests)…"
    ninja -j (CpuCount) zap_srv_unit_tests | Out-Host

    Set-Location ..

    $testExe = Join-Path "build" "zap_srv_unit_tests.exe"
    if (-not (Test-Path $testExe)) {
        $testExeNoExt = Join-Path "build" "zap_srv_unit_tests"
        if (Test-Path $testExeNoExt) { $testExe = $testExeNoExt }
    }

    Write-Info "running unit tests…"
    & "$testExe"
    Write-Success "unit tests succeed!"

    $coverageOut = "code_coverage.txt"
    try {
        if ($IsMacOS) {
            Write-Info "generating coverage (llvm-cov on macOS)…"
            Set-Location build
            & xcrun llvm-profdata merge -sparse zap_srv_unit_tests-*.profraw -o zap_srv_unit_tests.profdata
            & xcrun llvm-cov report ./zap_srv_unit_tests -instr-profile=zap_srv_unit_tests.profdata -ignore-filename-regex='.*/tests/.*' -enable-name-compression | Out-File -Encoding utf8 ../$coverageOut
            Set-Location ..
        } elseif (Get-Command gcovr -ErrorAction SilentlyContinue) {
            Write-Info "generating coverage (gcovr)…"
            gcovr -r . --exclude tests/ | Out-File -Encoding utf8 $coverageOut
        } else {
            Write-Info "coverage tools not found; skipping coverage."
            "Coverage not available on this runner." | Out-File -Encoding utf8 $coverageOut
        }
    } catch {
        Write-Info "coverage generation failed; continuing."
        "Coverage step failed or unavailable." | Out-File -Encoding utf8 $coverageOut
    }

    Get-Content $coverageOut | Write-Host
}

function Clean() {
    Remove-Item -Recurse -Force -ErrorAction SilentlyContinue build
    if (Test-Path external/lib-cextend/build.ps1) {
        & external/lib-cextend/build.ps1 -c
    } elseif (Test-Path external/lib-cextend/build.sh) {
        bash external/lib-cextend/build.sh -c
    }
}

function FClean() {
    Clean
    Remove-Item -Recurse -Force -ErrorAction SilentlyContinue r-type_server, unit_tests, plugins, code_coverage.txt, unit_tests-*.profraw, unit_tests.profdata, vgcore*, cmake-build-debug
    if (Test-Path libs) { Get-ChildItem libs | Remove-Item -Recurse -Force -ErrorAction SilentlyContinue }
    if (Test-Path external/lib-cextend/build.ps1) {
        & external/lib-cextend/build.ps1 -f
    } elseif (Test-Path external/lib-cextend/build.sh) {
        bash external/lib-cextend/build.sh -f
    }
}

param(
    [Alias('h')] [switch] $help,
    [Alias('d')] [switch] $debug,
    [Alias('c')] [switch] $clean,
    [Alias('f')] [switch] $fclean,
    [Alias('t')] [switch] $tests,
    [Alias('r')] [switch] $re
)

if ($help) {
@"
USAGE:
  .\build.ps1       builds r-type_server project (Ninja)

ARGUMENTS:
  .\build.ps1 [-h]        displays this message
  .\build.ps1 [-d]        debug flags compilation
  .\build.ps1 [-c]        clean the project
  .\build.ps1 [-f]        fclean the project
  .\build.ps1 [-t]        run unit tests
  .\build.ps1 [-r]        fclean then rebuild (release)
"@ | Write-Host
    exit 0
}

if ($clean)  { Clean;  exit 0 }
if ($fclean) { FClean; exit 0 }
if ($debug)  { Build-Debug }
if ($tests)  { Run-Tests; exit 0 }
if ($re)     { FClean; Build-Release; exit 0 }

Build-Release

