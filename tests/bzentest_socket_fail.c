/**
 * @file:	bzentest_socket_fail.c
 * @brief:	Unit test graceful exit on failures.
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

/* libzenc includes */
#include "bzentest.h"
#include "bzensock.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int bogus_namespace = AF_LOCAL | AF_UNIX | AF_FILE | AF_INET | AF_INET6 | AF_UNSPEC;
  int bogus_style = SOCK_STREAM | SOCK_DGRAM | SOCK_RAW;
  int bogus_protocol = -1;
  int socket_fd;
  int test_result;

  /* Expected FAIL on create socket w. bogus address family. */
  socket_fd = bzen_socket_open(NULL, 0, NULL);
  if (1 != bzen_test_eval_fn_bool("bzen_create_socket", 0, ((socket_fd < 0) && (errno != EPROTONOSUPPORT)), NULL))
  {
    fprintf(stderr, "Expected FAIL on create socket w. bogus address family.");
    result = BZEN_TEST_EVAL_FAIL;
    goto END_TEST;
  }
  bzen_socket_close(socket_fd, SHUT_RDWR);

  /* Expected FAIL on create socket w. bogus protocol. */
  socket_fd = bzen_socket_open(SOCK_DGRAM, -1, NULL);
  if (1 != bzen_test_eval_fn_bool("bzen_create_socket", 0, ((socket_fd < 0) && (errno != EPROTONOSUPPORT)), NULL))
  {
    fprintf(stderr, "Expected FAIL on create socket w. bogus protocol.");
    result = BZEN_TEST_EVAL_FAIL;
    goto END_TEST;
  }
  /* @todo: close socket if more tests follow. */

  /* @todo: open valid socket and fail on bind. */

  /* @todo: fail on transmissions. */

  /* @todo: fail on close. */

END_TEST:
  bzen_socket_close(socket_fd, SHUT_RDWR);
  return result;
}
