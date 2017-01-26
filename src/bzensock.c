/**
 * @file:	bzensock.c
 * @brief:	Portability API for POSIX/Windows sockets.
 * 
 * @see:	https://www.gnu.org/software/libc/manual
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
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>

#include "bzensock.h"

/* Create a socket on target OS. */

int bzen_create_socket (int namespace, int style, int protocol)
{
  int result = EXIT_FAILURE;
  
  switch (namespace) 
  {
    default:
      break;
    case PF_LOCAL:
    case PF_INET:
      /**
       * @todo: protocol validation 
       * At present we only accept default (0) protocol for 
       * given namespace.
       */
      if (0 != protocol) 
      {
        break;
      }
      
      /**
       * @todo: style validation support for SOCK_RAW
       */
      if ((SOCK_STREAM != style) || (SOCK_DGRAM != style))
      {
        break;
      }

      /* Create the socket. */
      result = socket(namespace, style, protocol);
      if (result < 0)
      {
        /*
         * @todo: result = EPROTONOSUPPORT (The protocol or style is not 
         *    supported by the namespace specified).
         */

        /*
         * @todo: result = EMFILE (The process already has too many file
         *    descriptors open).
         */

        /*
         * @todo: result = ENFILE (The system already has too many file 
         *    descriptors open).
         */


        /*
         * @todo: result = EACCES (The process does not have the privilege 
         *    to create a socket of the specified style or protocol).
         */

        /*
         * @todo: result = ENOBUFS (The system ran out of internal buffer space).
         */
        perror("socket");
      }
      break;
  }
  return result;
}

/**
 * Close given socket on target OS.
 */
int bzen_close_socket(int descriptor, int how) 
{
  int result = EXIT_FAILURE;
  
  switch (how) 
  {
    default:
      break;
    case SHUT_RD:
    case SHUT_WR:
    case SHUT_RDWR:
      /* close the socket */
      result = shutdown(descriptor, how);
      if (0 != result) 
      {
        /*
         * @todo: result = EBADF (socket is not a valid file descriptor).
         */

        /*
         * @todo: result = ENOTSOCK (socket is not a socket).
         */

        /*
         * @todo: result = ENOTCONN (socket is not connected).
         */
        perror("socket");
      }      
      break;
  }
  return result;
}
