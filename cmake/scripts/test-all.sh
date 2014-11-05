#!/bin/sh

current_dir=$(readlink -f "$(dirname $0)")

commands=()
for version in 3.3 3.4 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9; do
    commands+=("NO_TESTS=1 $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/$version/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1")
done

function join {
    local IFS="$1";
    shift;
    echo "$*";
}

commands=$(printf " && %s" "${commands[@]}")
commands=${commands:4}
eval "nice -n 19 ionice -c2 -n7 sh -c \"$commands\""

# $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/tcc/default.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1 &&
# 
# $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/default.cmake\"           -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1 &&
# $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/ccc-analyzer.cmake\"      -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1 &&
# $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/leak-sanitizer.cmake\"    -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1 &&
# $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/address-sanitizer.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1 &&
# $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/clang/thread-sanitizer.cmake\"  -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1 &&
# 
# NO_TESTS=1 $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/gcc/4.9/avr.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE_EMBED\"
