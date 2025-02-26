#!/bin/bash

opt=$1

mkdir -p build && cd build

# CMAKE_X5H_DIR=/path-to-cmake-dir
cmake=$CMAKE_X5H_DIR/bin/cmake

$cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../toolchain_arm_none_eabi.cmake ..

if [[ "$opt" = "clean" ]]; then
	make clean
fi

make -j8

cd -
