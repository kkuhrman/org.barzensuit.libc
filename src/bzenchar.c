/**
 * @file:	bzenchar.c
 * @brief:	Helper functions for managing C strings (char*).
 * 
 * @see:	ftp://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_16.html
 *
 * @todo:       memory management
 *
 * @copyright:	Copyright (C) 2017 Kuhrman Technology Solutions LLC
 * @license:	GPLv3+: GNU GPL version 3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/* Gnu Lib includes */
#include "inttostr.h"
#include "intprops.h"
#include "xalloc.h"
#include "xsize.h"

/* Local includes */
#include "bzenchar.h"

/* Default buffer size */
const int BZENCHAR_DEFAULT_BUFFER_SIZE = INT_BUFSIZE_BOUND (int);
#define BZENCHAR_DEFAULT_BUFFER_SIZE BZENCHAR_DEFAULT_BUFFER_SIZE

/* Creates string representation of integer on heap. */
char* bzen_int_to_str(int value)
{
  char* result;
  size_t buffer_size;
  char buffer[BZENCHAR_DEFAULT_BUFFER_SIZE];
  char* ptr_start;
  
  /* zero out buffer memory */
  /* memset(buffer, 0, BZENCHAR_DEFAULT_BUFFER_SIZE); */
  
  /* Gnu Lib conversion. */
  ptr_start = inttostr(value, buffer);
  buffer_size = xcast_size_t((buffer + BZENCHAR_DEFAULT_BUFFER_SIZE) - ptr_start);
  
  /* Allocate memory for output string. */
  result = xcharalloc(buffer_size);
  
  /* Copy contents of buffer to output (zero memory first). */
  memset(result, 0, buffer_size);
  result = (char*) memcpy(result, ptr_start, buffer_size);
  
  return result;
}
