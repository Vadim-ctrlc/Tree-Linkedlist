#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/.."
cd "$PROJECT_DIR"

echo "=== Building SFML 2.6.x for Linux ==="

# Clone SFML if not exists
if [ ! -d "SFML" ]; then
    echo "Cloning SFML repository..."
    git clone https://github.com/SFML/SFML.git
    cd SFML
    git checkout 2.6.x
    cd ..
else
    echo "Updating SFML..."
    cd SFML
    git pull
    git checkout 2.6.x
    cd ..
fi

# Create build directory
mkdir -p SFML/build
cd SFML/build

echo "Configuring SFML..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PROJECT_DIR/sfml_installed" \
    -DSFML_BUILD_EXAMPLES=OFF \
    -DSFML_BUILD_DOC=OFF \
    -DBUILD_SHARED_LIBS=ON

echo "Building SFML..."
make -j$(nproc)

echo "Installing SFML..."
make install

echo ""
echo "=== SFML built successfully! ==="