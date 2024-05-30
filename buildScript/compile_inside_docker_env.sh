#!/bin/bash

PLATFORM=$1

SRC_DIR="../"

BUILD_DIR="build"
OUTPUT_DIR="output"

SCRIPT_PATH=$(dirname $(readlink -f $0))

echo "The target OS is: $PLATFORM"

if [ -d "$BUILD_DIR" ]; then
  rm -rf $BUILD_DIR
fi

mkdir -p "${BUILD_DIR}"

if [ -d "$OUTPUT_DIR" ]; then
  rm -rf $OUTPUT_DIR
fi

mkdir -p "${OUTPUT_DIR}"

cd "${BUILD_DIR}"

case $PLATFORM in
    linux_arm)
        echo "Running Linux arm commands"
        cmake -DARCH_TAG=linux_arm "${SRC_DIR}"
        ;;

    linux_x86)
        echo "Running Linux x86 commands"
        cmake -DARCH_TAG=linux_x86 "${SRC_DIR}"
        ;;

    *)
        echo "Invalid platform! Please specify either 'linux_arm', 'linux_x86'."
        exit 1
        ;;
esac

# cmake --build . --target cpplint

make -j5
