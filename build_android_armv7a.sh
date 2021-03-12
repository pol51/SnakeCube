#!/bin/bash

export ANDROID_SDK=$HOME/Android/Sdk
export ANDROID_NDK=$HOME/Android/Sdk/ndk/16.1.4479499
export QT_ROOT=$HOME/Qt/5.6.3/android_armv7

rm -rf build
mkdir build

cmake \
  -S . -B build \
  -DANDROID_SDK_ROOT=$ANDROID_SDK \
  -DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a \
  -DCMAKE_ANDROID_STL_TYPE=gnustl_shared \
  -DCMAKE_PREFIX_PATH=$QT_ROOT \
  -DCMAKE_SYSTEM_NAME=Android \
  -DCMAKE_SYSTEM_VERSION=27 \
  -DQT_QMAKE_EXECUTABLE=$QT_ROOT/bin/qmake

cmake --build build -t snake_cube.apk

