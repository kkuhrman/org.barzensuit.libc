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
#include <string.h>

/* libbzenc */
#include "bzentest.h"
#include "bzensbuf.h"

const unsigned short int BZENTEST_BUFFER_SIZE = 128;

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  double timeout;
  bzen_cbuflock_t** cbuflock;
  int cbuflock_id;
  int num_test_buffers;

  /* Check counters. */
  if ((BZEN_TEST_EVAL_FAIL == 
       bzen_test_eval_fn_int("bzen_sbuf_count_allocated()", 0, 
			     bzen_sbuf_count_allocated(), NULL)) ||
      (BZEN_TEST_EVAL_FAIL == 
       bzen_test_eval_fn_int("bzen_sbuf_count_used()", 0, 
			     bzen_sbuf_count_used(), NULL)))
    {
      	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
    }

  /* First test dynamic sizing of internal buffers registry. */
  num_test_buffers = 2 *  BZEN_DEFAULT_NUMBER_OF_BUFFERS;
  cbuflock = (bzen_cbuflock_t**)bzen_malloc(sizeof(bzen_cbuflock_t*) 
					   * 2 * BZEN_DEFAULT_NUMBER_OF_BUFFERS);
  for (cbuflock_id = 0; cbuflock_id < num_test_buffers; cbuflock_id++)
    {
      /* Create a stream buffer */
      cbuflock[cbuflock_id] = bzen_sbuf_create(BZENTEST_BUFFER_SIZE);
      if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_create()",
							(cbuflock == NULL),
							0,
							NULL))
	{
	  fprintf(stderr, "\n\tFailed to create buffer %d of %d\n",
		  cbuflock_id, num_test_buffers);
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}

      /* Check counters. */

      /* gnulib realloc may allocate more than requested so we only verify 
         that allocated buffers is equal to or greater than used. */
      if ((BZEN_TEST_EVAL_FAIL == 
	   bzen_test_eval_fn_bool("bzen_sbuf_count_allocated()", 
				  1, 
				  bzen_sbuf_count_allocated() >= bzen_sbuf_count_used(), 
				  NULL)) ||
	  (BZEN_TEST_EVAL_FAIL == 
	   bzen_test_eval_fn_int("bzen_sbuf_count_used()", 1 + cbuflock_id, 
				 bzen_sbuf_count_used(), NULL)))
	{
      	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
    }

  /* Check data structures and destroy all but first buffer. */
  timeout = 1;
  for (cbuflock_id = num_test_buffers - 1; cbuflock_id > 0; cbuflock_id--)
    {
      status = ((cbuflock[cbuflock_id]->id == cbuflock_id) &&
		(cbuflock[cbuflock_id]->size == BZENTEST_BUFFER_SIZE));
      if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_create()",
							status,
							1,
							printf("\n\t%d of %d\n",
							       cbuflock_id,
							       num_test_buffers)))
	{
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
      status = bzen_sbuf_destroy(cbuflock[cbuflock_id], timeout);
      if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_destroy()",
							(status != 0),
							0,
							printf("\n\t%d of %d\n",
							       cbuflock_id,
							       num_test_buffers)))
	{
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
    }

  /* @todo: remainder of testing on zeroth buffer. */

  /* destroy remaining buffer. */
  status = bzen_sbuf_destroy(cbuflock[0], timeout);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_destroy()",
						    (status != 0),
						    0,
						    "\n\tlast buffer\n"))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

 END_TEST:

  return result;
}
