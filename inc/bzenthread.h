/**
 * @file:	bzenthread.h
 * @brief:	Encapsulate and supplement pthreads interfaces.
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

#ifndef _BZEN_THREAD_H_
#define _BZEN_THREAD_H_

#include <config.h>
#include <pthread.h>

/**
 * Supplement pthread_mutex_destroy() with error logging.
 * 
 * @see: pthread_mutex_destroy()
 *
 * @param pthread_mutex_t* mutex
 * 
 * @return int 0 on success or errno.
 */
int bzen_mutex_destroy(pthread_mutex_t* mutex);

/**
 * Supplement pthread_mutex_init() with error logging.
 * 
 * @see: pthread_mutex_init()
 *
 * @param pthread_mutex_t* mutex
 * @param const pthread_mutexattr_t* attr
 * 
 * @return int 0 on success or errno.
 */
int bzen_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);

/**
 * Send pthreads error message to stderr.
 *
 * @param const char* fn_name Use same as as perror()
 * @param int code The error number returned by a pthreads fn().
 *
 * @return void.
 */
void bzen_thread_print_error(const char* fn_name, int code);

#endif /* _BZEN_THREAD_H_ */
