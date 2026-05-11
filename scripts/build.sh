#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/.."
BUILD_DIR="$PROJECT_DIR/build_linux"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

export CMAKE_PREFIX_PATH="$PROJECT_DIR/sfml_installed:$CMAKE_PREFIX_PATH"

echo "=== Building Tree-Linkedlist (Release) ==="

echo "Configuring..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DSFML_DIR="$PROJECT_DIR/sfml_installed/lib/cmake/SFML"

echo "Building..."
make -j$(nproc)

echo "Copying assets..."
cp "$PROJECT_DIR/assets/arialmt.ttf" .

echo ""
echo "=== Build completed! ==="
echo "To run: ./run.sh"
echo ""

# Create run script
cat > run.sh << EOF
#!/bin/bash
SCRIPT_DIR="\$(cd "\$(dirname "\${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="\$SCRIPT_DIR/../sfml_installed/lib:\$LD_LIBRARY_PATH"
cd "\$SCRIPT_DIR"
exec "./Tree-Linkedlist" "\$@"
EOF

chmod +x run.sh