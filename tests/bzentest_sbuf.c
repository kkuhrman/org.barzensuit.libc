/**
 * @file:	bzentest_sbuf.c
 * @brief:	Unit test stream buffer functions.
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

/* libbzenc */
#include "bzentest.h"
#include "bzensbuf.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  double timeout;
  bzen_sbuf* pbuffer;

  /* Create a stream buffer */
  pbuffer = bzen_sbuf_create(BZEN_SBUF_DEFAULT_SIZE);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_create()",
						    (pbuffer == NULL),
						    0,
						    NULL))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  int index;
  for (index = 0; index < BZEN_SBUF_DEFAULT_SIZE; index++)
    {
      if (pbuffer->buffer[index] != 0)
	{
	  fprintf(stderr, "\n\tunexpected character at index %d in buffer: %c\n", 
		  index,
		  pbuffer->buffer[index]);
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
    }

  /* Destroy the stream buffer. */
  timeout = 1;
  status = bzen_sbuf_destroy(pbuffer, timeout);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_destroy()",
  						    (status != 0),
  						    0,
  						    NULL))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

 END_TEST:

  return result;
}
