/**
 * @file:	bzensock.h
 * @brief:	Portability API for POSIX/Windows sockets.
 * 
 * @todo: V. 0.1.2
 * Wrappers only. Support for POSIX connectionless sockets only.
 * socket(), bind(), send(), sendto(), recv(), recvfrom(), shutdown()
 * sockaddr_un
 *
 * @todo: V. 0.1.3
 * Wrappers only. Support for POSIX Internet sockets added.
 * listen(), accept(), connect()
 * sockaddr_in
 *
 * @todo: V. 0.1.4
 * threadsafe support and clean up
 *
 * @todo: V. 0.1.5
 * add sockaddr_in6 and host names support
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
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* libzenc includes */

/* A default prefix for things such as local addresses. */
#define BZENSOCK_PREFIX "BZENSOCK_"

/* socket is on server or client end of connection. */
#define BZENSOCK_SERVER 0x00000100
#define BZENSOCK_CLIENT 0x00001000

/**
 * Accept connection request.
 *
 * @param int socket_fd 
 * @param struct sockaddr* address Optional, out
 * @param socklen_t* address_size Optional, out
 * 
 * @return int File descriptor of server side connection or -1.
 */
int bzen_socket_accept(int socket_fd, 
		       struct sockaddr* address, 
		       socklen_t* address_size);

/**
 * Create an internet socket address.
 * 
 * @param uint32_t host Binary representation of IPv4 address.
 * @param uint32_t port
 *
 * @return struct sockaddr_in*
 */
struct sockaddr_in* bzen_socket_address_in(uint32_t host, uint32_t port);

/**
 * Create a local socket address.
 * 
 * @param const char* name Name socket address is registered under.
 * @param socklen_t* address_size Optional, returns size of address.
 *
 * @return struct sockaddr_un* The newly created address.
 */
struct sockaddr_un* bzen_socket_address_un(const char* name);

/**
 * @todo: struct sockaddr_in6* bzen_socket_adrdess_in6();
 */

/**
 * Bind a socket to the given address.
 *
 * @param int socket_fd 
 * @param struct sockaddr* address
 * @param socklen_t address_size
 * 
 * @return int 0 on success and -1 on failure. 
 */
int bzen_socket_bind(int socket_fd, 
		     struct sockaddr* address, 
		     socklen_t address_size);

/**
 * Close given socket.
 *
 * @param int socket_fd File descriptor of socket to shutdown.
 * @param int how SHUT_RD | SHUT_WR | SHUT_RDWR
 *
 * @return int 0 on success and -1 on failure.
 */
int bzen_socket_close(int socket_fd, int how);

/**
 * Request connection with  socket at  given address.
 *
 * @param int socket_fd 
 * @param struct sockaddr* address
 * @param socklen_t address_size
 * 
 * @return int 0 on success and -1 on failure. 
 */
int bzen_socket_connect(int socket_fd, 
			struct sockaddr* address, 
			socklen_t address_size);

/**
 * Enable connection requests on server socket.
 *
 * @param int socket_fd File decriptor of server socket.
 * @param int queue_len Number of connection requests to accept.
 *
 * @return int 0 on success and -1 on failure.
 */
int bzen_socket_listen(int socket_fd, int queue_len);

/**
 * Open a socket.
 *
 * @param int namespace
 * @param int style
 * @param int protocol
 *
 * @return int File descriptor of the newly opened socket.
 */
int bzen_socket_open(int namespace, int style, int protocol);

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
