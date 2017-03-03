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
const unsigned short int BZENTEST_N_BUFFERS = 8;

const char* sbuf_test_data = 
  "ABCDEFGHIJKLMNOPQRSTUVWX \
   YZabcdefghijklmnopqrstuv \
   wxyz0123456789`~!@#$%^&* \
   AaBbCcDdE()-_=+,<.>/?;:\\ \
   [{]}|\'\"\t    \n \
   \a\b\v\f\r\e \0";

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  double timeout;
  bzen_cbuflock_t** cbuflock;
  int cbuflock_id;
  int num_test_buffers;
  int sbuf_test_data_len;
  int nchar, expected, actual;

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
  num_test_buffers = BZENTEST_N_BUFFERS;
  cbuflock = (bzen_cbuflock_t**)bzen_malloc(sizeof(bzen_cbuflock_t*) * 
					     BZENTEST_N_BUFFERS);
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

      /*  Check counters ( gnulib realloc may allocate more than requested so 
	  we only verify allocated buffers is equal to or greater than used. */
      if ((BZEN_TEST_EVAL_FAIL ==
      	   bzen_test_eval_fn_bool("bzen_sbuf_count_allocated()",
      				  1,
      				  bzen_sbuf_count_allocated() >= bzen_sbuf_count_used(),
      				  NULL)) ||
      	  (BZEN_TEST_EVAL_FAIL ==
      	   bzen_test_eval_fn_int("bzen_sbuf_count_used()", 1 + cbuflock_id,
      				 bzen_sbuf_count_used(), NULL)))
      	{
	  fprintf(stderr, "\n\tbuffers allocated: %d\n\tbuffers used: %d\n",
		  bzen_sbuf_count_allocated(), bzen_sbuf_count_used());
      	  result = BZEN_TEST_EVAL_FAIL;
      	  goto END_TEST;
      	}
    }

  /* Check data structures (allocated size may be larger than requested so just 
     make sure it's at least as big as requested). */
  timeout = 1;
  for (cbuflock_id = 0; cbuflock_id < num_test_buffers; cbuflock_id++)
    {
      status = ((cbuflock[cbuflock_id]->id == cbuflock_id) &&
  		(cbuflock[cbuflock_id]->size >= BZENTEST_BUFFER_SIZE));
      if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_create()",
  							status,
  							1,
  							NULL))
  	{
  	  result = BZEN_TEST_EVAL_FAIL;
  	  goto END_TEST;
  	}
    }

  /**
   * @todo: refactor unit testing to test two separate buffers with two separate
   * strings respectively of varying length, reallocating in between calls to 
   * putc, followed by calls to getc to verify that the position of internal 
   * buffer iterators corrsponds to expected behavior and that neither buffer 
   * content nor iterator positions are effected by reallocation operation.
   */

  /* Fill one of the buffers with test data. */
  sbuf_test_data_len = strlen(sbuf_test_data);
  for (nchar = 0; nchar < sbuf_test_data_len; nchar++)
    {
      expected = sbuf_test_data[nchar];
      actual = bzen_sbuf_putc(expected, cbuflock[0]);
      if (actual == EOF)
	break;
      if (BZENPASS != BZENTEST_EQUALS_N(expected, actual))
	{
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
    }

  /* Rewind the test buffer. */
  status = bzen_sbuf_rewind(cbuflock[0]);
  if (BZENPASS != BZENTEST_EQUALS_N(0, status))
    {
      fprintf(stderr, "\n\tAttempt to rewind stream with id %d failed.\n", 
	      cbuflock[0]->id);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* Read back test data from stream and verify against source. */
  for (nchar = 0; nchar < num_test_buffers; nchar++)
    {
      expected = sbuf_test_data[nchar];
      actual = bzen_sbuf_getc(cbuflock[0]);
      if (actual == EOF)
	{
	  fprintf(stderr, "\n\tbzen_sbuf_getc() reached EOF at pos %d\n", nchar);
	  break;
	}
      if (BZENPASS != BZENTEST_EQUALS_N(expected, actual))
	{
	  fprintf(stderr, "\n\tbzen_sbuf_getc() char mismatch (%c, %c) pos %d\n", 
		  expected,
		  actual,
		  nchar);
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
    }

  /* destroy buffers. */
  timeout = 1;
  for (cbuflock_id = 0; cbuflock_id < num_test_buffers; cbuflock_id++)
    {
      status = bzen_sbuf_destroy(cbuflock[cbuflock_id], timeout);
      if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_destroy()",
							(status != 0),
							0,
							NULL))
	{
	  result = BZEN_TEST_EVAL_FAIL;
	  goto END_TEST;
	}
    }

 END_TEST:

  return result;
}
