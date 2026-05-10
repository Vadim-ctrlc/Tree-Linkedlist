@echo off
cd /d %~dp0

echo Cloning SFML 2.6.x...
if not exist SFML (
    git clone https://github.com/SFML/SFML.git
    cd SFML
    git checkout 2.6.x
    cd ..
)

echo Building SFML...
if not exist SFML\build (
    mkdir SFML\build
)

cd SFML\build

cmake .. -G "Visual Studio 17 2022" ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_INSTALL_PREFIX="%CD%/../../sfml_installed" ^
    -DSFML_BUILD_EXAMPLES=OFF ^
    -DSFML_BUILD_DOC=OFF ^
    -DBUILD_SHARED_LIBS=ON

cmake --build . --config Release --target INSTALL

echo SFML built and installed to: %CD%\..\..\sfml_installed
pause