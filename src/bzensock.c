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
#include <dirent.h>

/* libzenc includes */
#include "bzenmem.h"
#include "bzensock.h"

/* Create a local socket address. */
struct sockaddr_un* bzen_socket_address_un(const char* name)
{
  struct sockaddr_un* address;
  size_t address_alloc_size;

  if (name == NULL)
    {
      fprintf(stderr, "\n\tCannot create named socket with NULL filename.\n");
      address = NULL;
      goto ADDR_FAIL;
    }

  /* Allocate memory for the data structure. */  
  address_alloc_size = xcast_size_t(sizeof(struct sockaddr_un));  
  address = (struct sockaddr_un*)bzen_malloc(address_alloc_size);
  memset(address, 0, address_alloc_size);

  /* Allocate memory with data structure for the path. */
  size_t path_size = xcast_size_t(strlen(name));
  if (path_size > xcast_size_t(sizeof(address->sun_path)))
    {
      /* @todo: Gnu sockets use static allocation for path.
         not sure of workaround for path names exceeding max path. */
      fprintf(stderr, "\n\tNamed socket filename exceeds max characters.\n\tAllowed: %d\n\tGiven: %d\n",
	      sizeof(address->sun_path), path_size);
      address = NULL;
      goto ADDR_FAIL;
    }
  address->sun_family = AF_LOCAL;
  memset(address->sun_path, 0, path_size);
  memcpy(address->sun_path, name, path_size);
  address->sun_path[path_size] = '\0';

 ADDR_FAIL:

  return address;
}

/* Bind a socket to the given address. */
int bzen_socket_bind(int socket_fd, 
		     struct sockaddr* address, 
		     socklen_t address_size)
{
  int result;
  
  result = bind(socket_fd,
		     address, 
		     address_size);
  if (result < 0)
    {
      perror("bind");
      fprintf(stderr, "\n\tfd: %d\n\taddress: %s\n\tsize: %d\n",
  	      socket_fd,
  	      address->sa_data,
  	      address_size);
      shutdown(socket_fd, SHUT_RDWR);
      goto BIND_FAIL;
    }

 BIND_FAIL:

  return result;
}

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
	  perror("shutdown");
	  goto CLOSE_FAIL;
	}      
      break;
    }

 CLOSE_FAIL:

  return result;
}

/* Open a socket. */
int bzen_socket_open(int namespace, int style, int protocol)
{
  int socket_fd;

  /* Create the socket. */
  socket_fd = socket(namespace, style, protocol);
  if (socket_fd < 0) 
    {
      perror("socket");
      shutdown(socket_fd, SHUT_RDWR);
      socket_fd = -1;
      goto SOCK_FAIL;
    }

 SOCK_FAIL:

  return socket_fd;
}

/* Receive data from connected peer. */
ssize_t bzen_socket_receive(int socket_fd, void* data, size_t data_size, int flags)
{
  ssize_t result;

  result = recv(socket_fd, data, data_size, flags);
  if (result != data_size)
    {
      perror("recv");
      goto TXMT_FAIL;
    }

 TXMT_FAIL:

  return result;
}

/**
 * Receive data from connectionless peer.
 *
 * The address and address size parameters are optional and only used if it is 
 * necessary to know the address of the sender. Otherwise bzen_socket_receive()
 * should be used.
 *
 * @param in socket_fd File descriptor of receiving socket.
 * @param void* buffer Pointer to buffer serving as data sink.
 * @param size_t data_size Number of bytes to read.
 * @param int flags MSG_OOB | MSG_PEEK | MSG_DONTROUTE | 0.
 * @param struct sockaddr* address Address of sending socket.
 * @param socklen_t* address_size Size in bytes of sending address.
 * 
 * @return size_t Number of bytes received.
 */
ssize_t bzen_socket_receive_from(int socket_fd, 
				 void* data, 
				 size_t data_size, 
				 int flags,
				 struct sockaddr* address,
				 socklen_t* address_size)
{
  ssize_t result;

  result = recvfrom(socket_fd, data, data_size, flags, address, address_size);
  if (result != data_size)
    {
      perror("recv");
      goto TXMT_FAIL;
    }

 TXMT_FAIL:

  return result;
}

/* Send data to connected peer. */
ssize_t bzen_socket_send(int socket_fd, const void* data, size_t data_size, int flags)
{
  ssize_t result;

  result = send(socket_fd, data, data_size, flags);
  if (result != data_size)
    {
      perror("send");
      goto TXMT_FAIL;
    }

 TXMT_FAIL:

  return result;
}

/* Send data to connectionless peer. */
ssize_t bzen_socket_send_to(int socket_fd, 
			   const void* data, 
			   size_t data_size, 
			   int flags,
			   struct sockaddr* address, 
			   socklen_t address_size)
{
  ssize_t result;

  /* @todo: before business */

  result = sendto(socket_fd, data, data_size, flags, address, address_size);
  if (result != data_size)
    {
      perror("sendto");
      goto TXMT_FAIL;
    }

  /* @todo: after business */

 TXMT_FAIL:

  return result;
}
