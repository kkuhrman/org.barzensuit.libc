/**
 * @file:	bzentest_socket_create_local.c
 * @brief:	Unit test create socket in local namespace.
 *
 * In actuality, this unit test comprises creating, binding and 
 * closing a local socket. All three functions must succeed to PASS.
 *
 * For unit tests of error conditions:
 * @see:	bzentest_socket_fail.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bzentest.h"
#include "bzensock.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int style = SOCK_DGRAM; /* SOCK_STREAM; */
  int protocol = 0;
  int socket_fd;
  int check;

  /* create the socket */
  socket_fd = bzen_socket_listen_local(style, protocol, NULL);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_listen_local()",
						    (socket_fd < 0),
						    0,
						    NULL))
    {
      fprintf(stderr, "failed to open local socket; return: %d\n", (int) socket_fd);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  
  /* @todo: create client socket. */
  
  /* @todo: client send, server read */
  
  /* @todo: server send, client read */
    
  /* Close the socket. */
  check = bzen_socket_close(socket_fd, SHUT_RDWR);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_close()",
						    (check == 0),
						    1,
						    NULL)) 
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

 END_TEST:
  return result;
}
