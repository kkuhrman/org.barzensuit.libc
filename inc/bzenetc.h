/**
 * @file:	bzenetc.h
 * @brief:      Comprises functions for editable text configuration (etc) files.
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

#ifndef _BZEN_ETC_H_
#define _BZEN_ETC_H_

#include <config.h>

/**
 * @enum Configuration file types.
 */
enum BZENETC_TYPE
  {
    BZENETC_LOG
  };

/**
 * @typedef bzen_etc_log
 */
typedef struct _bzen_etc_log_s
{
  unsigned short int severity_code;
} bzen_etc_log_t;

#endif /* _BZEN_ETC_H_ */
