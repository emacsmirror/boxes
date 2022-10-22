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
 * The boxes-internal representation of strings.
 */

#include "config.h"
#include <string.h>
#include "bxstring.h"
#include "tools.h"
#include "unicode.h"



bxstr_t *from_ascii(char *pAscii)
{
    if (pAscii == NULL) {
        bx_fprintf(stderr, "%s: internal error: from_ascii() called with NULL\n", PROJECT);
        return NULL;
    }
    if (strchr(pAscii, '\t') != NULL) {
        bx_fprintf(stderr, "%s: internal error: from_ascii() called with tabs: \"%s\"\n", PROJECT, pAscii);
        return NULL;
    }

    bxstr_t *result = (bxstr_t *) calloc(1, sizeof(bxstr_t));
    result->memory = u32_strconv_from_arg(pAscii, "ASCII");
    if (result->memory == NULL) {
        BFREE(result);
        return NULL;
    }
    result->ascii = pAscii;

    size_t num_esc = 0;
    char *ascii_copy;
    size_t *map;
    result->num_chars_invisible = count_invisible_chars(result->memory, &num_esc, &ascii_copy, &(map));
    BFREE(ascii_copy);

    result->num_chars = strlen(pAscii);
    result->num_bytes = result->num_chars + 1;

    /* TODO convert map to uint32_t **visible_char */
    return result;
}



/*EOF*/                                          /* vim: set cindent sw=4: */
