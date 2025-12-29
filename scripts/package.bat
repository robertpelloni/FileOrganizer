@echo off
setlocal enabledelayedexpansion

cd /d "%~dp0\.."

:: Read version from VERSION.md
set /p VERSION=<VERSION.md
set VERSION=%VERSION: =%

echo ========================================
echo FileOrganizer Packaging Script v%VERSION%
echo ========================================

:: Clean previous builds
echo [1/5] Cleaning previous builds...
if exist dist rmdir /s /q dist
if exist build rmdir /s /q build

:: Create build directory and run cmake
echo [2/5] Configuring CMake (Release)...
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    echo ERROR: CMake configuration failed
    exit /b 1
)

:: Build the project
echo [3/5] Building project...
cmake --build build --config Release
if errorlevel 1 (
    echo ERROR: Build failed
    exit /b 1
)

:: Create dist directory structure
echo [4/5] Creating distribution...
mkdir dist
mkdir dist\docs

:: Copy essential files
copy build\cli\fo_cli.exe dist\
copy README.md dist\
copy LICENSE.txt dist\
if exist docs\USER_MANUAL.md copy docs\USER_MANUAL.md dist\docs\

:: Verify files exist
if not exist dist\fo_cli.exe (
    echo ERROR: fo_cli.exe not found in build output
    exit /b 1
)

:: Create ZIP archive
echo [5/5] Creating ZIP archive...
set ZIPNAME=FileOrganizer-%VERSION%-win64.zip
powershell -Command "Compress-Archive -Path 'dist\*' -DestinationPath '%ZIPNAME%' -Force"
if errorlevel 1 (
    echo ERROR: Failed to create ZIP archive
    exit /b 1
)

echo ========================================
echo SUCCESS: Created %ZIPNAME%
echo ========================================
dir %ZIPNAME%

endlocal
