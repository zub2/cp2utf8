# What is it?
cp2utf8 is a tool for converting data from legacy 8bit encoding into UTF-8 using translation tables identical to those used by the Linux kbd package. (See setfont(8)).

# Why not simply use iconv, recode, ...?
None of the tools I tried could handle both of the following two requirements:

 * convert data from CP895 alias Kamenicky (an old exotic encoding)
 * convert everything including control codes (e.g. ESC) and frame-drawing characters

so I created this tool. It can do both. In fact it's quite easy to define your own conversions because the translation tables are simple text files.

On the other hand it must be noted that cp2utf8 is quite limited: It does not support any sort of multibyte encoding for input. It also supports no guessing and no fuzziness. Only simple crisp rules.

I'm using this program together with DOSEMU, as part of a print filter.

# Compiling
CMake is used for the build. It's possible to configure and build the program by:

    cmake . && make

Prefix can be specified by adding `-DCMAKE_INSTALL_PREFIX=prefix` to the CMake invocation. Default is `/usr/local`.

No libraries apart from C++ runtime are required.

# Installing
Run the `install` make target:

    make install

You can specify your own `DESTDIR`.

# Usage

cp2uf8 always reads from standard input and outputs to standard output. Supply the name of the file containing the translation table to use as the only command line argument. If the file is not found, it's searched for in `$PREFIX/share/cp2utf8/tables`. This directory contains the tables included with cp2ut8. For example:

    cp2utf8 cp895.trans < input.file > output.file

# Licence

GNU GPL 3 or newer.
