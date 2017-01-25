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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bzensock.h"

int main (int argc, char *argv[])
{
  int result = 0;
  int namespace = PF_INET;
  int style = SOCK_STREAM;
  int protocol = 0;

  /* create the socket */
  int sock = bzen_create_socket(namespace, style, protocol);
  if (sock < 0) 
  {
    /*
     * Permission denied is a common error on systems, which support 
     *  sockets, but are not properly configured to allow user to do so.
     */
    if (EACCES == sock) 
    {
      fprintf (stderr, "User denied permission to create Internet socket.\n");
    }
    fprintf (stderr, "bzen_create_socket(PF_INET, SOCK_STREAM, 0)\n");
    result = 1;
  }
  else 
  {
    bzen_close_socket(sock, BZENSOCK_STOP_BOTH);
  }
  return result;
}
