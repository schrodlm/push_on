#!/bin/bash

# Simple build script for the roguelike game

set -e

BUILD_DIR="build"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Run CMake
cmake ..

# Build
make -j$(nproc)

echo ""
echo "Build complete! Run with: ./bin/roguelike_game"
