/**
 * @file:	bzensbuf.h
 * @brief:	Encapsulate thread-safe allocation/access of character buffers.
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
#include <sys/types.h>
#include "bzenthread.h"

/**
 * @todo: allow conf file declare number of buffers.
 */
const size_t BZEN_DEFAULT_NUMBER_OF_BUFFERS = 4;
#define BZEN_DEFAULT_NUMBER_OF_BUFFERS BZEN_DEFAULT_NUMBER_OF_BUFFERS

/**
 * default size for io stream buffer.
 */
const size_t BZEN_SBUF_DEFAULT_SIZE = 1024;
#define BZEN_SBUF_DEFAULT_SIZE BZEN_SBUF_DEFAULT_SIZE

/**
 * default wait time in seconds for lock on mutex.
 */
const double BZEN_SBUF_DEFAULT_WAIT = 1;
#define BZEN_SBUF_DEFAULT_WAIT BZEN_SBUF_DEFAULT_WAIT

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

#endif /* _BZEN_SBUF_H_ */
