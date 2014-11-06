#!/bin/sh

current_dir=$(readlink -f "$(dirname $0)")

commands=()
toolchains=("gcc/3.3/default" "gcc/3.4/default")
toolchains+=("gcc/4.0/default" "gcc/4.1/default" "gcc/4.2/default" "gcc/4.3/default" "gcc/4.4/default" "gcc/4.5/default" "gcc/4.6/default" "gcc/4.7/default")
toolchains+=("gcc/4.8/default" "gcc/4.8/address-sanitizer" "gcc/4.8/thread-sanitizer")
toolchains+=("gcc/4.9/default" "gcc/4.9/avr")
toolchains+=("clang/default" "clang/ccc-analyzer" "clang/leak-sanitizer" "clang/address-sanitizer" "clang/thread-sanitizer")
toolchains+=("tcc/default")
for toolchain in "${toolchains[@]}"; do
    commands+=("NO_TESTS=1 $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/$toolchain.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\" -DCMAKE_CONFIG_VERBOSE_MAKEFILE=1 -DCMAKE_VERBOSE_MAKEFILE=1")
done

function join {
    local IFS="$1";
    shift;
    echo "$*";
}

commands=$(printf " && %s" "${commands[@]}")
commands=${commands:4}
eval "nice -n 19 ionice -c2 -n7 sh -c \"$commands\""
