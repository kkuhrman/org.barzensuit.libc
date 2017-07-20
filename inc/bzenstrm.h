/**
 * @file:	bzenstrm.h
 * @brief:      Stream manipulation.
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

#ifndef _BZEN_STRM_H_
#define _BZEN_STRM_H_

#include <config.h>
#include "bzenpriv.h"

/** Size of opentype attribute in bytes. */
#define BZEN_STREAM_OPENTYPE_SIZE 3

/**
 * Helper function returns open file status flags.
 *
 * @param[in] bzen_stream_t* stream The stream to check.
 *
 * @return file status flags as a bit mask or @c -1 on error.
 */
int bzen_stream_get_file_status(bzen_stream_t* stream);

#endif /* _BZEN_STRM_H_ */
