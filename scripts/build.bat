@echo off
cd /d %~dp0\..

if not exist build (
    mkdir build
)

cd build

echo Configuring project...
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -DSFML_DIR="%CD%/../sfml_installed/lib/cmake/SFML"

echo Building project...
cmake --build . --config Release

echo Copying assets...
copy "..\assets\arialmt.ttf" "Release\" >nul 2>&1

echo.
echo Build completed! Executable: %CD%\Release\Tree-Linkedlist.exe
pause