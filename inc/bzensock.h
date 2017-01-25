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
# define _BZENLIBC_SOCK_H_

#include <errno.h>
#include <sys/socket.h>

/**
 * @typedef: BZENSOCK_STOP_RECV 0
 * 
 * Stop receiving data for this socket. If further data arrives, reject it.
 */
#define BZENSOCK_STOP_RECV 0

/**
 * @typedef: BZENSOCK_STOP_SEND 1
 * 
 * Stop trying to transmit data from this socket. Discard any data waiting 
 * to be sent. Stop looking for acknowledgement of data already sent; don’t 
 * retransmit it if it is lost.
 */
#define BZENSOCK_STOP_SEND 1

/**
 * @typedef: BZENSOCK_STOP_BOTH 2
 * 
 * Stop both reception and transmission.
 */
#define BZENSOCK_STOP_BOTH 2

/**
 * Create a socket on target OS.
 * 
 * @param int namespace Specifies local or Internet namepsace.
 * @param int style Specifies the communication style.
 * @param int protocol Designates the specific protocol.
 *
 * @return int File descriptor for the newly created socket.
 */
int bzen_create_socket(int namespace, int style, int protocol);

/**
 * Close given socket on target OS.
 * 
 * @param int descriptor File descriptor of socket to close.
 * @param int how BZENSOCK_STOP_RECV | BZENSOCK_STOP_SEND | BZENSOCK_STOP_BOTH
 * 
 * @return int 0 on success and -1 on failure
 */
int bzen_close_socket(int descriptor, int how);

#endif /* _BZENLIBC_SOCK_H_ */
