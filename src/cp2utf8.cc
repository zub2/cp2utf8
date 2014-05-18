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
 * cp2utf8 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cp2utf8. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstring>

#include <iostream>
#include <memory>

#include "converter.h"

static const char *progname = "cp2utf8";

void print_help()
{
    std::cout <<
        "Usage: " << progname << " translation_table_file\n"
        "Translates standard input according to translation table into UTF8 to "
        "standard output.\n"
        "translation_table_file is a file in the same format as that used by "
        "kbd. See setfont(8).\n"
        "translation_table_file is searched for as given, if it's not found, "
        "\"" << cp2utf8::Converter::DEFAULT_PATH << "\" is prepended to the path.\n"
        << std::endl;
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

    try
    {
        std::unique_ptr<cp2utf8::Converter> converter(new cp2utf8::Converter(argv[1],
                [](std::string const& msg) { std::cerr << "warning: " << msg << std::endl; }));

        // perform the actual conversion
        char c;
        std::cin.read(&c, 1);
        while (std::cin.good())
        {
            std::string utf8 = converter->convert(std::string(1, static_cast<char>(c)));
            std::cout.write(utf8.data(), utf8.size());
            std::cin.read(&c, 1);
        }
    }
    catch (cp2utf8::FileNotFoundException const&)
    {
        std::cerr << "Can't open input table." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
