/**
 * @file:	bzentest_socket_create_local.c
 * @brief:	Unit test create socket in local namespace.
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
  int namespace = PF_LOCAL;
  int style = SOCK_DGRAM;
  int protocol = 0;

  /* create the socket */
  int sock = bzen_create_socket(namespace, style, protocol);
  if (sock < 0) 
  {
    fprintf (stderr, "bzen_create_socket(PF_LOCAL, SOCK_DGRAM, 0)\n");
    result = 1;
  }
  else 
  {
    bzen_close_socket(sock, SHUT_RDWR);
  }
  return result;
}
