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

/** 
 * Internal registry for extended socket address data. 
 * @see gnulib/xalloc.h
 */
struct bzen_sockaddr_ext* bzen_sockaddr_registry = NULL;
size_t bzen_sockaddr_registry_size = 0;
size_t bzen_sockaddr_registry_used = 0;

/**
 * Checks if socket at local address (file) is accessible.
 * 
 * @param const char* filename The local socket 'address'.
 * @param int how R_OK, W_OK, X_OK, F_OK (exists).
 * 
 * @return 0 if access is permitted in given mode, otherwise -1.
 */
int bzen_socket_access_local(const char* filename, int how) 
{
  int result = access(filename, how);
  return result;
}

/* Clone the a local socket address. */
struct sockaddr_un* bzen_socket_clone_address_un(struct sockaddr_un* address)
{
  struct sockaddr_un* clone;

  if (address == NULL)
    {
      clone = NULL;
      goto CLONE_FAIL;
    }
 
  clone = bzen_socket_create_address_un(address->sun_path);

 CLONE_FAIL:

  return clone;
}

/* Clone the an Internet IPv4  socket address. */
struct sockaddr_in* bzen_socket_clone_address_in(struct sockaddr_in* address)
{
  return NULL;
}

/* Clone the an Internet IPv6 socket address. */
struct sockaddr_in6* bzen_socket_clone_address_in6(struct sockaddr_in6* address)
{
  return NULL;
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

/* Create a local socket address. */
struct sockaddr_un* bzen_socket_create_address_un(const char* name)
{
  struct sockaddr_un* address;
  
  if (name == NULL)
    {
      address = NULL;
      goto ADDR_FAIL;
    }

  /* Allocate memory for the data structure. */  
  size_t address_size = xcast_size_t(sizeof(struct sockaddr_un));  
  address = (struct sockaddr_un*)bzen_malloc(address_size);

  /* Allocate memory with data structure for the path. */
  size_t path_size = xcast_size_t(strlen(name));
  if (path_size > xcast_size_t(sizeof(address->sun_path)))
    {
      /* @todo: Gnu sockets use static allocation for path.
         not sure of workaround for path names exceeding max path. */
      address = NULL;
      goto ADDR_FAIL;
    }
  memcpy(address->sun_path, name, path_size);
  /* yes, offset + path_size is terminating null (not path_size - 1). */
  address->sun_path[path_size] = '\0';
  address->sun_family = AF_LOCAL;

 ADDR_FAIL:

  return address;
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
  
  
  /* Populate socket address. */
  if (style == SOCK_DGRAM)
    {
      socket_addr = (struct sockaddr*)bzen_socket_create_address_un(filename);
    }
  /* @todo: else SOCK_STREAM */
  
  if (0 != bzen_socket_register_address(filename, socket_fd, socket_addr))
    {
      /* @todo: error logging. */
      shutdown(socket_fd, SHUT_RDWR);
      socket_fd = -1;
      goto SOCK_FAIL;
    }

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

/*  Stores given socket address in memory. */
int bzen_socket_register_address(const char* name, 
				 int socket_fd, 
				 struct sockaddr* address)
{
  struct bzen_sockaddr_ext* paddress;
  struct bzen_sockaddr_ext* bzen_sockaddr_registry_it;
  int result = 0;
  
  /* Verify registry has been initialized. */
  if (bzen_sockaddr_registry_size == bzen_sockaddr_registry_used)
    {
      bzen_sockaddr_registry_size += BZENSOCK_REGISTRY_PAGE_SIZE;
      bzen_sockaddr_registry = 
	(struct bzen_sockaddr_ext*)bzen_realloc(bzen_sockaddr_registry,
						&bzen_sockaddr_registry_size,
						xcast_size_t(sizeof(struct bzen_sockaddr_ext*)));
    }
    
  /* Register extended address data. */
  bzen_sockaddr_registry[bzen_sockaddr_registry_used++] = *(struct bzen_sockaddr_ext*)
    bzen_malloc(xcast_size_t(sizeof(struct bzen_sockaddr_ext)));
  paddress = &bzen_sockaddr_registry[bzen_sockaddr_registry_used - 1];
  size_t name_size = xcast_size_t(strlen(name));
  paddress->name = bzen_malloc(name_size);
  strncpy(paddress->name, name, name_size);
  paddress->socket_fd = socket_fd;
  paddress->address = address;
  
  return result;
}

/* Fetch socket address from internal registry in memory. */
struct bzen_sockaddr_ext* bzen_socket_register_address_fetch(const char* name)
{
  struct bzen_sockaddr_ext* paddress = NULL;
  int index = 0;

  while (index < bzen_sockaddr_registry_used)
    {
      paddress = bzen_sockaddr_registry;
      if ((paddress->name != NULL) && (0 == strcmp(paddress->name, name)))
	break;
      paddress++;
    }
  
  return paddress;
}
