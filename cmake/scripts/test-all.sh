#!/bin/sh

current_dir=$(readlink -f "$(dirname $0)")

eval "nice -n 19 sh -c \"
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/3.3/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/3.4/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.0/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.1/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.2/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.3/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.4/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.5/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.6/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.7/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.8/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.9/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&

$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/tcc/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&

$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/default.cmake\"           -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/ccc-analyzer.cmake\"      -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/leak-sanitizer.cmake\"    -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/address-sanitizer.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" &&
$current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/thread-sanitizer.cmake\"  -DCMAKE_BUILD_TYPE=\"RELEASE\" &&

NO_TESTS=1 $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.9/avr.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE_EMBED\"
\""