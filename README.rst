Hierarchical memory allocator Tralloc
=====================================

Tralloc (tree alloc) is a hierarchical memory allocator with some extensions. It is an alternative implementation of simplified `talloc`_ api.

The main goal of the project is to minimize memory overhead.
Size of usual chunk is 32-49 bytes, size of reference chunk is 57-65 bytes on 64 bit system (depends on compiled features).
Debug features will increase size of each chunk by 16 bytes.


TODO
----
Thread safety, pool, more helper functions.


Supported platforms
-------------------

GNU/Linux, GNU/Hurd.


Debug build
-----------

    ::

     $ mkdir build && cd build
     $ cmake .. -DTRALLOC_EXAMPLES=ON
     $ make && make test


Documentation
-------------
Please read `man`_.


Gentoo
------

Copy ebuilds from provided "gentoo" folder or add `overlay`_.


DEB and RPM
-----------

    ::
    
     $ mkdir build && cd build
     $ cmake .. -DCMAKE_BUILD_TYPE=RELEASE
     $ make && make test && make package


LICENSE
-------
LGPL v3+


.. _talloc:  http://talloc.samba.org/talloc/doc/html/group__talloc.html
.. _man:     https://github.com/andrew-aladev/tralloc/blob/master/man/tralloc.txt
.. _overlay: https://github.com/andrew-aladev/puchuu-overlay
