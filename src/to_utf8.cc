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

#include "to_utf8.h"

#define UTF8_PREFIX1    0xc0   /* '110 00000' */
#define UTF8_PREFIX2    0xe0   /* '1110 0000' */
#define UTF8_PREFIX3    0xf0   /* '11110 000' */

#define UTF8_CONT       0x80   /* '10 000000' */
#define UTF8_CONT_MASK  0x3f   /* '00 111111' */
#define UTF8_CONT_SHIFT 6

/**
 * Returns the UTF8 representation of a given unicode character.
 *
 * This is inefficient. But I was lazy searching for/coming up with something
 * more efficient.
 */
std::string conv_to_utf8(TUnicodeChar character)
{
    char c[4];
    int used = 0;

    /* Convert to UTF8 */
    if (character < 0x80)
    {
        /* 0xxxxxxx - identity: */
        c[0] = static_cast<char>(character);
        used = 1;
    }
    else if (character < 0x800)
    {
        /* 110xxxxx 10xxxxxx */
        c[1] = static_cast<char>(UTF8_CONT | (character & UTF8_CONT_MASK));
        character >>= UTF8_CONT_SHIFT;
        c[0] = static_cast<char>(UTF8_PREFIX1 | character);
        used = 2;
    }
    else if (character < 0x10000)
    {
        /* 1110xxxx 10xxxxxx 10xxxxxx */
        c[2] = static_cast<char>(UTF8_CONT | (character & UTF8_CONT_MASK));
        character >>= UTF8_CONT_SHIFT;
        c[1] = static_cast<char>(UTF8_CONT | (character & UTF8_CONT_MASK));
        character >>= UTF8_CONT_SHIFT;
        c[0] = static_cast<char>(UTF8_PREFIX2 | character);
        used = 3;
    }
    else if (character < 0x110000)
    {
        /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
        c[3] = static_cast<char>(UTF8_CONT | (character & UTF8_CONT_MASK));
        character >>= UTF8_CONT_SHIFT;
        c[2] = static_cast<char>(UTF8_CONT | (character & UTF8_CONT_MASK));
        character >>= UTF8_CONT_SHIFT;
        c[1] = static_cast<char>(UTF8_CONT | (character & UTF8_CONT_MASK));
        character >>= UTF8_CONT_SHIFT;
        c[0] = static_cast<char>(UTF8_PREFIX3 | character);
        used = 4;
    }
    else
    {
        fprintf(stderr,"Cannot express character U+%x in UTF-8.\n", character);
        abort();
    }

    std::string buff;
    buff.append(c, used);

    return buff;
}
