/**
 * @file:	bzentime.h
 * @brief:	Encapsulates some time functions.
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

#ifndef _BZEN_TIME_H_
#define _BZEN_TIME_H_

#include <config.h>
#include <time.h>

/**
 * Safe buffer size for printing time strings.
 */
const unsigned short int BZEN_TIME_STR_BUFFER_SIZE = 256;
#define BZEN_TIME_STR_BUFFER_SIZE BZEN_TIME_STR_BUFFER_SIZE

#endif /* _BZEN_TIME_H_ */
