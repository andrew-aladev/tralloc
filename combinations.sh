#!/bin/bash

make_jobs=$(($(nproc) + 1))
feature_combinations=$(echo {0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1}{0..1})
for feature in $feature_combinations; do
    if [ "${feature:0:1}" == "1" ]; then
        arguments="-DTRALLOC_THREADS=1 "
    else
        arguments="-DTRALLOC_THREADS=0 "
    fi
    if [ "${feature:1:1}" == "1" ]; then
        arguments+="-DTRALLOC_LENGTH=1 "
    else
        arguments+="-DTRALLOC_LENGTH=0 "
    fi
    if [ "${feature:2:1}" == "1" ]; then
        arguments+="-DTRALLOC_DESTRUCTOR=1 "
    else
        arguments+="-DTRALLOC_DESTRUCTOR=0 "
    fi
    if [ "${feature:3:1}" == "1" ]; then
        arguments+="-DTRALLOC_REFERENCE=1 "
    else
        arguments+="-DTRALLOC_REFERENCE=0 "
    fi
    if [ "${feature:4:1}" == "1" ]; then
        arguments+="-DTRALLOC_POOL=1 "
    else
        arguments+="-DTRALLOC_POOL=0 "
    fi
    if [ "${feature:5:1}" == "1" ]; then
        arguments+="-DTRALLOC_UTILS_BUFFER=1 "
    else
        arguments+="-DTRALLOC_UTILS_BUFFER=0 "
    fi
    
    if [ "${feature:6:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_THREADS=1 "
    else
        arguments+="-DTRALLOC_DEBUG_THREADS=0 "
    fi
    if [ "${feature:7:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_CALLBACKS=1 "
    else
        arguments+="-DTRALLOC_DEBUG_CALLBACKS=0 "
    fi
    if [ "${feature:8:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_STATS=1 "
    else
        arguments+="-DTRALLOC_DEBUG_STATS=0 "
    fi
    
    if [ "${feature:9:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_LOG=1 "
    else
        arguments+="-DTRALLOC_DEBUG_LOG=0 "
    fi
    if [ "${feature:10:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_THREADS_LENGTH='spinlock' "
    else
        arguments+="-DTRALLOC_DEBUG_THREADS_LENGTH='mutex' "
    fi
    if [ "${feature:11:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_THREADS_CHILDREN='spinlock' "
    else
        arguments+="-DTRALLOC_DEBUG_THREADS_CHILDREN='mutex' "
    fi
    
    arguments+="-DTRALLOC_MAN=0 $@"
    
    command="cmake .. $arguments && make clean && make -j $make_jobs"
    echo $command
    eval $command
    if [ ! $? -eq 0 ]; then
        echo "Failed arguments:"
        echo "  $arguments"
        exit 1
    fi
    if [ -z "$NO_TESTS" ]; then
        eval "make test"
        if [ ! $? -eq 0 ]; then
            echo "Failed arguments:"
            echo "  $arguments"
            exit 2
        fi
    fi
done

echo "Success"
