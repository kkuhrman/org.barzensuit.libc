/**
 * @file:	bzentest_thread.c
 * @brief:	Unit test pthread encapsulation functions.
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
#include <pthread.h>

/* libbzenc */
#include "bzentest.h"
#include "bzenthread.h"

const int BZENTEST_EXIT_SUCCESS = 0;

void* bzentest_thread_routine(void* arg);

void* bzentest_thread_routine(void* arg)
{
  int* result = (int*)arg;
  *result = BZENTEST_EXIT_SUCCESS;
  return result;
}

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  pthread_t thread;
  int status;
  int wait_time;
  int* exit_code;

  wait_time = 1;
  status = bzen_thread_create(&thread, NULL, bzentest_thread_routine, &wait_time);
  if (BZENPASS != BZENTEST_EQUALS_N(0, status))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  *exit_code = 0;
status = bzen_thread_join(thread, (void**) &exit_code);
  if (BZENPASS != BZENTEST_EQUALS_N(0, status))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  if (BZENPASS != BZENTEST_EQUALS_N(BZENTEST_EXIT_SUCCESS, *exit_code))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  bzen_thread_exit(exit_code);

  return result;
}
