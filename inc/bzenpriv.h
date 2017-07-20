/**
 * @file:	bzenpriv.h
 * @brief:      Library private global defs.
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

#ifndef _BZEN_PRIV_H_
#define _BZEN_PRIV_H_

#include <config.h>
#include <assert.h>
#include <string.h>
#include "xsize.h"
#include "bzenapi.h"

/**
 * @todo: what's in and what's out based on NDEBUG flag?
 */
#define BZEN_ASSERT(expression) assert(expression)

/**
 * Cast arbitrary size as size_t with overflow protection.
 *
 * @see: xcast_size_t() SIZE_MAX defined in limits.h
 */
#define BZEN_SIZE(N) xcast_size_t(N)

/**
 * Cast sizeof given type to size_t with overflow protection.
 */
#define BZEN_SIZEOF(T) xcast_size_t( sizeof(T) )

#endif /* _BZEN_PRIV_H_ */
