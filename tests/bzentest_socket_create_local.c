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

/* A random sequence of characters to transmit as test data. */
const char* BZENSOCK_TEST_DATA = "e5a8f08d-d640-4523-8b84-99602f2a60b1";
#define BZENSOCK_TEST_DATA BZENSOCK_TEST_DATA

/* Default size of transmission buffers. */
const size_t BZENSOCK_DEFAULT_BUFFER_SIZE = 1024;
#define BZENSOCK_DEFAULT_BUFFER_SIZE BZENSOCK_DEFAULT_BUFFER_SIZE

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int namespace = PF_LOCAL;
  int style = SOCK_DGRAM; /* SOCK_STREAM; */
  int protocol = 0;
  char* socket_dir;

  size_t bytes_out;
  size_t bytes_in;
  size_t data_size;
  size_t address_size_check;
  int flags = 0;
  int check;
  
  /* server socket */
  int server_socket_fd;
  const char* server_socket_file = "lserver";
  char* server_socket_full_path;
  int server_socket_full_path_size;
  struct sockaddr_un* server_address;
  size_t server_address_size;
  char s_buf[BZENSOCK_DEFAULT_BUFFER_SIZE];

  /* client socket*/
  int client_socket_fd;
  const char* client_socket_file = "lclient";
  char* client_socket_full_path;
  int client_socket_full_path_size;
  struct sockaddr_un* client_address;
  socklen_t client_address_size;
  char c_buf[BZENSOCK_DEFAULT_BUFFER_SIZE];

  /* server address. */
  socket_dir = getenv("BZENTEST_TEMP_DIR");
  server_socket_full_path_size = xcast_size_t(strlen(socket_dir) + 
    strlen(server_socket_file) + 
    strlen(BZEN_PATH_DELIMITER) + 1);
  server_socket_full_path = (char*)bzen_malloc(server_socket_full_path_size);
  sprintf(server_socket_full_path, "%s%s%s", 
	  socket_dir,
	  BZEN_PATH_DELIMITER,
	  server_socket_file);
  unlink(server_socket_full_path);
  server_address = bzen_socket_address_un(server_socket_full_path);
  server_address_size = SUN_LEN(server_address);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_address_un()",
						    (server_address != NULL),
						    1,
						    "\n\tserver address\n"))
    {
      fprintf(stderr, "\n\tserver address @ %d\n", server_address);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* create server socket */
  server_socket_fd = bzen_socket_open(namespace, style, protocol);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_open()",
						    (server_socket_fd < 0),
						    0,
						    "\n\tserver socket\n"))
    {
      fprintf(stderr, "failed to open local socket; return: %d\n", (int) server_socket_fd);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* bind server socket to address */
  check = bzen_socket_bind(server_socket_fd, (struct sockaddr*)server_address, server_address_size);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_bind()",
						    (check < 0),
						    0,
						    "\n\tserver socket\n"))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* client address. */
  ;  client_socket_full_path_size = xcast_size_t(strlen(socket_dir) + 
    strlen(client_socket_file) + 
    strlen(BZEN_PATH_DELIMITER) + 1);
  client_socket_full_path = (char*)bzen_malloc(client_socket_full_path_size);
  sprintf(client_socket_full_path, "%s%s%s", 
	  socket_dir,
	  BZEN_PATH_DELIMITER,
	  client_socket_file);
  unlink(client_socket_full_path);
  client_address = bzen_socket_address_un(client_socket_full_path);
  client_address_size = SUN_LEN(client_address);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_address_un()",
						    (client_address != NULL),
						    1,
						    "\n\tclient address\n"))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* create client socket */
  client_socket_fd = bzen_socket_open(namespace, style, protocol);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_open()",
						    (client_socket_fd < 0),
						    0,
						    "\n\tclient socket\n"))
    {
      fprintf(stderr, "failed to open local socket; return: %d\n", (int) client_socket_fd);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* bind client socket to address */
  check = bzen_socket_bind(client_socket_fd, (struct sockaddr*)client_address, client_address_size);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_bind()",
						    (check < 0),
						    0,
						    "\n\tclient socket\n"))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* client send, server read */
  /* data_size = xcast_size_t(strlen(BZENSOCK_TEST_DATA)); */
  /* bytes_out = bzen_socket_send_to(client_socket_fd, */
  /* 				  BZENSOCK_TEST_DATA, */
  /* 				  data_size, */
  /* 				  flags, */
  /* 				  (struct sockaddr*)client_address, */
  /* 				  client_address_size); */
  /* if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_send_to()", */
  /* 						    (bytes_out == data_size), */
  /* 						    1, */
  /* 						    "\n\tclient send\n")) */
  /*   { */
  /*     result = BZEN_TEST_EVAL_FAIL; */
  /*     goto END_TEST; */
  /*   } */

  /* Close the sockets. */
  check = bzen_socket_close(client_socket_fd, SHUT_RDWR);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_close()",
						    (check == 0),
						    1,
						    NULL)) 
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  check = bzen_socket_close(server_socket_fd, SHUT_RDWR);
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
