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
#include <sys/un.h>
#include <dirent.h>

/* libzenc includes */
#include "bzenmem.h"
#include "bzensock.h"

/* Close given socket on target OS. */
int bzen_socket_close(int socket_fd, int how)
{
  int result = -1;
  
  switch (how) 
    {
    default:
      break;
    case SHUT_RD:
    case SHUT_WR:
    case SHUT_RDWR:
      /* close the socket */
      result = shutdown(socket_fd, how);
      if (result != 0) 
	{
	  /* @todo: error logging. */
	  perror("shutdown");
	  goto SOCK_FAIL;
	}      
      break;
    }
  /* @todo: if socket is local, unlink file. */
  /* unlink(address.sun_path); */

 SOCK_FAIL:
  return result;
}

/*  Open a socket to connect to socket server (client). */
int bzen_socket_connect(int socket_fd, const char* address)
{
}

/* Open a socket to listen on network address (server). */
int bzen_socket_listen_inet(int style, 
			    int protocol, 
			    const char* host,
			    unsigned short int port,
			    int format)
{
}

/* Open a socket to listen on local address (server).*/
int bzen_socket_listen_local(int style, int protocol, const char* filename)
{
  int socket_fd;
  int af_supported;
  char* socket_dir;
  char* socket_file;
  struct sockaddr* socket_addr;
  size_t socket_addr_size;
  int bind_result;
  
  /* Validate communication style is supported. */
  switch (style)
    {
    default:
      af_supported = 0;
      break;
    case SOCK_DGRAM:
    case SOCK_STREAM:
      af_supported = 1;
      break;
    }
  
  if (0 == af_supported)
    {
      /* @todo: error logging. */
      socket_fd = -1;
      goto SOCK_FAIL;
    }
  
  /* Create the socket. */
  socket_fd = socket(PF_LOCAL, style, protocol);
  if (socket_fd < 0) 
    {
      /* @todo: error logging. */
      shutdown(socket_fd, SHUT_RDWR);
      socket_fd = -1;
      goto SOCK_FAIL;
    }
  
  /* @todo: filename is currrently ignored. */
  
  /* @todo: environment variable for application temp dir. */
  socket_dir = getenv("BZENTEST_TEMP_DIR");
  socket_file = getenv("BZENTEST_TEMP_DIR");
  
  /* Populate socket address. */
  if (style == SOCK_DGRAM)
    {
      struct sockaddr_un dgram_address;
      dgram_address.sun_family = AF_LOCAL;
      strncpy(dgram_address.sun_path, socket_dir, sizeof(dgram_address.sun_path));
      dgram_address.sun_path[sizeof(dgram_address.sun_path) - 1] = '\0';
      socket_addr_size = SUN_LEN (&dgram_address);
      socket_addr = (struct sockaddr*) &dgram_address;
    }
  /* @todo: else SOCK_STREAM */
  
  /* Attempt to bind socket to address. */
  bind_result = bind(socket_fd, socket_addr, socket_addr_size);
  if (socket_fd < 0) 
    {
      /* @todo: error logging. */
      shutdown(socket_fd, SHUT_RDWR);
      socket_fd = -1;
      goto SOCK_FAIL;
    }

 SOCK_FAIL:
  /* @todo: on fail. */

  return socket_fd;
}
