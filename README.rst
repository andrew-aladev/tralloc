Hierarchical memory allocator Tralloc
=====================================

Tralloc (tree alloc) is a hierarchical memory allocator with extensions. It is an alternative implementation of simplified `talloc`_ api.

The main goals of the project are to minimize memory overhead and provide thread safety.


WIP
---
Thread safety.


Supported platforms
-------------------

Posix systems are supported.


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
        -DTRALLOC_THREADS=0/1      \
        -DTRALLOC_LENGTH=0/1       \
        -DTRALLOC_DESTRUCTORS=0/1  \
        -DTRALLOC_REFERENCES=0/1   \
        -DTRALLOC_POOL=0/1         \
        -DTRALLOC_UTILS_BUFFER=0/1 \
        \
        -DTRALLOC_DEBUG_THREADS=0/1   \
        -DTRALLOC_DEBUG_CALLBACKS=0/1 \
        -DTRALLOC_DEBUG_STATS=0/1     \
        -DTRALLOC_DEBUG_LOG=0/1       \
        \
        -DTRALLOC_SUBTREE_LOCK_TYPE="rwlock/mutex/spinlock"         \
        -DTRALLOC_CHILDREN_LOCK_TYPE="rwlock/mutex/spinlock"        \
        -DTRALLOC_DEBUG_STATS_LOCK_TYPE="rwlock/mutex/spinlock"     \
        -DTRALLOC_DEBUG_THREADS_LOCK_TYPE="rwlock/mutex/spinlock"   \
        -DTRALLOC_DEBUG_CALLBACKS_LOCK_TYPE="rwlock/mutex/spinlock" \
        \
        -DTRALLOC_SHARED=0/1   \
        -DTRALLOC_STATIC=0/1   \
        -DTRALLOC_MAN=0/1      \
        -DTRALLOC_TESTS=0/1    \
        -DTRALLOC_EXAMPLES=0/1 \
        \
        -DTRALLOC_COLLECT_SOURCES=0/1

You can test all possible combinations of extensions and features

    ::
    
     $ NO_TESTS=1 ../combinations.sh
     $ ALL_COMBINATIONS=1 ../combinations.sh
     $ ../combinations.sh -DCMAKE_BUILD_TYPE="DEBUG"
     $ ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"


Multilib
--------

    ::
    
     $ CFLAGS="-m32" ../combinations.sh -DCMAKE_BUILD_TYPE="RELEASE"
     
     
Supported compilers
---------
    
    Checkout `test_all_`.

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


Notes about implementation
--------------------------

1. `inline workaround`_


Related bugs
------------

1. `gcc maybe uninitialized`


Language
--------
My English is not perfect, my native language is Belarusian. Feel free to ask me about meaning of any comment in code or sentence in documentation.


.. _talloc:                  http://talloc.samba.org/talloc/doc/html/group__talloc.html
.. _man:                     https://github.com/andrew-aladev/tralloc/blob/master/man/tralloc.txt
.. _overlay:                 https://github.com/andrew-aladev/puchuu-overlay
.. _gentoo:                  https://github.com/andrew-aladev/tralloc/tree/master/gentoo
.. _format_sources:          https://github.com/andrew-aladev/tralloc/blob/master/format_sources
.. _test_all:                https://github.com/andrew-aladev/tralloc/blob/threads/test-all.sh
.. _inline workaround:       https://www.puchuu.com/posts/inline-workaround.html
.. _gcc maybe uninitialized: https://www.puchuu.com/posts/inline-workaround.html
