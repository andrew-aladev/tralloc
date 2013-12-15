Hierarchical memory allocator Talloc2
=====================================

Talloc2 is a hierarchical memory allocator with some extensions. It is an alternative implementation of simplified talloc api.

The main goal of the project is to minimize memory overhead.
Size of usual chunk is 32-49 bytes, size of reference chunk is 57-65 bytes on 64 bit system (depends on compiled features).
Debug features will increase size of each chunk by 16 bytes.


Supported platforms
-------------------

GNU/Linux, GNU/Hurd.


Debug build
-----------

mkdir build && cd build
cmake .. -DTALLOC_EXAMPLES=ON && make test


Documentation
-------------
Please read `man`_


Gentoo
------

Copy ebuilds from provided "gentoo" folder or add `overlay`_.


DEB and RPM
-----------

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE && make test && make package


LICENSE
-------
LGPL v3+


.. _man:     https://github.com/andrew-aladev/talloc2/blob/master/man/talloc2.txt
.. _overlay: https://github.com/andrew-aladev/puchuu-overlay