#!/bin/bash

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc-3.4.6.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc-4.7.4.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc-4.8.3.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/tcc.cmake       -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang.cmake     -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/ccc-analyzer.cmake  -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang-leak.cmake    -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang-address.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang-thread.cmake  -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

NO_TESTS=1 ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/avr-gcc-4.8.3.cmake -DCMAKE_BUILD_TYPE="RELEASE_EMBED" && rm -r *
