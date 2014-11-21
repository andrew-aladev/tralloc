#!/bin/sh

current_dir=$(readlink -f "$(dirname $0)")

toolchains=("gcc/3.3/default" "gcc/3.4/default")
toolchains+=("gcc/4.0/default" "gcc/4.1/default" "gcc/4.2/default" "gcc/4.3/default" "gcc/4.4/default" "gcc/4.5/default" "gcc/4.6/default" "gcc/4.7/default")
toolchains+=("gcc/4.8/address-sanitizer" "gcc/4.8/avr" "gcc/4.8/default" "gcc/4.8/i686" "gcc/4.8/thread-sanitizer")
toolchains+=("gcc/4.9/address-sanitizer" "gcc/4.9/default" "gcc/4.9/leak-sanitizer" "gcc/4.9/thread-sanitizer" "gcc/4.9/undefined-sanitizer")
toolchains+=("clang/address-sanitizer" "clang/ccc-analyzer" "clang/default" "clang/leak-sanitizer" "clang/thread-sanitizer" "clang/undefined-sanitizer")
toolchains+=("tcc/default")
for toolchain in "${toolchains[@]}"; do
    command=("LABEL=\"$toolchain\" NO_TESTS=1 $current_dir/combinations.sh -DCMAKE_TOOLCHAIN_FILE=\"$current_dir/../toolchains/$toolchain.cmake\" -DCMAKE_BUILD_TYPE=\"RELEASE\"")
    echo "$command"
    eval "nice -n 19 ionice -c2 -n7 sh -c \"$command\""
    if [ "$?" != "0" ]; then
        echo "Failed command:"
        echo "  $command"
        exit 1
    fi
done
