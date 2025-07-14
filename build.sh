#!/bin/bash

# Layout Converter Build Script
# Builds the C++ core library and CLI tool

set -e

echo "Building Layout Converter..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

echo "Build completed successfully!"
echo "CLI tool location: build/bin/layout_converter"
echo "Library location: build/lib/liblayout_converter_core.so" 