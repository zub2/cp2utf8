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

#ifndef CP2UTF8_CONVERTER_H
#define CP2UTF8_CONVERTER_H

#include <stdexcept>
#include <memory>
#include <functional>

namespace cp2utf8
{
    class TranslationTable;

    class FileNotFoundException: public std::runtime_error
    {
    public:
        FileNotFoundException(std::string const& message):
            std::runtime_error(message)
        {}
    };

    /**
     * Class performing conversion from a legacy 8bit encoding (described by
     * a conversion table) to UTF-8.
     */
    class Converter
    {
    public:
        typedef std::function<void(std::string const& msg)> TWarningCallback;

        /**
         * Create an instance of the converter.
         *
         * \param[in] tableName Name of the conversion table. It can be either the name of the table
         *                      without path and suffix (it's searched for in DEFAULT_PATH/name.trans)
         *                      or a filename of the table to use.
         *
         */
        Converter(std::string const& tableName, TWarningCallback warningCallback = TWarningCallback());

        /**
         * Convert a string.
         *
         * \param[in] Input, in whatever encoding this instance has been created for.
         * \return Input converted to UTF-8.
         */
        std::string convert(std::string const& in);

        virtual ~Converter();

        static char const DEFAULT_PATH[];

    private:

        std::unique_ptr<TranslationTable> m_table;
    };
}

#endif // CP2UTF8_CONVERTER_H
