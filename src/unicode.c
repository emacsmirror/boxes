/*
 * boxes - Command line filter to draw/remove ASCII boxes around text
 * Copyright (C) 1999  Thomas Jensen and the boxes contributors
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, version 2, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/*
 * Functions and constants for handling unicode strings with libunistring.
 */

#include "config.h"
#include <errno.h>
#include <stdlib.h>

#include <unictype.h>
#include <unistr.h>
#include <unitypes.h>

#include "unicode.h"


const char *encoding;                          /* the character encoding that we use */

const ucs4_t char_tab     = 0x00000009;        /* ucs4_t character '\t' (tab)  */
const ucs4_t char_space   = 0x00000020;        /* ucs4_t character ' '  (space) */
const ucs4_t char_cr      = 0x0000000d;        /* ucs4_t character '\r' (carriage return) */
const ucs4_t char_newline = 0x0000000a;        /* ucs4_t character '\n' (newline) */
const ucs4_t char_esc     = 0x0000001b;        /* ucs4_t character 0x1b (escape)  */
const ucs4_t char_nul     = 0x00000000;        /* ucs4_t character '\0' (zero) */



/**
 * Check whether the character at the given index has the given value.
 *
 * @param <text> the string to check
 * @param <idx> the index position of the character to check
 * @param <expected_char> the expected character value
 * @return flag indicating whether the character has the expected value
 */
int is_char_at(const uint32_t *text, const size_t idx, const ucs4_t expected_char)
{
    return text != NULL && u32_cmp(text + idx, &expected_char, 1) == 0;
}



/**
 * Set the character at the given index to the given value.
 *
 * @param <text> the string to modify
 * @param <idx> the index position of the character to modify
 * @param <char_to_set> the new character value
 */
void set_char_at(uint32_t *text, const size_t idx, const ucs4_t char_to_set)
{
    u32_set(text + idx, char_to_set, 1);
}



/**
 *  Determine if a string is NULL/empty or not.
 *
 *  @param <text> the string to check
 *  @return > 0: the string is empty or NULL
 *            0: the string contains at least 1 character
 */
int is_empty(const uint32_t *text)
{
    return text == NULL || is_char_at(text, 0, char_nul);
}



int is_ascii_printable(const ucs4_t c)
{
    return c >= 0x20 && c < 0x7f;
}



uint32_t *new_empty_string32()
{
    return (uint32_t *) calloc(1, sizeof(uint32_t));
}



uint32_t *advance32(uint32_t *s, const size_t offset)
{
    if (is_empty(s)) {
        return new_empty_string32();
    }
    if (offset == 0) {
        return s;
    }

    ucs4_t c;                  /* the current character we're looking at */
    const uint32_t *cStr = s;  /* pointer to c in s */
    size_t idx = 0;            /* the count of visible characters */
    const uint32_t *last_esc;  /* pointer to the start of the last escape sequence encountered */
    const uint32_t *rest = s;  /* pointer to the next character coming up, needed only for u32_next() api */
    int visible = 1;           /* flag indicating whether the previous char was a visible char */
    int ansipos = 0;           /* progression of ansi sequence */

    while ((rest = u32_next(&c, rest))) {
        if (ansipos == 0 && c == char_esc) {
            /* Found an ESC char, count it as invisible and move 1 forward in the detection of CSI sequences */
            last_esc = cStr;
            visible = 0;
            ansipos++;
        } else if (ansipos == 1 && c == '[') {
            /* Found '[' char after ESC. A CSI sequence has started. */
            ansipos++;
            visible = 0;
        } else if (ansipos == 1 && c >= 0x40 && c <= 0x5f) {
            /* Found a char designating the end of a two-byte escape sequence */
            visible = 0;
            ansipos = 0;
        } else if (ansipos == 2) {
            /* Inside CSI sequence - Keep counting chars as invisible */
            visible = 0;

            /* A char between 0x40 and 0x7e signals the end of an CSI or escape sequence */
            if (c >= 0x40 && c <= 0x7e) {
                ansipos = 0;
            }
        } else {
            /* a visible char */
            if (idx == offset) {
                if (!visible) {
                    return (uint32_t *) last_esc;
                }
                return (uint32_t *) cStr;
            }
            ++idx;
            visible = 1;
        }
        cStr = rest;
    }
    return new_empty_string32();  /* offset too large, not enough characters in string */
}


/*EOF*/                                                  /* vim: set sw=4: */