bin2c
=====

Convert binary files to C arrays for easy inclusion within your existing code.

It sometimes makes sense to bypass the entire management of binary resources, especially for smaller or more sensitive data files.
The data becomes available as a single array (or optionally as a NSString) that can be parsed in memory any way you like.

Building
========

The project was created in Xcode 5 on Mac OS X but the C file should compile on pretty much any system with an ANSI C compiler.
It's also known to work on Gentoo with gcc 5.4.0.


Usage
=====

`bin2c [-i inputfile] [-o output.h] [-l line_len] [-t type] [-hs0] -a array_name`
-h: show a brief help message
-s: prepend the 'static' keyword to the generated variables
-0: when -t char, add a null-char at the end of the array

If no output file is specified, the C array will be sent on the standard output. Likewise, if no input file is given, data is read from the standard input.

- `array_name` is used to specify the name of the variable.
- `line_len` specifies how many bytes to put per line in the output (default 80).
- `type` can be either `char` (default, C array of unsigned char), or `nsstring` (Objective-C `NSString` object).

License
=======

Very liberal - do as you wish with it! Pull requests welcome.

Author
======

Stephane Peter
Catloaf Software, LLC

http://github.com/megastep
http://www.catloafsoft.com/
