/**
 * @file:	bzentest_char.c
 * @brief:	Unit test string manipulation.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <config.h>
#include "xsize.h"
#include "bzenchar.h"

int main (int argc, char *argv[])
{
  int result = 0;
  const int test_num = -322048;
  const char* test_str = "-322048";
  const size_t test_str_size = xcast_size_t(sizeof(test_str));
  char* str_out;
  size_t str_out_size;

  /* Convert int to str */
  str_out = bzen_int_to_str(test_num);
  if (str_out == NULL)
  {
    fprintf (stderr, "bzen_int_to_str(%d) expected %s (%d); returned NULL\n", 
	     test_num,
	     test_str,
	     test_str_size);
    result = 1;
    goto END_TEST;
  }
  
  /* Compare size. */
  str_out_size = xcast_size_t(sizeof(str_out));
  if (bzen_test_eval_fn_int("bzen_int_to_str\n", test_str_size, str_out_size, NULL))
    goto END_TEST;
  
  /* if (test_str_size != str_out_size) */
  /* { */
  /*   fprintf(stderr, "bzen_int_to_str returns %s\n", str_out); */
  /*   result = bzen_test_eval_fn_int("bzen_int_to_str\n", test_str_size, str_out_size, NULL); */
  /*   goto END_TEST; */
  /* } */
  
  /* Compare character sequence. */
  if (0 != strcmp(test_str, str_out))
  {
    /* @todo: bzen_test_eval_fn_chars. */
    result = 1;
    goto END_TEST;
  }
  
END_TEST:
  return result;
}
