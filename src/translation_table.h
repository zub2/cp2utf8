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

#ifndef TRANSLATION_TABLE_H
#define TRANSLATION_TABLE_H

#include <stdexcept>
#include <functional>
#include <string>
#include <map>
#include <sstream>

#include "cp2utf8.h"

namespace cp2utf8
{
    /**
     * Exception thrown when parsing of the translation table fails.
     */
    class TranslationTableParseError: public std::runtime_error
    {
    public:
        TranslationTableParseError(size_t lineNo, std::string const& extraMsg = std::string()):
            std::runtime_error(formatErrorMessage(lineNo, extraMsg))
        {}

    private:
        static std::string formatErrorMessage(size_t lineNo, std::string const& extraMsg)
        {
            std::stringstream stringStream;
            stringStream << "Error in translation table at line at line " << lineNo;
            if (!extraMsg.empty())
            {
                stringStream << ": " << extraMsg;
            }
            return stringStream.str();
        }
    };

    class TranslationTable
    {
    public:
        typedef std::function<void(std::string const& msg)> TWarningCallback;

        TranslationTable(FILE *c_file, TWarningCallback warningCallback = TWarningCallback());

        std::string const& map(char c) const
        {
            auto it = m_table.find(c);
            if (it == m_table.end())
            {
                return EMPTY_STRING; // assume no entry = translate to empty string
            }

            return it->second;
        }

    private:
        /**
         * Read a kdbtools-style translation table.
         *
         * The supported format is the same as the format used by kbdtools:
         *
         * # denotes a comment
         * other lines:
         * num1 whitespace num2
         *
         * num1 = src
         * num2 = desc
         *
         * This is actually a bit sloppy - it ignores newlines, so it accepts any
         * sequence of <int> <uni> items. But this includes well-formed uni translation
         * tables, so I think it's ok. :)
         */
        void readTable(FILE *c_file, TWarningCallback warningCallback);

        void check(TWarningCallback warningCallback) const;
        static bool checkByteRange(int c);

        /**
         * A map from input bytes into UTF-8 encoded unicode strings.
         */
        typedef std::map<char, std::string> TTransTable;

        TTransTable m_table;
        static const std::string EMPTY_STRING;
    };
}

#endif // TRANSLATION_TABLE_H
