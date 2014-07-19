#!/bin/bash

passed_arguments=$@
make_jobs=$(($(nproc) + 1))

function execute_command {
    command="cmake .. $arguments && make clean && make -j $make_jobs"
    echo $command
    eval $command
    if [ ! $? -eq 0 ]; then
        echo "Failed arguments:"
        echo "  $arguments"
        exit 1
    fi
    if [ -z "$NO_TESTS" ]; then
        echo "make test"
        eval "make test"
        if [ ! $? -eq 0 ]; then
            echo "Failed arguments:"
            echo "  $arguments"
            exit 2
        fi
    fi
}

function test {
    current=0
    for test_feature in $test_features; do
        progress=$(printf "test \"%s\", combination %u / %u, progress %05.2f%%\n" $test_name $(($current + 1)) $test_count $(awk "BEGIN{print $current / $test_count * 100}"))
        echo -ne "\033]2;$progress\007"
        current=$(($current + 1))

        arguments="$passed_arguments "
        $test_callback
        arguments+="-DTRALLOC_MAN=0 -DTRALLOC_SHARED=0 -DTRALLOC_COLLECT_SOURCES=1"
        
        execute_command
    done
}

function make_extensions_arguments {
    if [ "${test_feature:0:1}" == "1" ]; then
        arguments+="-DTRALLOC_THREADS=1 "
    else
        arguments+="-DTRALLOC_THREADS=0 "
    fi
    if [ "${test_feature:1:1}" == "1" ]; then
        arguments+="-DTRALLOC_LENGTH=1 "
    else
        arguments+="-DTRALLOC_LENGTH=0 "
    fi
    if [ "${test_feature:2:1}" == "1" ]; then
        arguments+="-DTRALLOC_DESTRUCTORS=1 "
    else
        arguments+="-DTRALLOC_DESTRUCTORS=0 "
    fi
    if [ "${test_feature:3:1}" == "1" ]; then
        arguments+="-DTRALLOC_REFERENCES=1 "
    else
        arguments+="-DTRALLOC_REFERENCES=0 "
    fi
    if [ "${test_feature:4:1}" == "1" ]; then
        arguments+="-DTRALLOC_POOL=1 "
    else
        arguments+="-DTRALLOC_POOL=0 "
    fi
    if [ "${test_feature:5:1}" == "1" ]; then
        arguments+="-DTRALLOC_UTILS_BUFFER=1 "
    else
        arguments+="-DTRALLOC_UTILS_BUFFER=0 "
    fi
}
function test_extensions {
    test_name="extensions"
    test_count=$((2 ** 6))
    test_features=$(echo {0..1}{0..1}{0..1}{0..1}{0..1}{0..1})
    test_callback=make_extensions_arguments
    test
}

function make_debug_arguments {
    if [ "${test_feature:0:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_THREADS=1 "
    else
        arguments+="-DTRALLOC_DEBUG_THREADS=0 "
    fi
    if [ "${test_feature:1:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_CALLBACKS=1 "
    else
        arguments+="-DTRALLOC_DEBUG_CALLBACKS=0 "
    fi
    if [ "${test_feature:2:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_STATS=1 "
    else
        arguments+="-DTRALLOC_DEBUG_STATS=0 "
    fi
    if [ "${test_feature:3:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_LOG=1 "
    else
        arguments+="-DTRALLOC_DEBUG_LOG=0 "
    fi
}
function test_debug {
    test_name="debug"
    test_count=$((2 ** 4))
    test_features=$(echo {0..1}{0..1}{0..1}{0..1})
    test_callback=make_debug_arguments
    test
}

function make_locks_arguments {
    if [ "${test_feature:0:1}" == "2" ]; then
        arguments+="-DTRALLOC_SUBTREE_LOCK_TYPE=\"rwlock\" "
    elif [ "${test_feature:0:1}" == "1" ]; then
        arguments+="-DTRALLOC_SUBTREE_LOCK_TYPE=\"mutex\" "
    else
        arguments+="-DTRALLOC_SUBTREE_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${test_feature:1:1}" == "2" ]; then
        arguments+="-DTRALLOC_CHILDREN_LOCK_TYPE=\"rwlock\" "
    elif [ "${test_feature:1:1}" == "1" ]; then
        arguments+="-DTRALLOC_CHILDREN_LOCK_TYPE=\"mutex\" "
    else
        arguments+="-DTRALLOC_CHILDREN_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${test_feature:2:1}" == "2" ]; then
        arguments+="-DTRALLOC_DEBUG_STATS_LOCK_TYPE=\"rwlock\" "
    elif [ "${test_feature:2:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_STATS_LOCK_TYPE=\"mutex\" "
    else
        arguments+="-DTRALLOC_DEBUG_STATS_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${test_feature:3:1}" == "2" ]; then
        arguments+="-DTRALLOC_DEBUG_THREADS_LOCK_TYPE=\"rwlock\" "
    elif [ "${test_feature:3:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_THREADS_LOCK_TYPE=\"mutex\" "
    else
        arguments+="-DTRALLOC_DEBUG_THREADS_LOCK_TYPE=\"spinlock\" "
    fi
    if [ "${test_feature:4:1}" == "2" ]; then
        arguments+="-DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE=\"rwlock\" "
    elif [ "${test_feature:4:1}" == "1" ]; then
        arguments+="-DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE=\"mutex\" "
    else
        arguments+="-DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE=\"spinlock\" "
    fi
}
function test_locks {
    test_name="locks"
    test_count=$((3 ** 5))
    test_features=$(echo {0..2}{0..2}{0..2}{0..2}{0..2})
    test_callback=make_locks_arguments
    test
}

test_extensions
test_debug
test_locks

echo "Success"
