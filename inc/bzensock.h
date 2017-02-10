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
#include <sys/un.h>
#include <unistd.h>

/* libzenc includes */
#include "bzenchar.h"

/* A default prefix for things such as local addresses. */
const char* BZENSOCK_PREFIX = "BZENSOCK_";
#define BZENSOCK_PREFIX BZENSOCK_PREFIX

/* socket is on server or client end of connection. */
const int BZENSOCK_SERVER = 0x00000100;
const int BZENSOCK_CLIENT = 0x00001000;
#define BZENSOCK_SERVER BZENSOCK_SERVER
#define BZENSOCK_CLIENT BZENSOCK_CLIENT

/**
 * Create a local socket address.
 * 
 * @param const char* name Name socket address is registered under.
 *
 * @return struct sockaddr_un* The newly created address.
 */
struct sockaddr_un* bzen_socket_address_un(const char* name);

/**
 * Open local socket for sending individually-addressed packets unreliably.
 *
 * @param const char* filename The local socket 'address'.
 * @param int protocol Designates the specific protocol.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_socket_dgram_local(const char* filename, int protocol);

/**
 * Accepts a connection request from a local client.
 *
 * @param int socket_fd File descriptor of server accepting the connection.
 * @param const char* filename Local address of client.
 *
 * @return int File descriptor of new, connected server-side socket.
 */
/* int accept (int socket_fd, const char* filename); */

/**
 * Checks if socket at local address (file) is accessible.
 * 
 * @param const char* filename The local socket 'address'.
 * @param int how R_OK, W_OK, X_OK, F_OK (exists).
 * 
 * @return 0 if access is permitted in given mode, otherwise -1.
 */
int bzen_socket_access_local(const char* filename, int how);

/**
 * Clone the an Internet IPv4  socket address.
 * 
 * @param struct sockaddr_in* address The address to clone.
 *
 * @return struct sockaddr_in* The cloned address.
 */
struct sockaddr_in* bzen_socket_clone_address_in(struct sockaddr_in* address);

/**
 * Clone the an Internet IPv6 socket address.
 * 
 * @param struct sockaddr_in6* address The address to clone.
 *
 * @return struct sockaddr_in6* The cloned address.
 */
struct sockaddr_in6* bzen_socket_clone_address_in6(struct sockaddr_in6* address);

/**
 * Open a socket to connect to socket server (client).
 *
 * @param const char* name Name socket was registered under.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_socket_connect(const char* name);

/**
 * Connect to socket listening on local address (server).
 *
 * @param int style Specifies the communication style.
 * @param int protocol Designates the specific protocol.
 * @param const char* filename The local socket 'address'.
 *
 * @return int File descriptor for newly created client socket.
 */
int bzen_socket_connect_local(int style, int protocol, const char* filename);

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
 * Open a socket on local address and put in listening mode (server).
 *
 * @param int style Specifies the communication style.
 * @param int protocol Designates the specific protocol.
 * @param const char* filename The local socket 'address'.
 * @param int qlen Length of queue for pending connections.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_socket_listen_local(int style, int protocol, const char* filename, int qlen);

/**
 * Open a socket and register on local address.
 *
 * @param int style Specifies the communication style.
 * @param int protocol Designates the specific protocol.
 * @param const char* filename The local socket 'address'.
 *
 * @return int File descriptor for the newly created socket.
 */
static int bzen_socket_open_local(int style, int protocol, const char* filename);

/**
 * Receive data from connected peer.
 *
 * @param in socket_fd File descriptor of receiving socket.
 * @param void* buffer Pointer to buffer serving as data sink.
 * @param size_t data_size Number of bytes to read.
 * @param int flags MSG_OOB | MSG_PEEK | MSG_DONTROUTE | 0.
 * 
 * @return size_t Number of bytes received.
 */
ssize_t bzen_socket_receive(int socket_fd, void* data, size_t data_size, int flags);

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
				 socklen_t* address_size);

/**
 * Send data to connected peer.
 *
 * @param int socket_fd File descriptor of transmitting socket.
 * @param const void* data Pointer to buffer containing transmission data.
 * @param size_t data_size Size in bytes of data to transmit.
 * @param int flags MSG_OOB | MSG_DONTROUTE | 0
 *
 * @return size_t Number of bytes transmitted.
 */
ssize_t bzen_socket_send(int socket_fd, const void* data, size_t data_size, int flags);

/**
 * Send data to connectionless peer.
 *
 * @param int socket_fd File descriptor of transmitting socket.
 * @param const void* data Pointer to buffer containing transmission data.
 * @param size_t data_size Size in bytes of data to transmit.
 * @param int flags MSG_OOB | MSG_DONTROUTE | 0
 * @param struct sockaddr* address Address of destination socket.
 * @param socklen_t address_size Size in bytes of destination address.
 *
 * @return size_t Number of bytes transmitted.
 */
ssize_t bzen_socket_send_to(int socket_fd, 
			   const void* data, 
			   size_t data_size, 
			   int flags,
			   struct sockaddr* address, 
			   socklen_t address_size);

#endif /* _BZENLIBC_SOCK_H_ */
