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

#include <config.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* libzenc includes */
#include "bzenmem.h"
#include "bzentest.h"
#include "bzensock.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int style = SOCK_DGRAM; /* SOCK_STREAM; */
  int protocol = 0;
  int socket_fd;
  int check;
  char* socket_dir;
  char* socket_file;
  char* socket_suffix;
  char* socket_full_path;
  int socket_full_path_size;
  
  /* generate local socket address from environment variables. */
  socket_dir = getenv("BZENTEST_TEMP_DIR");
  socket_file = getenv("BZENTEST_SOCKET_LOCAL_ADDR");
  socket_full_path_size = xcast_size_t(strlen(socket_dir) + 
    strlen(socket_file) + 
    strlen(PATH_DELIMITER) + 1);
  socket_full_path = (char*)bzen_malloc(socket_full_path_size);
  sprintf(socket_full_path, "%s%s%s", 
	  socket_dir,
	  PATH_DELIMITER,
	  socket_file);

  /* Test address management functions*/
  struct sockaddr_un* address = bzen_socket_create_address_un(socket_full_path);
  if (0 != strcmp(socket_full_path, address->sun_path))
    {
      fprintf(stderr, "\nbzen_socket_create_address_un path mismatch:\n%s != \n%s\n",
  		socket_full_path, address->sun_path);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_int("bzen_socket_create_address_un path()",
						    address->sun_family,
						    AF_LOCAL,
						    NULL))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  struct sockaddr_un* clone = bzen_socket_clone_address_un(address);
if (0 != strcmp(socket_full_path, clone->sun_path))
    {
      fprintf(stderr, "\nbzen_socket_clone_address_un path mismatch:\n%s != \n%s\n",
  		socket_full_path, clone->sun_path);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_int("bzen_socket_clone_address_un path()",
						    clone->sun_family,
						    AF_LOCAL,
						    NULL))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* create the socket */
  socket_fd = bzen_socket_listen_local(style, protocol, socket_full_path);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_listen_local()",
						    (socket_fd < 0),
						    0,
						    NULL))
    {
      fprintf(stderr, "failed to open local socket; return: %d\n", (int) socket_fd);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  
  /* bzen_socket_listen_local() should have registered socket address data. */
  struct bzen_sockaddr_ext* sockaddr_ext =
    bzen_socket_register_address_fetch(socket_full_path);
  if (sockaddr_ext == NULL)
    {
      fprintf(stderr, "Failed to fetch registered extended socket address data for %s.", socket_full_path);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  if (0 != strcmp(socket_full_path, sockaddr_ext->name))
    {
      fprintf(stderr, "\nbzen_socket_register_address_fetch name mismatch:\n%s != \n%s\n",
  		socket_full_path, address->sun_path);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_int("bzen_socket_register_address_fetch()",
  						    sockaddr_ext->socket_fd,
  						    socket_fd,
  						    " - socket file descriptor mismatch.\n"))
    {
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
