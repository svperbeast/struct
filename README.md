# Introduction

Pack and Unpack packet data.

Inspired by 'The Practice of Programming (Brian W. Kernighan, Rob Pike)' and

Python struct module.

# Install

    cd build
    cmake ..
    make
    make install

headers: `build/export/include/`.

library: `build/export/lib/`.

## Test

    make test

or run `struct_test`.

valgrind memory check:

    ctest -T memcheck

# References

[The Practice of Programming (9.1 Formatting Data)](http://www.amazon.com/Practice-Programming-Addison-Wesley-Professional-Computing/dp/020161586X/ref=sr_1_1?ie=UTF8&qid=1359350725&sr=8-1&keywords=practice+of+programming "The Practice of Programming")

[Python struct](http://docs.python.org/2/library/struct.html#module-struct "Python struct module")
