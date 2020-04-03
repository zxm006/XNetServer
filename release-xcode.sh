#!/bin/bash


# Build program with release mode.
BUILD_DIR=build-xcode
#rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -G "Xcode" ..
cd ..
exit 0
