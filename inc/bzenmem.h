/**
 * @file:	bzenmem.h
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

#ifndef _BZENLIBC_MEM_H_
#define _BZENLIBC_MEM_H_

#include <config.h>
#include "bzenpriv.h"

/**
 * Free memory allocated dynamically.
 *
 * @param void* ptr Pointer to block of memory to be freed.
 *
 * @return void
 */
void bzen_free(void* ptr);

/**
 * Allocate N bytes of memory dynamically, with error checking.
 *
 * @param size_t n Size of memory block to allocate in bytes.
 *
 * @return void* Pointer to allocated block of memory.
 */
void* bzen_malloc(size_t n);

/**
 * Prints statistics on memory allocated by malloc to stream.
 *
 * @param FILE* stream Stream to print to.
 * 
 * @return void.
 */
void bzen_malloc_print_stats(FILE* stream);

/**
 * Reallocate a block at p of pn objects of s bytes each.
 *
 * @see x2nrealloc() in gnulib/xalloc.h
 *
 * Be mindful that pn returns with the 'new' size, which is likely to be greater
 * than the size requested as gnulib increments at a rate of about 1.5 * pn per 
 * call. Check the outcoming value if knowing the allocated block size is 
 * important (do not rely on the input value being unchanged).
 * see gnulib/xalloc.h for further description of function behavior.
 *
 * @param void* p Pointer to memory block to reallocate.
 * @param size_t* pn New number of allocated objects.
 * @param size_t s Size of object.
 *
 * @return void* Pointer to the new block.
 */
void* bzen_realloc (void* p, size_t* pn, size_t s);

#endif /* _BZENLIBC_MEM_H_ */
