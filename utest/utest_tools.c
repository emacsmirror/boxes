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
 * Helper functions for all the unit tests.
 */

#include "config.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "utest_tools.h"


void assert_array_equal(size_t *p_expected, size_t *p_actual, size_t p_len_expected)
{
    if (p_expected == NULL && p_actual == NULL) {
        assert_int_equal(0, p_len_expected);
        return;
    }
    if (p_expected == NULL) {
        assert_null(p_actual);
        assert_int_equal(0, p_len_expected);
    }
    if (p_actual == NULL) {
        assert_null(p_expected);
        assert_int_equal(0, p_len_expected);
    }

    for (size_t i = 0; i < p_len_expected; i++) {
        assert_int_equal((int) p_expected[i], (int) p_actual[i]);
    }
}


/*EOF*/                                          /* vim: set cindent sw=4: */
