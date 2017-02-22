/**
 * @file:	bzensbuf.h
 * @brief:	Thread-safe stream buffer management functions.
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
const unsigned short int BZEN_DEFAULT_NUMBER_OF_BUFFERS = 4;
#define BZEN_DEFAULT_NUMBER_OF_BUFFERS BZEN_DEFAULT_NUMBER_OF_BUFFERS

/**
 * default size for io stream buffer.
 */
const unsigned short int BZEN_SBUF_DEFAULT_SIZE = 1024;
#define BZEN_SBUF_DEFAULT_SIZE BZEN_SBUF_DEFAULT_SIZE

/**
 * default wait time in seconds for lock on mutex.
 */
const unsigned short int BZEN_SBUF_DEFAULT_WAIT = 1;
#define BZEN_SBUF_DEFAULT_WAIT BZEN_SBUF_DEFAULT_WAIT

/**
 * @typedef:    bzen_sbuf
 */
typedef struct _bzen_sbuf_s
{
  pthread_mutex_t mutex;
  char buffer[1024];
} bzen_sbuf_t;

/**
 * Allocate memory for a new stream buffer.
 *
 * @param unsigned long int size Size in bytes of buffer.
 *
 * @return bzen_sbuf* pointer to newly allocated buffer or NULL.
 */
bzen_sbuf_t* bzen_sbuf_create(unsigned long int size);

/**
 * Thread-safe stream buffer destructor.
 * 
 * bzen_sbuf_destroy() will try to get a lock on corresponding mutex before
 * destroying the mutex and freeing any memory.
 *
 * bzen_sbuf* buffer Pointer to buffer to destroy.
 * double timeout Timeout period in seconds (wait for lock on mutex).
 *
 * @return int 0 on success
 */
int bzen_sbuf_destroy(bzen_sbuf_t* buffer, double timeout);

#endif /* _BZEN_SBUF_H_ */
