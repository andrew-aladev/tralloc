#!/bin/bash

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/3.3/default-3.3.6.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/3.4/default-3.4.6.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.0/default-4.0.4.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.1/default-4.1.2.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.2/default-4.2.4.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.3/default-4.3.6.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.4/default-4.4.7.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.5/default-4.5.4.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.6/default-4.6.4.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.7/default-4.7.4.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.8/default-4.8.3.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.9/default-4.9.1.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/tcc/default.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/default.cmake           -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/ccc-analyzer.cmake      -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/leak-sanitizer.cmake    -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/address-sanitizer.cmake -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&
../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/clang/thread-sanitizer.cmake  -DCMAKE_BUILD_TYPE="RELEASE" && rm -r * &&

NO_TESTS=1 ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/gcc/4.9/avr-4.9.1.cmake -DCMAKE_BUILD_TYPE="RELEASE_EMBED" && rm -r *
