Vriska
======

A simple thread and network cross-platform library written in C++.


License
=======

See the *LICENSE* file at the root of the repository.


How to use
==========

1. Clone the repository
2. Call the library CMakelists
3. Link with the resulting libraries (in lib/)

See the **Examples** section for more information.


CMake options
-------------

You can specify some options to CMake to trigger different results:

 - **CMAKE_BUILD_TYPE**: _Release_ or _Debug_, like any CMake project
 - **VRISKA_LIBRARY_TYPE**: _STATIC_ or _SHARED_, the type of library linking
 - **VRISKA_BUILD_TESTS**: _Yes_ or _No_, trigger the compilation of the tests as well


Examples
========

There is a test/example project in the directory *tests* at the root of the repository.

It shows how to properly build the project with CMake.

Moreover, it explains how to implements a basic server, a lot of the client features, the timer feature and switching endianness.


Author
======

Created by [Louis Brunner](https://github.com/Hyrrmadr). Some contributions from [Jonathan Nau](https://github.com/sadikum) and [Thibault Schueller](https://github.com/ryp).
