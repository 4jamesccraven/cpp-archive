# Archive of Old Project Rewrites
Alternate title: "Yes I really did this for fun"
------------------------------------------------
I've been recently playing around with and enjoying modern
c++23 features, and it inspired me to go back and rewrite
some of my old college assingments.

There's no real other "point" to this, you can just come see
how my style has changed and improved.

Building
--------
This project uses CMake >= 3.16, and C++23. I only tested with
gcc, but I have no reason to doubt it'd work on clang or msvc
either. If it breaks make an issue.
```
git clone https://github.com/4jamesccraven/cpp-archive.git
cd cpp-archive
mkdir build && cd build
cmake ..
make
```
To run a project just type its name as the first argument to the
main binary. A flag to list available projects is coming eventually.

This is the current list of projects:
- 'fortune': an interactive fortune teller
