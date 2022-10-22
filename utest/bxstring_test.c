/*
 * boxes - Command line filter to draw/remove ASCII boxes around text
 * Copyright (c) 1999-2021 Thomas Jensen and the boxes contributors
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
 * License, version 3, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Unit tests of the 'bxstring' module
 */

#include "config.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "boxes.h"
#include "bxstring.h"
#include "global_mock.h"
#include "bxstring_test.h"


void test_ascii_simple(void **state)
{
    (void) state;  /* unused */

    bxstr_t *actual = from_ascii("foo");

    assert_non_null(actual);
    assert_non_null(actual->memory);
    assert_string_equal("foo", actual->ascii);
    assert_int_equal(0, (int) actual->indent);
    assert_int_equal(4, (int) actual->num_bytes);
    assert_int_equal(3, (int) actual->num_chars);
    assert_int_equal(0, (int) actual->num_chars_invisible);
    assert_int_equal(0, (int) actual->trailing);
    /* TODO first_char, visible_char */
    assert_int_equal(0, actual->offset_start);
    assert_int_equal(0, actual->offset_end);
}

/*EOF*/                                          /* vim: set cindent sw=4: */
