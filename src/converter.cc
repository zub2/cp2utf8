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

#include "converter.h"
#include "translation_table.h"

#include <algorithm>
#include <cstdio>

namespace cp2utf8
{
    namespace
    {
        const char TABLE_SUFFIX[] = ".trans";
        const char DIR_SEPARATOR =
#ifdef WIN32
                '\\'
#else
                '/'
#endif
        ;
    }

    char const Converter::DEFAULT_PATH[] = PREFIX "/share/cp2utf8/tables/";

    Converter::Converter(std::string const& tableName, TWarningCallback warningCallback)
    {
        FILE *f = nullptr;

        if (tableName.find(DIR_SEPARATOR) == ustring::npos)
        {
            // try opening a table in DEFAULT_PATH
            const std::string fname = DEFAULT_PATH + tableName + TABLE_SUFFIX;
            f = fopen(fname.c_str(), "r");
        }

        if (f == nullptr)
        {
            // try opening the argument as is
            f = fopen(tableName.c_str(), "r");
        }

        if (f == nullptr)
        {
            throw FileNotFoundException("can't open a file for the given table");
        }

        m_table = std::unique_ptr<TranslationTable>(new TranslationTable(f, warningCallback));
    }

    std::string Converter::convert(std::string const& in)
    {
        std::string result;
        result.reserve(in.size());

        std::for_each(in.begin(), in.end(), [&](std::string::value_type c)
            {
                result.append(m_table->map(c));
            });

        return result;
    }

    Converter::~Converter()
    {}
}
