/**
 * @file:	bzentest.h
 * @brief:	Helper functions for unit testing.
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

#ifndef _BZENLIBC_TEST_H_
#define _BZENLIBC_TEST_H_

/**
 * Some POSIX systems standardize on zero as success
 * and nonzero as fail, based on the convention of 
 * main() returning similar results. boolean tests 
 * tend to expect the opposite: zero for false and 
 * nonzero for true. These constants are provided to
 * avoid some confusion over this nuance in code.
 */
const unsigned short int BZEN_TEST_EVAL_PASS = 0;
const unsigned short int BZEN_TEST_EVAL_FAIL = 1;
#define BZEN_TEST_EVAL_PASS BZEN_TEST_EVAL_PASS
#define BZEN_TEST_EVAL_FAIL BZEN_TEST_EVAL_FAIL

const unsigned short int BZEN_TEST_BOOL_TRUE = 1;
const unsigned short int BZEN_TEST_BOOL_FALSE = 0;
#define BZEN_TEST_BOOL_TRUE BZEN_TEST_BOOL_TRUE
#define BZEN_TEST_BOOL_FALSE BZEN_TEST_BOOL_FALSE
#define BZEN_TEST_BOOL bzen_test_bool

/**
 * Casts any integer as a pseudo boolean type.
 *
 * @param int value Integer value to evaluate.
 *
 * @return int 1 or value is nonzero otherwise 0.
 */
int bzen_test_bool(int value);

/**
 * Evaluates result of test on function returning boolean.
 *
 * In Gnu C true and false are nonzero and zero respectively.
 *
 * @param char* fn_name Name of function.
 * @param int expected Expected return value.
 * @param int actual Actual return value.
 * @param char* error_message Additional text to log on error.
 *
 * @return int 0 on success, 1 on fail.
 */
int bzen_test_eval_fn_bool(char* fn_name, int expected, int actual, char* error_message);

/**
 * Evaluates result of test on funtion returning integer.
 *
 * @param char* fn_name Name of function.
 * @param int expected Expected return value.
 * @param int actual Actual return value.
 * @param char* error_message Additional text to log on error.
 *
 * @return int 0 on success, 1 on fail.
 */
int bzen_test_eval_fn_int(char* fn_name, int expected, int actual, char* error_message);

#endif /* _BZENLIBC_TEST_H_ */
