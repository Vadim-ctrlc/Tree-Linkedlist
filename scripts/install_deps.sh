#!/bin/bash

echo "Installing SFML dependencies for Ubuntu/Debian..."

sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    git \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libfreetype6-dev \
    libopenal-dev \
    libflac-dev \
    libvorbis-dev \
    libudev-dev

echo ""
echo "Dependencies installed successfully!"