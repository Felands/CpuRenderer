param(
    [switch] $Clean,
    [switch] $Build,
    [switch] $Run
)

$ErrorActionPreference = "Stop"

$cmake = Get-Command cmake
$vcpkgRoot = $env:VCPKG_ROOT
$vcpkgToolchain = Join-Path $vcpkgRoot "scripts\buildsystems\vcpkg.cmake"
$scriptDirectory = Split-Path -Parent $MyInvocation.MyCommand.Path
$sourceDirectory = [System.IO.Path]::GetFullPath((Join-Path $scriptDirectory ".."))
$manifestDirectory = $sourceDirectory
$manifestPath = Join-Path $manifestDirectory "vcpkg.json"
$buildDirectory = Join-Path $sourceDirectory "build"

if ($Clean) {
    if (Test-Path $buildDirectory) {
        Write-Host "Cleaning build directory..." -ForegroundColor DarkYellow
        Remove-Item -LiteralPath $buildDirectory -Recurse -Force
    }
}

if (-not (Test-Path $buildDirectory -PathType Container)) {
    Write-Host "Creating build directory: $buildDirectory..." -ForegroundColor DarkYellow
    New-Item -ItemType Directory -Path $buildDirectory -Force | Out-Null
}

if ($Build) {
    Write-Host "Configuring project..." -ForegroundColor DarkYellow
    & $cmake.Source -S $sourceDirectory -B $buildDirectory `
        "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchain" `
        "-DVCPKG_MANIFEST_DIR=$manifestDirectory"
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed with exit code $LASTEXITCODE."
    }

    Write-Host "Building project..." -ForegroundColor DarkYellow
    & $cmake.Source --build $buildDirectory --parallel
    if ($LASTEXITCODE -ne 0) {
        throw "CMake build failed with exit code $LASTEXITCODE."
    }

    Write-Host "Build completed successfully." -ForegroundColor DarkGreen
}

if ($Run) {
    Write-Host "Running demo..." -ForegroundColor DarkYellow
    $executable = Join-Path $buildDirectory "Debug\CpuDemo.exe"
    Push-Location (Split-Path -Parent $executable)
    try {
        & $executable
        if ($LASTEXITCODE -ne 0) {
            throw "Demo failed with exit code $LASTEXITCODE."
        }
    } finally {
        Pop-Location
    }
    Write-Host "Running demo successfully." -ForegroundColor DarkGreen
}