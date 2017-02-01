/**
 * @file:	bzensock.h
 * @brief:	Portability API for POSIX/Windows sockets.
 * 
 * @see:	ftp://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_16.html
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

#ifndef _BZENLIBC_SOCK_H_
#define _BZENLIBC_SOCK_H_

#include <errno.h>
#include <sys/socket.h>
#include "bzenchar.h"

/* A default prefix for things such as local addresses. */
const char* BZENSOCK_PREFIX = "BZENSOCK_";
#define BZENSOCK_PREFIX BZENSOCK_PREFIX

/**
 * Close given socket on target OS.
 * 
 * Specify SHUT_RD for 'how' to stop receiving data for this socket. 
 * In this case, if further data arrives, reject it.
 *
 * Specify SHUT_WR for 'how' to stop trying to transmit data from this 
 * socket. Discard any data waiting to be sent. Stop looking for 
 * acknowledgement of data already sent; don’t retransmit it if it is lost.
 * 
 * Specify SHUT_RDWR for 'how' to stop both reception and transmission.
 * 
 * @see bzen_create_socket
 *
 * @param int descriptor File descriptor of socket to close.
 * @param int how SHUT_RD | SHUT_WR | SHUT_RDWR
 * 
 * @return int EXIT_SUCCESS | EXIT_FAILURE.
 */
int bzen_socket_close(int descriptor, int how);

/**
 * Open a socket to connect to socket server (client).
 *
 * @param int socket_fd File descriptor of server socket.
 * @param const char* address Address of server socket.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_socket_connect(int socket_fd, const char* address);

/**
 * Open a socket to listen on network address (server).
 *
 * @param int style Specifies the communication style.
 * @param int protocol Designates the specific protocol.
 * @param const char* host IP address or FQDN.
 * @param unsigned short int port Port number.
 * @param int format IPv4 or IPv6.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_socket_listen_inet(int style, 
			    int protocol, 
			    const char* host,
			    unsigned short int port,
			    int format);

/**
 * Open a socket to listen on local address (server).
 *
 * @param int style Specifies the communication style.
 * @param int protocol Designates the specific protocol.
 * @param const char* filename Reserved - should be NULL.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_socket_listen_local(int style, int protocol, const char* filename);

#endif /* _BZENLIBC_SOCK_H_ */
