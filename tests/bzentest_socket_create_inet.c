/**
 * @file:	bzentest_socket_create_inet.c
 * @brief:	Unit test create socket in IPv4 Internet namespace.
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

/* unit test on loopback only */
const char* BZENSOCK_LOCAL_HOST = "127.0.0.1";
#define BZENSOCK_LOCAL_HOST BZENSOCK_LOCAL_HOST

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int namespace = PF_INET;
  int style = SOCK_STREAM;
  int protocol = 0;
  uint32_t host;
  uint32_t port;
  int format;
  int check;

  /* server socket*/
  int server_socket_fd;
  int server_side_connection_fd;
  struct sockaddr_in* server_address;
  size_t server_address_size;

  /* client socket */
  int client_socket_fd;
  
  /* construct server address */
  host = INADDR_LOOPBACK;
  port = IPPORT_USERRESERVED + 2048;
  server_address =   bzen_socket_address_in(host, port);
  server_address_size = sizeof(*server_address);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_address_in()",
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
      fprintf(stderr, "failed to open INET socket; return: %d\n", (int) server_socket_fd);
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

  /* enable connection requests */
  check = bzen_socket_listen(server_socket_fd, 1);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_listen()",
						    (check < 0),
						    0,
						    NULL))
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
      fprintf(stderr, "failed to open INET socket; return: %d\n", (int) client_socket_fd);
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }
  
  /* request connection with server  */
  check = bzen_socket_connect(client_socket_fd, (struct sockaddr*)server_address, server_address_size);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_connect()",
						    (check < 0),
						    0,
						    NULL))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

  /* accept connection request */
  struct sockaddr address_out;
  socklen_t address_out_size;
  server_side_connection_fd = bzen_socket_accept(server_socket_fd, 
						 &address_out,
						 &address_out_size);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_bind()",
						    (check < 0),
						    0,
						    "\n\tserver socket\n"))
    {
      result = BZEN_TEST_EVAL_FAIL;
      goto END_TEST;
    }

 END_TEST:

  /* Close the sockets. */
  check = bzen_socket_close(server_side_connection_fd, SHUT_RDWR);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_close()",
						    (check == 0),
						    1,
						    "\n\tserver-side connection\n")) 
    {
      result = BZEN_TEST_EVAL_FAIL;
    }
  check = bzen_socket_close(client_socket_fd, SHUT_RDWR);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_close()",
						    (check == 0),
						    1,
						    "\n\tclient\n")) 
    {
      result = BZEN_TEST_EVAL_FAIL;
    }
  check = bzen_socket_close(server_socket_fd, SHUT_RDWR);
  if (BZEN_TEST_EVAL_FAIL == bzen_test_eval_fn_bool("bzen_socket_close()",
						    (check == 0),
						    1,
						    "\n\tserver\n")) 
    {
      result = BZEN_TEST_EVAL_FAIL;
    }

  return result;
}
