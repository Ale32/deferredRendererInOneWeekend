#!/usr/bin/env bash

BUILD_DIR="build"

if [ -d $BUILD_DIR ]; then
    rm -r $BUILD_DIR
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G "Visual Studio 16 2019" -A x64 ..
