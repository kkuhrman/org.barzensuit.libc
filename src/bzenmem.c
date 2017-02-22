/**
 * @file:	bzenmem.c
 * @brief:	Helper functions for managing dynamically allocated memory.
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
#include <malloc.h>
#include "xalloc.h"
#include "bzenmem.h"

/* Free memory allocated dynamically. */
void bzen_free(void* ptr)
{
  free(ptr);
}

/* Allocate N bytes of memory dynamically, with error checking. */
void* bzen_malloc(size_t n) 
{
  void* ptr;

  /* @todo: manage cleanup of heap. */

  /* Allocate n bytes @ ptr. */
  ptr =  xmalloc(n);
  
  /* zero-out newly allocated memory. */
  memset(ptr, 0, n);
  
  return ptr;
}

/* Prints statistics on memory allocated by malloc to stream. */
void bzen_malloc_print_stats(FILE* stream)
{
  if (stream != NULL)
    {
      struct mallinfo stats;
      stats = mallinfo();

      /* @see: https://www.gnu.org/software/libc/manual/html_node/
               Statistics-of-Malloc.html#Statistics-of-Malloc */
      fprintf(stream, "\nDYNAMIC MEMORY STATISTICS\n");

      /* This is the total size of memory allocated with sbrk by malloc, in bytes.  */
      fprintf(stream, "\n\tarena size: %d", stats.arena);

      /* This is the number of chunks not in use.  */
      fprintf(stream, "\n\tChunks not in use: %d", stats.ordblks);

      /* This is the total number of chunks allocated with mmap. */
      fprintf(stream, "\n\tChunks allocated with mmap: %d", stats.hblks);

      /* This is the total size of memory allocated with mmap, in bytes.  */
      fprintf(stream, "\n\tTotal memory allocated with mmap: %d", stats.hblkhd);

      /* This is the total size of memory occupied by chunks handed out by malloc.  */
      fprintf(stream, "\n\tMemory allocated with malloc(): %d", stats.uordblks);

      /* This is the total size of memory occupied by free (not in use) chunks.  */
      fprintf(stream, "\n\tMemory occupied by free(): %d", stats.fordblks);

      /* This is the size of the top-most releasable chunk that normally borders 
	 the end of the heap. */
      fprintf(stream, "\n\tSize of top-most releasable chunk: %d\n", stats.keepcost);
    }
}

/* Reallocate a block at p of pn objects of s bytes each. */
void* bzen_realloc (void* p, size_t* pn, size_t s) 
{
  /* @todo: manage cleanup of heap. */
  return x2nrealloc(p, pn, s);
}
