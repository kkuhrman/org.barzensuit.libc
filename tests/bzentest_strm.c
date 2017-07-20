/**
 * @file:	bzentest_strm.c
 * @brief:	Unit test stream manipulation code.
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

/* libzenc includes */
#include "bzentest.h"
#include "bzenstrm.h"

#define BZEN_TEST_ASCII_LO 32
#define BZEN_TEST_ASCII_HI 126
#define BZEN_TEST_OPENTYPE "w+"
#define BZEN_TEST_FILESIZE \
  BZEN_SIZE( BZEN_TEST_ASCII_HI + 1 - BZEN_TEST_ASCII_LO )
#define BZEN_TEST_FILENAME "bzentest_sbuf.txt"

/* Helper funtion tests putc, rewind, getc */
int bzentest_stream_rw(bzen_stream_t* stream);

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  char tempfile[1024];
  bzen_stream_t* stream;

  /* Create a new stream struct. */
  status = bzen_stream_new(&stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test file status (expect fail). */
  status = bzen_stream_get_file_status(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, -1))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test a static buffer in memory. */
  status = bzen_stream_fmemopen(stream, BZEN_TEST_FILESIZE, BZEN_TEST_OPENTYPE);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test read/write functionality. */
  status = bzentest_stream_rw(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Close the file. */
  status = bzen_stream_close(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test a dynamic buffer in memory. */
  status = bzen_stream_open_memstream(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test read/write functionality. */
  status = bzentest_stream_rw(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Close the file. */
  status = bzen_stream_close(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test a file from storage. */
  sprintf(tempfile, "%s/%s", getenv("BZENTEST_TEMP_DIR"), BZEN_TEST_FILENAME);
  status = bzen_stream_fopen(stream, tempfile, BZEN_TEST_OPENTYPE);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test read/write functionality. */
  status = bzentest_stream_rw(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Close the file. */
  status = bzen_stream_close(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Delete the stream struct. */
  status = bzen_stream_delete(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  return result;
}

/* Helper funtion tests putc, rewind, getc */
int bzentest_stream_rw(bzen_stream_t* stream)
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  int current, expected, actual;

    /* Test putting characters to the stream. */
  for (current = BZEN_TEST_ASCII_LO; current < BZEN_TEST_FILESIZE; current++)
    {
      expected = current;
      actual = bzen_stream_putc(current, stream);
      if (BZENPASS != BZENTEST_EQUALS_N(expected, actual))
	{
	  fprintf(stderr, 
		  "\n\texpected %s actual %s\n", 
		  (char)expected, 
		  (char)actual);
	  goto END_SUBTEST;
	}
    }

  /* Test rewinding the stream. */
  status = bzen_stream_rewind(stream);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      fprintf(stderr, "\n\tFAIL: bzen_steram_rewind()\n");
      goto END_SUBTEST;
    }

  /* Test getting characters from the stream. */
  for (current = BZEN_TEST_ASCII_LO; current < BZEN_TEST_FILESIZE; current++)
    {
      expected = current;
      actual = bzen_stream_getc(stream);
      if (BZENPASS != BZENTEST_EQUALS_N(expected, actual))
	{
	  fprintf(stderr, 
		  "\n\texpected %s actual %s\n", 
		  (char)expected, 
		  (char)actual);
	  goto END_SUBTEST;
	}
    }

 END_SUBTEST:

  return result;
}
