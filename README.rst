Hierarchical memory allocator Tralloc
=====================================

Tralloc (tree alloc) is a hierarchical memory allocator with extensions. It is an alternative implementation of simplified `talloc`_ api.

The main goals of the project are to minimize memory overhead and provide thread safety.


WIP
---
Thread safety.


Supported platforms
-------------------

GNU/Linux, GNU/Hurd and microcontrollers are supported. Cygwin is not supported but it works.


Debug build
-----------

    ::

     $ mkdir build && cd build
     $ cmake ..
     $ make VERBOSE=1 && make test


Documentation
-------------
Please read `man`_.
     
     
Extensions and features enable/disable
-------------------------
    
    ::
    
     $ cmake .. \
        -DTRALLOC_THREADS=0      \
        -DTRALLOC_LENGTH=0       \
        -DTRALLOC_DESTRUCTOR=0   \
        -DTRALLOC_REFERENCE=0    \
        -DTRALLOC_POOL=0         \
        -DTRALLOC_UTILS_BUFFER=0 \
        \
        -DTRALLOC_DEBUG_THREADS=0   \
        -DTRALLOC_DEBUG_CALLBACKS=0 \
        -DTRALLOC_DEBUG_STATS=0     \
        -DTRALLOC_DEBUG_LOG=0       \
        \
        -DTRALLOC_SHARED=0   \
        -DTRALLOC_STATIC=0   \
        -DTRALLOC_MAN=0      \
        -DTRALLOC_TESTS=0    \
        -DTRALLOC_EXAMPLES=0 \
        \
        -DTRALLOC_COLLECT_SOURCES=0

You can test all possible combinations of extensions and features : debug, length, destructor, reference, pool, utils_buffer.

    ::
    
     $ ../combinations.sh -DCMAKE_BUILD_TYPE="DEBUG"
     $ ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"


Multilib
--------

    ::
    
     $ CFLAGS="-m32" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     
     
Supported compilers
---------
    
    ::

     $ CC="gcc-4.8.2" CFLAGS="-Werror -Wno-maybe-uninitialized" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     $ NO_TESTS=1 CFLAGS="-Werror -Wno-maybe-uninitialized" ../combinations.sh -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/arduino.cmake -DCMAKE_BUILD_TYPE="RELEASE"
     $ CC="tcc" CFLAGS="-Werror" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     $ CC="clang" CFLAGS="-Werror" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     $ CC="ccc-analyzer" CFLAGS="-Werror" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     $ CC="clang" CFLAGS="-Werror -fsanitize=leak -fno-omit-frame-pointer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     $ CC="clang" CFLAGS="-Werror -fsanitize=address -fno-omit-frame-pointer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     $ CC="clang" CFLAGS="-Werror -fsanitize=thread -fno-omit-frame-pointer" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"


Gentoo
------

Copy ebuilds from provided `gentoo`_ folder or add `overlay`_.


DEB and RPM
-----------

    ::
    
     $ cmake .. -DCMAKE_BUILD_TYPE=RELEASE
     $ make && make test && make package


LICENSE
-------
LGPL v3+ + SLE ( static link exception )



Reformatting Source Code
------------------------
I prefer to use command:

    ::

     $ astyle --mode=c --style=kr --pad-oper --pad-paren --pad-header original.c beautifull.c
     
Source tree includes `format_sources`_ file for Kdevelop IDE.


Language
--------
My English is not perfect, my native language is Belarusian. Feel free to ask me about meaning of any comment in code or sentence in documentation.


.. _talloc:         http://talloc.samba.org/talloc/doc/html/group__talloc.html
.. _man:            https://github.com/andrew-aladev/tralloc/blob/master/man/tralloc.txt
.. _overlay:        https://github.com/andrew-aladev/puchuu-overlay
.. _gentoo:         https://github.com/andrew-aladev/tralloc/tree/master/gentoo
.. _format_sources: https://github.com/andrew-aladev/tralloc/blob/master/format_sources
