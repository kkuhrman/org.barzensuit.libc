/**
 * @file:	bzensbuf.h
 * @brief:	Encapsulate thread-safe allocation/access of character buffers.
 * 
 * Project Barzensuit uses char buffers in memory as IO streams but leverages 
 * POSIX fmemopen() to cast the char* as type FILE*, thus allowing use of most
 * standard C stream features (internal pointers etc). The design drivers for
 * this are:
 * 1. IO associated with FILE on disk represents a huge performance hit.
 * 2. Most Project Barzensuit modules will only need two streams (send/recv).
 * 3. Most Project Barzensuit buffers will only need to  hold a few KBs at most.
 * 3. Portability to non-POSIX OSes with GnuLib fmemopen module
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

#ifndef _BZEN_SBUF_H_
#define _BZEN_SBUF_H_

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include "bzenthread.h"

/**
 * @typedef bzen_cbuflock
 */
typedef struct _bzen_cbuflock_s
{
  unsigned short int id;
  pthread_mutex_t mutex;
  size_t size;
} bzen_cbuflock_t;

/**
 * Return a count of the number of buffers currently allocated.
 *
 * @return size_t
 */
size_t bzen_sbuf_count_allocated();

/**
 * Return a count of the number of buffers currently in use.
 *
 * @return size_t
 */
size_t bzen_sbuf_count_used(); 

/**
 * Allocate memory for a new character buffer.
 *
 * @param size_t size Size in bytes of buffer.
 *
 * @return bzen_cbuflock_t* Pointer to new buffer lock.
 */
bzen_cbuflock_t* bzen_sbuf_create(size_t size);

/**
 * Thread-safe stream buffer destructor.
 * 
 * bzen_sbuf_destroy() will try to get a lock on corresponding mutex before
 * destroying the mutex and freeing any memory.
 *
 * bzen_cbuflock_t* cbuflock Pointer to lock for targeted buffer.
 * double timeout Timeout period in seconds (wait for lock on mutex).
 *
 * @return int 0 on success
 */
int bzen_sbuf_destroy(bzen_cbuflock_t* cbuflock, double timeout);

/**
 * Performs safety check on buffer and attempts to lock it.
 *
 * @param bzen_cbuflock_t* cbuflock Pointer to lock for buffer.
 *
 * @return int Id of the buffer if safe and locked, otherwise -1.
 */
int bzen_sbuf_lock(bzen_cbuflock_t* cbuflock);

/**
 * Attempts to unlock the given buffer.
 *
 * @param bzen_cbuflock_t* cbuflock Pointer to lock for buffer.
 *
 * @return int 0 if buffer is unlocked, otherwise -1.
 */
int bzen_sbuf_unlock(bzen_cbuflock_t* cbuflock);

/**
 * Get a character from the given buffer.
 *
 * This function reads the next character as an unsigned char from the stream
 * stream and returns its value, converted to an int. If an end-of-file 
 * condition or read error occurs, EOF is returned instead. 
 *
 * @param bzen_cbuflock_t* cbuflock Pointer to lock for buffer.
 *
 * @return int Next character in buffer.
 */
int bzen_sbuf_getc(bzen_cbuflock_t* cbuflock);

/**
 * Put a character to the given buffer.
 *
 * The fputc function converts the character c to type unsigned char, and writes
 * it to the stream stream. EOF is returned if a write error occurs; otherwise
 *  the character c is returned. 
 *
 * @param int c The character to put.
 * @param bzen_cbuflock_t* cbuflock Pointer to lock for buffer.
 *
 * @return int The put character or EOF.
 */
int bzen_sbuf_putc(int c, bzen_cbuflock_t* cbuflock);

/**
 * Set file position to beginning of stream and reset error indicator.
 *
 * @param bzen_cbuflock_t* cbuflock Pointer to lock for buffer.
 *
 * @return int 0 on success, otherwise nonzero.
 */
int  bzen_sbuf_rewind(bzen_cbuflock_t* cbuflock);

#endif /* _BZEN_SBUF_H_ */
