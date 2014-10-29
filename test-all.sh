#!/bin/bash

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/3.3/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/3.4/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.0/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.1/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.2/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.3/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.4/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.5/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.6/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.7/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.8/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.9/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/tcc/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/default.cmake           -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/ccc-analyzer.cmake      -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/leak-sanitizer.cmake    -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/address-sanitizer.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/thread-sanitizer.cmake  -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

NO_TESTS=1 ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.9/avr.cmake -DCMAKE_BUILD_TYPE="RELEASE_EMBED" && rm -r *
