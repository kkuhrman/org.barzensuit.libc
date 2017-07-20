/**
 * @file:	bzenstrm.h
 * @brief:      Stream manipulation functions.
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
#include <fcntl.h>
#include "bzenstrm.h"


/* Close stream. */
int bzen_stream_close(bzen_stream_t* stream)
{
  int result = 0;

  /* Expect non-null pointer. */
  BZEN_ASSERT(stream);

  /* Close the encapsulated stream. */
  result = fclose(stream->file);
  if (result != 0)
    {
      perror("fclose");
      goto CLOSE_FAIL;
    }

  /* Initialize struct. */
  memset(stream, 0, BZEN_SIZEOF(bzen_stream_t));

 CLOSE_FAIL:

  return result;
}

/* Free memory for given bzen_stream_t struct. */
int bzen_stream_delete(bzen_stream_t* stream)
{
  int result;
  
  /* Expect non-null pointer. */
  BZEN_ASSERT(stream);

  if (stream->buffer != NULL)
    {
      bzen_free(*stream->buffer);
      bzen_free(stream->buffer);
    }
  bzen_free(stream);
  result = 0;

  return result;
}

/* Open stream as a file. */
int bzen_stream_fopen(bzen_stream_t* stream,
		      const char* name,
		      const char* type)
{
  int result;

  /* Verify that stream is not already open. */
  result = bzen_stream_get_file_status(stream);
  if (result < 0)
    {
      /* Open stream. */
      stream->file = fopen(name, type);
      if (stream->file == NULL)
	{
	  goto OPEN_FAIL;
	}

      /* Success. */
      result = 0;
    }

 OPEN_FAIL:

  return result;
}

/* Open stream as a fixed size buffer in memory. */
int bzen_stream_fmemopen(bzen_stream_t* stream,
			 size_t size,
			 const char* type)
{
  size_t buffer_size;
  int result;

  /* Verify that stream is not already open. */
  result = bzen_stream_get_file_status(stream);
  if (result < 0)
    {
      /* Allocate buffer. */
      buffer_size = BZEN_SIZE(size * sizeof(char));
      stream->buffer = (char*)bzen_malloc(buffer_size);
      if (stream->buffer == NULL)
	{
	  goto OPEN_FAIL;
	}

      /* Open stream. */
      stream->file = fmemopen(*stream->buffer, buffer_size, type);
      if (stream->file == NULL)
	{
	  goto OPEN_FAIL;
	}
     
      /* Save open attributes. */
      stream->size = buffer_size;
      memcpy(stream->opentype, type, BZEN_OPENTYPE_SIZE);

      /* Success. */
      result = 0;
    }

 OPEN_FAIL:

  return result;
}

/* Get a character from the given stream. */
int bzen_stream_getc(bzen_stream_t* stream)
{
  int result = EOF;

  /* Expect non-null pointer. */
  BZEN_ASSERT(stream);

  if (stream->file)
    {
      /* Put character to buffer. */
      result = fgetc(stream->file);
    }

  return result;
}

/* Open stream as a dynamic buffer in memory. */
int bzen_stream_open_memstream(bzen_stream_t* stream)
{
  size_t buffer_size;
  int result;

  /* Verify that stream is not already open. */
  result = bzen_stream_get_file_status(stream);
  if (result < 0)
    {
      /* Allocate minimum buffer. */
      buffer_size = BZEN_SIZE(BZEN_STREAM_MIN_BUFFER_SIZE * sizeof(char));
      stream->buffer = (char*)bzen_malloc(buffer_size);
      if (stream->buffer == NULL)
	{
	  goto OPEN_FAIL;
	}

      /* Open stream. */
      stream->file = open_memstream(stream->buffer, &stream->size);
      if (stream->file == NULL)
	{
	  goto OPEN_FAIL;
	}

      /* Save open attributes. */
      stream->size = buffer_size;
     
      /* Success. */
      result = 0;
    }

 OPEN_FAIL:

  return result;
}

/* Allocate memory for a bzen_stream_t struct. */
int bzen_stream_new(bzen_stream_t** stream)
{
  int result = 0;

  *stream = (bzen_stream_t*)bzen_malloc(BZEN_SIZEOF(bzen_stream_t));
  if (*stream == NULL)
    {
      result = -1;
      goto MALLOC_FAIL;
    }

  /* Initialize struct. */
  memset(*stream, 0, BZEN_SIZEOF(bzen_stream_t));

 MALLOC_FAIL:

  return result;
}

/* Helper function returns open file flags. */
int bzen_stream_get_file_status(bzen_stream_t* stream)
{
  int fd;
  int result = -1;

  /* Expect non-null pointer. */
  BZEN_ASSERT(stream);

  if (stream->file)
    {
      fd = fileno(stream->file);
      result = fcntl(fd, F_GETFL);
      if (result < 0)
	{
	  perror("fcntl");
	}
    }

  return result;
}

/* Put a character to the given buffer. */
int bzen_stream_putc(int c, bzen_stream_t* stream)
{
  int result = EOF;

  /* Expect non-null pointer. */
  BZEN_ASSERT(stream);

  if (stream->file)
    {
      /* Put character to buffer. */
      result = fputc(c, stream->file);
    }

  return result;
}

/* Set file position to beginning of stream and reset error indicator. */
int  bzen_stream_rewind(bzen_stream_t* stream)
{
  int result = -1;

  /* Expect non-null pointer. */
  BZEN_ASSERT(stream);

  if (stream->file)
    {
      /* rewind() does not return a value. */
      rewind(stream->file);
      result = 0;
    }

  return result;
}
