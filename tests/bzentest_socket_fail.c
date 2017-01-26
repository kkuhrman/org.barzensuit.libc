/**
 * @file:	bzentest_socket_create_local.c
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bzensock.h"

int main (int argc, char *argv[])
{
  int result = 0;
  int bogus_namespace = AF_LOCAL | AF_UNIX | AF_FILE | AF_INET | AF_INET6 | AF_UNSPEC;
  int bogus_style = SOCK_STREAM | SOCK_DGRAM | SOCK_RAW;
  int bogus_protocol = -1;
  int sock;

  /* Attempt to create the socket using combinations of valid and 
   * bogus parameters.
   */
  sock = bzen_create_socket(AF_LOCAL, bogus_style, 0);
  if ((sock< 0) && (errno != EPROTONOSUPPORT))
  {
    fprintf (stderr, "bzen_create_socket(AF_LOCAL, bogus_style, 0)\n");
    result = 1;
  }
  sock = bzen_create_socket(bogus_namespace, SOCK_DGRAM, 0);
  if ((sock< 0) && (errno != EPROTONOSUPPORT))
  {
    fprintf (stderr, "bzen_create_socket(bogus_namespace, SOCK_DGRAM, 0)\n");
    result = 1;
  }
  sock = bzen_create_socket(AF_LOCAL, SOCK_DGRAM, bogus_protocol);
  if ((sock< 0) && (errno != EPROTONOSUPPORT))
  {
    fprintf (stderr, "bzen_create_socket(AF_LOCAL, SOCK_DGRAM, bogus_protocol)\n");
    result = 1;
  }
  
  /*
   * Create a valid, open socket.
   */
  sock = bzen_create_socket(PF_LOCAL, SOCK_DGRAM, 0);
  if (sock < 0)
  {
    fprintf(stderr, "bzen_create_socket(PF_LOCAL, SOCK_DGRAM, 0)\n");
    result = 1;
  }

  
  /*
   * Attempt to close valid, open socket with combinations of valid 
   * and bogus parameters.
   */
  if (sock < 0)
  {
    /* Just close it already. */
    bzen_close_socket(sock, SHUT_RDWR);
  }
  return result;
}
