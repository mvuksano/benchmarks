#!/bin/bash

# Detect the operating system
OS=$(uname)

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
  # Change to the benchmark directory
  cd "$(dirname "$0")/benchmark"

  # Create the build directory
  cmake -E make_directory "build"

  # Change to the build directory, run cmake and go back to the benchmark directory
  cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../

  # Build the project with the specified configuration
  cmake --build "build" --config Release ${OPTS:+--} ${OPTS}

  # Change back to the project root directory
  cd ..

  # Build root project
  cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
  cmake --build build
}

# Function to clean the build directory
clean_build() {
  # Change to the benchmark directory
  cd "$(dirname "$0")/benchmark"

  # Remove the build directory
  rm -rf build

  # Change back to the project root directory
  cd ..
}

# Function to fetch all git submodules (dependecies)
init_deps() {
  git submodule update --init --recursive
}

# Check the input option and call the corresponding function
case "$1" in
  --clean)
    clean_build
    ;;
  --init)
    build_project
    ;;
  --init-deps)
    init_deps
    ;;
  *)
    echo "Usage: $0 --clean | --init | --init-deps"
    echo "  --clean       Clean the build directory"
    echo "  --init        Initialize and build the project"
    echo "  --init-deps   Fetch dependencies"
    exit 1
    ;;
esac

