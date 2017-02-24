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
 * Encapsulated buffers.
 */
static FILE** buffers = NULL;

/**
 * Encapsulated buffer counters.
 */
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
      /* Allocate memory for FILE* pointers to streams. */
      buffers = (FILE**)bzen_malloc(xcast_size_t(sizeof(FILE*)) * buffers_allocated);
    }
  else
    {
      if (buffers_used == buffers_allocated)
	{
	  /* Grow by default size. */
	  buffers = (FILE**)bzen_realloc(buffers,
					 &buffers_allocated,
					 xcast_size_t(sizeof(FILE*)));
	}
    }

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

  /* Create the buffer as an IO stream (FILE) in memory.
     Passing a NULL pointer to fmemopen is intentional. It results in
     the function dynamically allocating memory for the buffer, 
     preventing direct access to it except through stdio functions,
     thus providing for better built-in thread-safety via FILE locks
     and also cleans up heap when fclose() is called.  */
  buffer_id = buffers_used++;
  size_t new_buffer_size = xcast_size_t(sizeof(int) * size);
  buffers[buffer_id] = fmemopen(NULL, new_buffer_size, "r+");
  if (buffers[buffer_id] == NULL)
    {
      bzen_free(pcbuflock);
      pcbuflock = NULL;
      goto CREATE_FAIL;
    }

  /* Initialize other members. */
  pcbuflock->id = buffer_id;
  pcbuflock->size = new_buffer_size;

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
	  /* fclose() will free buffer memory on heap. */
	  result = fclose(buffers[cbuflock->id]);
	  if (result != 0)
	    {
	      /* @todo: this leaves us with a file close error and 
	         a destroyed mutex, which is bad. */
	      goto DESTROY_FAIL;
	    }
	  buffers[cbuflock->id] = NULL;
	  bzen_free(cbuflock);
	  buffers_used--;
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

/* Performs safety check on buffer and attempts to lock it. */
int bzen_sbuf_lock(bzen_cbuflock_t* cbuflock)
{
  int buffer_id;
  int lock_status;

  /* Safety check for null pointer. */
  if (cbuflock == NULL)
    {
      buffer_id = -1;
      goto LOCK_FAIL;
    }

  /* Lock mutex. This will block destruction of buffer via bzen_sbuf_destroy()
     until mutex is unlocked. */
  lock_status = pthread_mutex_lock(&cbuflock->mutex);
  if (lock_status != 0)
    {
      /* @todo: error loging */
      buffer_id = -1;
      goto LOCK_FAIL;
    }

  /* Safety check for null buffer. */
  if ((cbuflock->id >= buffers_used) ||
      (buffers == NULL) ||
      (buffers[cbuflock->id] == NULL))
    {
      buffer_id = -1;
      goto LOCK_FAIL;
    }

  /* Buffer is locked and appears to be ready for read/write op. */
  buffer_id = cbuflock->id;

 LOCK_FAIL:

  return buffer_id;
}

/* Attempts to unlock the given buffer. */
int bzen_sbuf_unlock(bzen_cbuflock_t* cbuflock)
{
  int unlock_status;

  /* Safety check for null pointer. */
  if (cbuflock == NULL)
    {
      goto LOCK_FAIL;
    }

  /* Unlock mutex. */
  unlock_status = pthread_mutex_lock(&cbuflock->mutex);
  if (unlock_status != 0)
    {
      /* @todo: error loging */
      goto LOCK_FAIL;
    }

  /* Buffer is unlocked. */

 LOCK_FAIL:

  return unlock_status;
}

/* Get a character from the given buffer. */
int bzen_sbuf_getc(bzen_cbuflock_t* cbuflock)
{
  int result;
  int buffer_id;

  /* Check buffer safety and attempt to lock it. */
  buffer_id = bzen_sbuf_lock(cbuflock);
  if (buffer_id == -1)
    {
      result = EOF;
      goto GETC_FAIL;
    }

  /* Put character to buffer. */
  result = fgetc(buffers[cbuflock->id]);

  /* Unlock the buffer. */
  bzen_sbuf_unlock(cbuflock);

 GETC_FAIL:

  return result;
}

/* Put a character to the given buffer. */
int bzen_sbuf_putc(int c, bzen_cbuflock_t* cbuflock)
{
  int result;
  int buffer_id;

  /* Check buffer safety and attempt to lock it. */
  buffer_id = bzen_sbuf_lock(cbuflock);
  if (buffer_id == -1)
    {
      result = EOF;
      goto PUTC_FAIL;
    }

  /* Put character to buffer. */
  result = fputc(c, buffers[cbuflock->id]);

  /* Unlock the buffer. */
  bzen_sbuf_unlock(cbuflock);

 PUTC_FAIL:

  return result;
}

/* Set file position to beginning of stream and reset error indicator. */
int  bzen_sbuf_rewind(bzen_cbuflock_t* cbuflock)
{
  int result;
  int buffer_id;

  /* Check buffer safety and attempt to lock it. */
  buffer_id = bzen_sbuf_lock(cbuflock);
  if (buffer_id == -1)
    {
      result = EOF;
      goto REWIND_FAIL;
    }

  /* rewind() does not return a value. */
  result = 0;
  rewind(buffers[cbuflock->id]);

  /* Unlock the buffer. */
  bzen_sbuf_unlock(cbuflock);

 REWIND_FAIL:

  return result;
}
