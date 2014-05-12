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

#ifndef TBL_PARSE_H
#define TBL_PARSE_H

#include <string>
#include <map>

#include "cp2utf8.h"

typedef std::map<char, ustring> TTransTable;

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
void read_table(FILE *f, TTransTable &t);

#endif // TBL_PARSE_H
