#!/bin/bash

set -xev 

# Detect the operating system
OS=$(uname)
PROJECT_DIR=$(realpath $(dirname "$0"))
LIB_INSTALL_DIR=$(realpath $(dirname "$0"))/installed
CPP_COMPILER=$(which clang++ 2>/dev/null)
if [ -z $CPP_COMPILER ]; then
  # Check here for latest version instead of just presence of compiler.
  echo "Error: clang++ compiler not found."
  exit 1
fi

OPTS=""

if [ "$OS" = "Darwin" ]; then
  OPTS="-j$(sysctl -n hw.logicalcpu)"
elif [ "$OS" = "Linux" ]; then
  OPTS="-j$(nproc)"
else
  echo "Platform not supported."
  exit 1
fi

# Function to build the project
build_project() {
  # Build root project
  cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_PREFIX_PATH=$LIB_INSTALL_DIR ${CPP_COMPILER:+-DCMAKE_CXX_COMPILER=$CPP_COMPILER} 
  cmake --build build
}

# Function to clean the build directory
clean_build() {
  rm -rf "$(dirname "$0")/benchmark/build"
  rm -rf "$(dirname "$0")/gflags/_build"
  rm -rf "$(dirname "$0")/installed"
}

# Function to fetch all git submodules (dependecies)
init_deps() {
  git submodule update --init --recursive
}

build_deps() {
  mkdir -p $LIB_INSTALL_DIR

  # Change to the benchmark directory
  cd "$(dirname "$0")/benchmark"
  # Create the build directory
  cmake -E make_directory "build"
  # Change to the build directory, run cmake and go back to the benchmark directory
  cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../ -DCMAKE_INSTALL_PREFIX=$LIB_INSTALL_DIR ${CPP_COMPILER:+-DCMAKE_CXX_COMPILER=$CPP_COMPILER} 
  # Build the project with the specified configuration
  cmake --build "build" --config Release ${OPTS:+--} ${OPTS}
  # Install benchmark
  cmake --build "build" --config Release --target install ${OPTS:+--} ${OPTS}
  # Change back to the project root directory
  cd ..

  # Change to gflags directory
  cd "$(dirname "$0")/gflags"
  cmake -B _build -S . -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$LIB_INSTALL_DIR ${CPP_COMPILER:+-DCMAKE_CXX_COMPILER=$CPP_COMPILER} 
  cmake --build _build
  cd _build
  make install

  # Change to glog directory
  cd "$PROJECT_DIR/glog"
  cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$LIB_INSTALL_DIR ${CPP_COMPILER:+-DCMAKE_CXX_COMPILER=$CPP_COMPILER}
  cmake --build build
  cmake --build build --target install
}

# Check the input option and call the corresponding function
case "$1" in
  --clean)
    clean_build
    ;;
  --build-main)
    build_project
    ;;
  --init-deps)
    init_deps
    ;;
  --build-deps)
    build_deps
    ;;
  *)
    echo "Usage: $0 --clean | --build-main | --init-deps | --build-deps"
    echo "  --clean       Clean the build directory"
    echo "  --build-main  Build main projects"
    echo "  --init-deps   Fetch dependencies"
    echo "  --build-deps  Build dependencies"
    exit 1
    ;;
esac

