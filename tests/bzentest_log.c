/**
 * @file:	bzentest_log.c
 * @brief:	Unit test the log facility.
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
#include <stdlib.h>
#include <unistd.h>

/* libzenc includes */
#include "bzentest.h"
#include "bzenlog.h"

/* Five logs to test realloc. */
#define BZENTEST_NUM_LOGS 5
const char* log_names[BZENTEST_NUM_LOGS] =
  {
    "apple",
    "bannana",
    "carrot",
    "dillweed",
    "endive"
  };

const char* msg_short =
  "The quick brown fox jumped over the lazy doggy and the orange kitty did too.";

const char* msg_long =
  "The quick brown fox jumped over the lazy doggy and then the orange kitty did also. \
If that wasn't enough, the noisy chicken came along and did the very same thing.";

int main (int argc, char *argv[])
{
  int log_id;
  const char* logdir = getenv("BZEN_LOG_DIR");
  int testdir;
  int status;
  int result;
  
  /* Attempt to open the directory for test file output. */
  testdir = access(logdir, R_OK | W_OK);
  if (BZENPASS != BZENTEST_FALSE(testdir < 0))
    {
      perror("access");
      fprintf (stderr, 
	       "\n\tCould not access log dir specified @ %s.\n\tSee package README for help.", 
	       logdir);
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Open five log files. */
  for (log_id = 0; log_id < BZENTEST_NUM_LOGS; log_id++)
    {
      status = bzen_log_open(log_names[log_id], BZEN_LOG_FOPEN_DEFAULT_ATTR);
      if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
	{
	  BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
	}
    }

  /* Close first log file. */
  status = bzen_log_close(log_names[0]);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Reopen first log file. */
  status = bzen_log_open(log_names[0], BZEN_LOG_FOPEN_DEFAULT_ATTR);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Write short message to first log. */
  status = bzen_log_write(log_names[0], BZENLOG_STATUS, msg_short);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Write long message to first log. */
  status = bzen_log_write(log_names[0], BZENLOG_STATUS, msg_long);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Close first log file again. */
  status = bzen_log_close(log_names[0]);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Write long message to first log using stat method. */
  status = bzen_log_write_stat(log_names[0], BZENLOG_STATUS, msg_long);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Close all remaining files. */
  status = bzen_log_close_all();
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* SUCCESS */
  result = 0;

END_TEST:

  return result;
}
