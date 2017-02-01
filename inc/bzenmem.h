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

/**
 * Allocate N bytes of memory dynamically, with error checking.
 *
 * @param size_t n Size of memory block to allocate in bytes.
 *
 * @return void* Pointer to allocated block of memory.
 */
void* bzen_malloc(size_t n);

#endif /* _BZENLIBC_MEM_H_ */
