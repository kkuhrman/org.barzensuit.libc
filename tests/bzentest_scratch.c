/**
 * @file:	bzentest_scratch.c
 * @brief:	For scratch unit tests. Changes are excluded for distro.
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
#include <stdio.h>
#include <string.h>

/* libbzenc */
#include "bzenmem.h"
#include "bzentest.h"
#include "bzensbuf.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;

  /* Some examples of how to use shorthand notation. 
   * In this case arguments are expected to be equal.
   */
  if (BZENPASS != BZENTEST_EQUALS_N(1, 1))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* In this case arguments are expected to be not equal. 
   * The one line notation below is OK if no cleanup is
   * required before exit (as in example above).
   */
  if (BZENPASS != BZENTEST_NOT_EQUALS_N(1, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* In this case expression is expected to be false. */
  if (BZENPASS != BZENTEST_FALSE(1 == 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* In this case expression is expected to be true. 
   * The three line notation with goto statement in 
   * example below is recommended if there is cleanup
   * work to do before exit.
   */
  if (BZENPASS != BZENTEST_TRUE(1 == 1))
    {
      BZENTEST_PRINT_FAIL(__FILE__, __LINE__);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

 END_TEST:

  if (result)
    BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
  else
    BZENTEST_EXIT_PASS;
}
