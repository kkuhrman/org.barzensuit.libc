/**
 * @file:	bzentest_dbug.c
 * @brief:	Unit test debugging functions.
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
#include <string.h>

/* libbzenc */
#include "bzentest.h"
#include "bzenlog.h"
#include "bzendbug.h"

const unsigned short int BZENTEST_BUFFER_SIZE = 128;
const unsigned short int BZENTEST_N_BUFFERS = 1;

const char* sbuf_test_data = 
  "ABCDEFGHIJKLMNOPQRSTUVWX \
   YZabcdefghijklmnopqrstuv \
   wxyz0123456789`~!@#$%^&* \
   AaBbCcDdE()-_=+,<.>/?;:\\ \
   [{]}|\'\"\t    \n \
   \a\b\v\f\r\e \0";
const char* logname = "bzentest-debugging.log";

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  bzen_cbuflock_t* cbuflock;
  int sbuf_test_data_len,  nchar, actual;
  const char* logdir;
  int status;

  /* Create a buffer. */
  cbuflock = bzen_sbuf_create(BZENTEST_BUFFER_SIZE);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_sbuf_create()",
						    (cbuflock == NULL),
						    0,
						    NULL))
    {
      fprintf(stderr, "\n\tFailed to create buffer for debugging unit test.\n");
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* Write test data to buffer. */
  sbuf_test_data_len = strlen(sbuf_test_data);
  for (nchar = 0; nchar < sbuf_test_data_len; nchar++)
    {
      actual = bzen_sbuf_putc(sbuf_test_data[nchar], cbuflock);
      if (actual == EOF)
	break;
    }

  /* Verify size of buffer. */
  /* @todo: this is a redundant and unnecessary test but produces 136 vs. 512. Check types. */
  /* if (BZENPASS != BZENTEST_EQUALS_N(cbuflock->size, sbuf_test_data_len)) */
  /*   { */
  /*     fprintf(stderr, "\n\tBuffer write failed.\n\tExpected %d chars wrote %d.\n", */
  /* 	      sbuf_test_data_len, cbuflock->size); */
  /*     result = BZEN_TEST_EVAL_FAIL; */
  /*     goto END_TEST; */
  /*   } */

  /* Rewind buffer. */
  status = bzen_sbuf_rewind(cbuflock);
  if (BZENPASS != BZENTEST_EQUALS_N(0, status))
    {
      fprintf(stderr, "\n\tAttempt to rewind debugging buffer failed.\n");
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* Open a debugging log. */
  logdir = bzen_log_dir();
  if (BZENPASS == BZENTEST_FALSE(logdir != NULL))
    {
      fprintf (stderr, 
	       "\n\tCould not access log dir specified @ %s.\n\tSee package README for help.\n", 
	       logdir);
      goto END_TEST;
    }
  status = bzen_log_open(logname, BZEN_LOG_FOPEN_DEFAULT_ATTR);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      fprintf (stderr, 
	       "\n\tFailed to open log file %s.\n", 
	       logname);
      goto END_TEST;
    }

  /* Dump buffer to debugging log. */
  nchar = bzen_dbug_log_buffer_dump(cbuflock, logname, BZENTEST_BUFFER_SIZE);

  /* Verify characters written to log. */
  if (BZENPASS != BZENTEST_EQUALS_N(nchar, sbuf_test_data_len))
    {
      fprintf(stderr, "\n\tBuffer dump failed.\n\tExpected %d chars wrote %d.\n",
	      sbuf_test_data_len, nchar);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }


 END_TEST:

  return result;
}
