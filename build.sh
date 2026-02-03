#!/usr/bin/env bash

set -e

command -v g++ >/dev/null || { echo "g++ not found"; exit 1; }
command -v root-config >/dev/null || { echo "root-config not found (source thisroot.sh)"; exit 1; }
command -v pkg-config >/dev/null || { echo "pkg-config not found"; exit 1; }

CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra -O2"

GTKMM_FLAGS="$(pkg-config --cflags --libs gtkmm-3.0)"
ROOT_FLAGS="$(root-config --cflags --libs)"
ROOT_LIBDIR="$(root-config --libdir)"

SOURCES="
	main.cpp
	latex-equation.cpp
	root-renderer.cpp
	image-processor.cpp
"

echo "Compiling latex-equation..."
$CXX $CXXFLAGS \
     $SOURCES \
     -o latex-equation \
     $GTKMM_FLAGS \
     $ROOT_FLAGS \
     -Wl,-rpath,$ROOT_LIBDIR
echo "Build sucessful."
