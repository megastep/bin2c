bin2c
=====

Convert binary files to C arrays for easy inclusion within your existing code.

It sometimes makes sense to bypass the entire management of the resources, especially for smaller or more sensitive files.
The data becomes availably as a single array that can be parsed in memory any way you like.

Building
========

The project was created in Xcode 5 on Mac OS X but the C file should compile on pretty much any system with an ANSI C compiler.


Usage
=====

`bin2c -i <inputfile> [-o output.h] -a array_name`

If no output file is specified, the C array will be sent on the standard output.

`array_name` is used to specify the name of the variable.

License
=======

Very liberal - do as you wish with it! Pull requests welcome.

Author
======

Stephane Peter
http://github.com/megastep
