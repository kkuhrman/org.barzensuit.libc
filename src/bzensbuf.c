/**
 * @file:	bzensbuf.c
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

#include <config.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include "bzenmem.h"
#include "bzenthread.h"
#include "bzensbuf.h"

/**
 * encapsulated buffers.
 */
static char** buffers = NULL;
static size_t buffers_used = 0;
static size_t buffers_allocated = 0;

/* Return a count of the number of buffers currently allocated. */
size_t bzen_sbuf_count_allocated()
{
  return buffers_allocated;
}

/* Return a count of the number of buffers currently in use. */
size_t bzen_sbuf_count_used()
{
  return buffers_used;
}

/* Allocate memory for a new stream buffer. */
bzen_cbuflock_t* bzen_sbuf_create(size_t size)
{
  size_t buffer_struct_size;
  int status;

  /* Allocate default number of buffers if not already done. */
  bzen_cbuflock_t* pcbuflock;
  unsigned short int buffer_id;
  if (buffers == NULL)
    {
      buffers_allocated = BZEN_DEFAULT_NUMBER_OF_BUFFERS;
      buffers = (char**)bzen_malloc(xcast_size_t(sizeof(char*)) * buffers_allocated);
    }
  else
    {
      if (buffers_used == buffers_allocated)
	{
	  /* Grow by default size. */
	  fprintf(stderr, "\n\tallocated in: %d", buffers_allocated);
	  buffers = (char**)bzen_realloc(buffers,
					 &buffers_allocated,
					 xcast_size_t(sizeof(char*)));
	}
    }
  buffer_id = buffers_used++;
  buffers[buffer_id] = (char*)bzen_malloc(sizeof(char) * size);
  buffers[buffer_id][0] = '\0';

  /* Determine size of buffer struct.  */
  buffer_struct_size = xcast_size_t(sizeof(bzen_cbuflock_t));
  
  /* Allocate memory for buffer struct. */
  pcbuflock = (bzen_cbuflock_t*)bzen_malloc(buffer_struct_size);

  /* Initialize the mutex. */
  status = bzen_mutex_init(&pcbuflock->mutex, NULL);
  if (status != 0)
    {
      bzen_free(pcbuflock);
      pcbuflock = NULL;
      goto CREATE_FAIL;
    }

  /* Initialize other members. */
  pcbuflock->id = buffer_id;
  pcbuflock->size = size;

 CREATE_FAIL:

  return pcbuflock;
}

/* Thread-safe stream buffer destructor. */
int bzen_sbuf_destroy(bzen_cbuflock_t* cbuflock, double timeout)
{
  int result;
  clock_t start_time;
  clock_t end_time;
  double wait_time;

  /* Initialize clock timer.*/
  start_time = clock();

  do
    {
      /* Attempt to destroy mutex while unlocked. It is safe to destroy an 
	 initialised mutex that is unlocked. Attempting to destroy a locked 
	 mutex results in undefined behaviour. */
      result = bzen_mutex_destroy(&cbuflock->mutex);
      if (result == 0)
	{
	  /* @todo: Deallocate buffer memory. 
	   obviously somehow out of bounds as this causes double free or 
	   corruption (top) condition */
	  bzen_free(cbuflock);
	  break;
	}
      else
	{
	  if (result == EBUSY)
	    {
	      /* Wait a few ticks. */
	      sleep(BZEN_SBUF_DEFAULT_WAIT);

	      /* Check wait time. */
	      wait_time = ((double)(clock() - start_time))/CLOCKS_PER_SEC;
	    }
	  else
	    {
	      /* Some other error condition prevented us from destroying mutex.
	         Error already logged by bzen_thread_destroy, so just bail. */
	      goto DESTROY_FAIL;
	    }
	}
    } while (wait_time < timeout);

 DESTROY_FAIL:

  return result;
}
