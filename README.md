# What is it?
cp2utf8 is a tool for converting 8-byte data into UTF-8 based on translation tables identical to those used by the linux kbd package. (See setfont(8)).

# Why not simply use iconv, recode, ...?
None of the tools I tried could handle both of the following two requirements:

 * convert data from CP895 alias Kamenicky (an old exotic encoding)
 * convert everything including control codes (e.g. ESC) and frame-drawing characters

so I created this tool. It can do both. In fact it's quite easy to define your own conversions because the translation tables are simple text files.

On the other hand it must be noted that cp2utf8 is quite limited: It does not support any sort of multi-byte encoding. It also supports no quessing and no fuzzyness. Only simple crisp rules.

I'm using this program together with DOSEMU, as part of a print filter.

# Compiling
CMake is used for the build. You can configure and build the program by:

    cmake . && make

You can specify prefix by adding `-DCMAKE_INSTALL_PREFIX=prefix` to the CMake invocation. Default is `/usr/local`.

No libraries apart from C++ runtime are required.

# Installing
Run the `install` target of make:

    make install

You can specify yout own `DESTDIR`.

# Licence

GNU GPL 3 or newer.
