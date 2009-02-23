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

#include <cstdio>
#include <cstdlib>
#include <limits>

#include "tbl_parse.h"

namespace
{
    /**
     * A simple wrapper around stdio stream.
     * It keeps track of current line number, it can skip #-style comments etc...
     */
    class file
    {
    public:
        file(FILE *f);
        ~file();

        /**
         * Get next character while skipping #-style comments.
         */
        int getc();

        int ungetc(int c);
        inline int skip_ws_fgetc();

        inline bool check_next_char_skip_ws(char c);
        inline bool check_next_char(char c);
        bool check_str_skip_ws(const char *s);

        /**
         * Parse an integer.
         */
        bool parse_int(int &i);

        /**
         * Parse a unicode character description, as used by the .trans files.
         * Valid input is of the form: "U+XXXX" where X is a hex digit.
         */
        bool parse_uni(TUnicodeChar &i);

        /**
         * Parse a sequence of unicode character descriptions.
         */
        bool parse_uni_str(ustring &s);

        int get_line_no();

        bool ferror();
        bool feof();

    private:
        FILE *f;
        int line;
    };

    file::file(FILE *f):
        f(f),
        line(1)
    {}

    file::~file()
    {
        if (f != NULL)
            fclose(f);
    }

    int file::getc()
    {
        char c = fgetc(f);
        if (c == '\n')
            line++;

        // skip comments
        if (c == '#')
        {
            do
            {
                c = fgetc(f);
            }
            while (c != EOF && c != '\n');

            if (c == '\n')
                line++;
        }

        return c;
    }

    int file::ungetc(int c)
    {
        if (c == '\n')
            line--;

        return ::ungetc(c,f);
    }

    int file::skip_ws_fgetc()
    {
        int c = getc();
        while (c != EOF && c <= ' ')
            c = getc();

        return c;
    }

    bool file::check_next_char_skip_ws(char c)
    {
        return c == skip_ws_fgetc();
    }

    bool file::check_next_char(char c)
    {
        return c == getc();
    }

    bool file::check_str_skip_ws(const char *s)
    {
        if (s == NULL || *s == 0)
            return true;

        if (!check_next_char_skip_ws(*s++))
            return false;

        while (*s != 0)
            if (!check_next_char(*s++))
                return false;

        return true;
    }

    bool file::parse_int(int &i)
    {
        int c;

        c = skip_ws_fgetc();
        if (c == EOF)
            return false;

        ungetc(c);
        if (fscanf(f, "%i", &i) != 1)
            return false;

        return true;
    }

    bool file::parse_uni(TUnicodeChar &i)
    {
        if (!check_next_char_skip_ws('U') || !check_next_char('+'))
            return false;

        if (fscanf(f, "%x", &i) != 1)
            return false;

        return true;
    }

    bool file::parse_uni_str(std::basic_string<TUnicodeChar> &s)
    {
        TUnicodeChar i;
        char c;

        s.clear();

        c = skip_ws_fgetc();
        while (c == 'U')
        {
            ungetc('U');
            if (!parse_uni(i))
                return false;

            s.append(1, i);
            c = skip_ws_fgetc();
        }

        if (c != EOF)
            ungetc(c);
        return true;
    }

    int file::get_line_no()
    {
        return line;
    }

    bool file::ferror()
    {
        return f == NULL || ::ferror(f);
    }

    bool file::feof()
    {
        return f == NULL || ::feof(f);
    }
}

inline bool check_byte_range(int c)
{
    return c >= 0 && c <= 0xff;
}

void read_table(FILE *c_file, TTransTable &trans_table)
{
    static const char err_transtbl[] = "Error in translation table at line %d.\n";
    static const char warn_reset[] = "Warning: resetting the value of byte %02x at line %d\n";
    static const char err_out_of_range[] = "Warning: A source byte in translation table is outside the 0x00 - 0xFF range at line %d\n";
    static const char undefined_byte[] = "Warning: no character assigned to byte %02x\n";

    file f(c_file);

    while (!f.feof() && !f.ferror())
    {
        int from;
        if (!f.parse_int(from))
        {
            if (f.feof())
            {
                break;
            }
            fprintf(stderr, err_transtbl, f.get_line_no());
            exit(1);
        }

        // check for possible range: %i - %i
        const int c = f.skip_ws_fgetc();
        if (c == '-')
        {
            // this is a range
            const int start = from;

            int end;
            if (!f.parse_int(end))
            {
                fprintf(stderr, err_transtbl, f.get_line_no());
                exit(1);
            }

            // we detected a range, the only keyword here we know is "idem"
            if (!f.check_str_skip_ws("idem"))
            {
                fprintf(stderr, "Error in translation table at line %d: 'idem' expected.", f.get_line_no());
                exit(1);
            }

            // check byte values
            if (!check_byte_range(start) || !check_byte_range(end) || start > end)
            {
                fprintf(stderr, err_out_of_range, f.get_line_no());
                exit(1);
            }

            // set all bytes in the range to identity
            for (unsigned i = static_cast<unsigned>(start); i <= static_cast<unsigned>(end); i++)
            {
                const ustring str(1, i);
                const bool new_element = trans_table.insert(std::make_pair(static_cast<char>(i), str)).second;
                if (!new_element)
                    fprintf(stderr, warn_reset, from, f.get_line_no());
            }
        }
        else
        {
            // standard item
            f.ungetc(c);

            ustring to;
            if (!f.parse_uni_str(to))
            {
                fprintf(stderr, err_transtbl, f.get_line_no());
                exit(1);
            }

            // check byte values
            if (!check_byte_range(from))
            {
                fprintf(stderr, err_out_of_range, f.get_line_no());
                exit(1);
            }

            if (to.length() == 0)
            {
                fprintf(stderr, err_transtbl, f.get_line_no());
                exit(1);
            }

            const bool new_element = trans_table.insert(std::make_pair(static_cast<char>(from), to)).second;
            if (!new_element)
                fprintf(stderr, warn_reset, from, f.get_line_no());
        }
    }

    // check that all bytes were assigned
    TTransTable::const_iterator it = trans_table.begin();
    for (int c = static_cast<int>(std::numeric_limits<char>::min());
            c < static_cast<int>(std::numeric_limits<char>::max()); ++c)
    {
        if (it->first != c)
        {
            fprintf(stderr, undefined_byte, static_cast<unsigned char>(c));
        }
        else
            ++it;
    }
}
