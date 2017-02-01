/**
 * @file:	bzentest_ut.c
 * @brief:	Unit test unit testing helper functions.
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

#include "bzentest.h"

int main (int argc, char *argv[])
{
  int result = 0;

  /* boolean comparison PASS. */
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_test_eval_fn_bool()", 
						    1, 
						    1, 
						    NULL))
    {
      fprintf(stderr, "boolean comparison expected: PASS; actual: FAIL.");
      result = 1;
      goto END_TEST;
    }

  /* boolean comparison FAIL. */
  if (BZEN_TEST_EVAL_PASS == bzen_test_eval_fn_bool("bzen_test_eval_fn_bool()", 
						    1, 
						    0, 
						    NULL))
    {
      fprintf(stderr, "boolean comparison expected: FAIL; actual: PASS.");
      result = 1;
      goto END_TEST;
    }

  /* integer comparison PASS. */
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_int("bzen_test_eval_fn_int()", 
						   1, 
						   1, 
						   NULL))
    {
      fprintf(stderr, "integer comparison expected: PASS; actual: FAIL.");
      result = 1;
      goto END_TEST;
    }

  /* integer comparison FAIL. */
  if (BZEN_TEST_EVAL_PASS == bzen_test_eval_fn_int("bzen_test_eval_fn_int()", 
						   1, 
						   0, 
						   NULL))
    {
      fprintf(stderr, "integer comparison expected: FAIL; actual: PASS.");
      result = 1;
      goto END_TEST;
    }

 END_TEST:
  return result;
}
