@echo off
cd /d %~dp0

if not exist build (
    mkdir build
)

cd build

echo Configuring...
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release

echo Building...
cmake --build . --config Release

echo.
echo Build complete! Executable is in: %CD%
pause