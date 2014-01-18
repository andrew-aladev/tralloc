Hierarchical memory allocator Tralloc
=====================================

Tralloc (tree alloc) is a hierarchical memory allocator with extensions. It is an alternative implementation of simplified `talloc`_ api.

The main goal of the project is to minimize memory overhead.
Size of chunk is 32-88 bytes on x86_64 system (depends on compiled and runtime features).
Debug features will increase size of each chunk by 16 bytes.


TODO
----
Thread safety, pool.


Supported platforms
-------------------

GNU/Linux, GNU/Hurd.


Debug build
-----------

    ::

     $ mkdir build && cd build
     $ cmake ..
     $ make && make test


Extensions and features enable/disable
-------------------------
    
    ::
    
     $ cmake .. \
        -DTRALLOC_DEBUG=OFF        \
        -DTRALLOC_LENGTH=OFF       \
        -DTRALLOC_DESTRUCTOR=OFF   \
        -DTRALLOC_REFERENCE=OFF    \
        -DTRALLOC_POOL=OFF         \
        -DTRALLOC_UTILS_BUFFER=OFF \
        -DTRALLOC_SHARED=OFF       \
        -DTRALLOC_STATIC=OFF       \
        -DTRALLOC_MAN=OFF          \
        -DTRALLOC_TEST=OFF         \
        -DTRALLOC_EXAMPLES=OFF


Documentation
-------------
Please read `man`_.


Other compilers
---------
    
    ::
    
     $ CC="clang" cmake .. -DCMAKE_C_FLAGS="-O2"


Gentoo
------

Copy ebuilds from provided "gentoo" folder or add `overlay`_.


DEB and RPM
-----------

    ::
    
     $ cmake .. -DCMAKE_BUILD_TYPE=RELEASE
     $ make && make test && make package


LICENSE
-------
LGPL v3+


.. _talloc:  http://talloc.samba.org/talloc/doc/html/group__talloc.html
.. _man:     https://github.com/andrew-aladev/tralloc/blob/master/man/tralloc.txt
.. _overlay: https://github.com/andrew-aladev/puchuu-overlay
