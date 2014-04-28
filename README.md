Vriska
======

A simple thread and network cross-plateform library written in C++.


License
=======

See the *LICENSE* file at the root of the repository.


How to use
==========

Linux
-----

1. Clone the repository
2. Add a rule in your Makefile/CMake/whatever to call the library Makefile (without any rule)
3. Link with the resulting libraries

See the **Example** section for more information.

Windows
-------

1. Clone the repository
2. Add a dependency in Visual Studio on all the libraries to your project

See the **Example** section for more information.


Example
=======

There is a test/example project in the directory *tests* at the root of the repository.

It shows how to properly build on Linux (Makefile) and Windows (Solution and projects).

Moreover, it explains how to implements a basic server and a lot of the client features.


Known bugs
==========

* Vriska::Time::Zero doesn't work on Windows (it doesn't compile actually)
