/**
 * @file:	bzendbug.h
 * @brief:	Library debugging utilities.
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

#ifndef __BZEN_DBUG_H_
#define __BZEN_DBUG_H_

#include <config.h>
#include "bzensbuf.h"

/*
 * Dumps content of buffer to a log file.
 *
 * @param bzen_cbuflock_t* cbuflock Buffer containing data to dump to log.
 * @param const char* name The name of the log to write to.
 * @param size_t size How much to dump? For whole buffer use cbuflock->size.
 *
 * @return size_t Number of characters written to log or -1 on error.
 */
size_t bzen_dbug_log_buffer_dump(bzen_cbuflock_t* cbuflock, 
				 const char* name,
				 size_t size);

#endif /* __BZEN_DBUG_H_ */
