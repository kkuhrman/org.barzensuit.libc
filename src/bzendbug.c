/**
 * @file:	bzendbug.c
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

#include <config.h>
#include "bzenlog.h"
#include "bzenmem.h"
#include "bzendbug.h"

/* Dumps content of buffer to a log file. */
size_t bzen_dbug_log_buffer_dump(bzen_cbuflock_t* cbuflock, 
				 const char* name,
				 size_t size)
{
  size_t nchars = -1;
  int charid;
  char* dumpstr;
  int status;

  /* Allocate string for saving buffer content. */
  dumpstr = (char*)bzen_malloc(sizeof(char) * cbuflock->size);

  /* attempt to rewind buffer. */
  status = bzen_sbuf_rewind(cbuflock);
  if (status == 0)
    {
      /* nchars = 0; */
      do
	{
	  dumpstr[++nchars] = bzen_sbuf_getc(cbuflock); 
	} while ((dumpstr[nchars] != EOF) && (nchars < size));
    }

  /* Write to debugging log. */
  status = bzen_log_write_stat(name, BZENLOG_DEBUG, dumpstr);
  if (status != 0)
    {
      nchars = -1;
    }

  return nchars;
}
