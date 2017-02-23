/**
 * @file:	bzentest.c
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

#include <config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bzentest.h"

/* Casts any integer as a pseudo boolean type. */
int bzen_test_bool(int value)
{
  int result = (value == 0) ? BZEN_TEST_BOOL_FALSE : BZEN_TEST_BOOL_TRUE;
  return result;
}

/* Test if two integer values are equal. */
int bzen_test_equals_int(int expected, int actual)
{
  int result = (expected == actual) ? BZENPASS : BZENFAIL;
  return result;
}

/* Test if two integer values are not equal. */
int bzen_test_not_equals_int(int expected, int actual)
{
  int result = (expected != actual) ? BZENPASS : BZENFAIL;
  return result;
}

/* Tests if the given expression is FALSE. */
int bzen_test_logical_false(int expr)
{
  int result = BZEN_TEST_BOOL(expr) ? BZENFAIL : BZENPASS;
  return result;
}

/* Tests if the given expression is TRUE. */
int bzen_test_logical_true(int expr)
{
  int result = BZEN_TEST_BOOL(expr) ? BZENPASS : BZENFAIL;
  return result;
}

/* Evaluates result of test on funtion returning boolean. */
int bzen_test_eval_fn_bool(char* fn_name, int expected, int actual, char* error_message)
{
  int result;
  char* msg_out;
  int expected_bool = BZEN_TEST_BOOL(expected);
  int actual_bool = BZEN_TEST_BOOL(actual);
  
  result = (expected_bool == actual_bool) ? BZEN_TEST_EVAL_PASS : BZEN_TEST_EVAL_FAIL;
  if (result == BZEN_TEST_EVAL_FAIL) 
    {
      /* Fix NULL message. */
      if (error_message != NULL)
	{
	  msg_out = error_message;
	}
      else 
	{
	  msg_out = "\0";
	}
      /* @todo: unit test logging. */
      char* str_exp = (expected == 0) ? "false" : "true";
      char* str_act = (actual == 0) ? "false" : "true";
      fprintf(stderr, "Call to %s() expected: %s; returned: %s. %s",
	      fn_name,
	      str_exp,
	      str_act,
	      msg_out);
      result = BZEN_TEST_EVAL_FAIL;
    }
  return result;
}

/* Evaluates result of test on funtion returning integer. */
int bzen_test_eval_fn_int(char* fn_name, int expected, int actual, char* error_message)
{
  int result = 0;
  char* msg_out;

  /* Fix NULL message. */
  if (error_message != NULL)
    {
      msg_out = error_message;
    }
  else 
    {
      msg_out = "\0";
    }

  if (expected != actual) {
    /* @todo: unit test logging. */
    fprintf(stderr, "Call to %s() expected: %d; returned: %d. %s\n",
	    fn_name,
	    expected,
	    actual,
	    msg_out);
    result = 1;
  }
  return result;
}

/* Log a message indicating test failure and exit the test program. */
void bzen_test_exit_fail(const char* filename, int linenum)
{
  fprintf(stderr, "\nTest terminated with FAIL status near line %d in file: \n%s\n",
	  linenum, filename);
  exit(BZEN_TEST_EVAL_FAIL);
}

/* Exit the test program indicating PASS status. */
void bzen_test_exit_pass()
{
  exit(BZEN_TEST_EVAL_PASS);
}

/* Prints a generic message to stderr. */
void bzen_test_print_fail(const char* filename, int linenum)
{
  fprintf(stderr, "\nTest FAIL condition raised near line %d in file: \n%s\n",
	  linenum, filename);
}
