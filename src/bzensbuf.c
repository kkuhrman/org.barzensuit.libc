/**
 * @file:	bzensbuf.c
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

#include <config.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include "bzenmem.h"
#include "bzenthread.h"
#include "bzensbuf.h"

/* Allocate memory for a new stream buffer. */
bzen_sbuf_t* bzen_sbuf_create(unsigned long int size)
{
  bzen_sbuf_t* pbuffer;
  char* buffer;
  size_t buffer_struct_size;
  int status;
  int nullterminate_pos;

  /* Determine size of buffer struct.  */
  buffer_struct_size = xcast_size_t(sizeof(bzen_sbuf_t) + (sizeof(char*) * size));
  
  /* Allocate memory for buffer struct. */
  pbuffer = (bzen_sbuf_t*)bzen_malloc(buffer_struct_size);

  /* Initialize the mutex. */
  status = bzen_mutex_init(&pbuffer->mutex, NULL);
  if (status != 0)
    {
      bzen_free(pbuffer);
      pbuffer = NULL;
      goto CREATE_FAIL;
    }

 CREATE_FAIL:

  return pbuffer;
}

/* Thread-safe stream buffer destructor. */
int bzen_sbuf_destroy(bzen_sbuf_t* buffer, double timeout)
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
      result = bzen_mutex_destroy(&buffer->mutex);
      if (result == 0)
	{
	  /* @todo: Deallocate buffer memory. 
	   obviously somehow out of bounds as this causes double free or 
	   corruption (top) condition */
	  /* bzen_free(buffer); */
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
