/*
 * Copyright (C) 2006, 2009, 2014 David Kozub <zub at linux.fjfi.cvut.cz>
 *
 * This file is part of cp2utf8.
 *
 * cp2utf8 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cp2utf8. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cerrno>

#include "cp2utf8.h"
#include "to_utf8.h"
#include "tbl_parse.h"

static const char *progname = "cp2utf8";
static const char *DEFAULT_PATH = PREFIX "/share/cp2utf8/tables/";

void print_help()
{
    printf(
        "Usage: %s translation_table_file\n"
        "Translates standard input according to translation table into UTF8 to "
        "standard output.\n"
        "translation_table_file is a file in the same format as that used by "
        "kbd. See setfont(8).\n"
        "translation_table_file is searched for as given, if it's not found, "
        "\"%s\" is prepended to the path.\n", progname, DEFAULT_PATH
    );
}

int main(int argc, char *argv[])
{
    if (argc > 1)
        progname = argv[0];

    if (argc != 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        print_help();
        return EXIT_FAILURE;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f && errno == ENOENT)
    {
        // retry with default path
        std::string s(DEFAULT_PATH);
        s += argv[1];
        f = fopen(s.c_str(), "r");
        if (!f)
        {
            perror("cannot open translation table file (tried default path)");
            return EXIT_FAILURE;
        }
    }

    if (!f)
    {
        perror("cannot open translation table file");
        return EXIT_FAILURE;
    }

    TTransTable t;
    read_table(f, t);

    // perform actual conversion
    int c;
    while ((c = getchar()) != EOF)
    {
        TTransTable::const_iterator it = t.find(c);
        if (it == t.end())
        {
            continue; // assume no entry = translate to empty string
        }

        const ustring& s = it->second;
        for (unsigned i = 0; i < s.length(); i++)
        {
            std::string utf8 = conv_to_utf8(s[i]);
            for (size_t j = 0; j < utf8.length(); j++)
                putchar(utf8[j]);
        }
    }

    return EXIT_SUCCESS;
}
