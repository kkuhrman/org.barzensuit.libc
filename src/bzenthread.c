/**
 * @file:	bzenthread.c
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

#include <config.h>
#include <errno.h>
#include <stdio.h>
#include "bzenthread.h"

/* Supplement pthread_mutex_destroy() with error logging. */
int bzen_mutex_destroy(pthread_mutex_t* mutex)
{
  int status;

  status = pthread_mutex_destroy(mutex);
  if (status != 0)
    {
      bzen_thread_print_error("pthread_mutex_destroy", status);
    }

  return status;
}

/* Supplements pthread_mutex_init() with error logging. */
int bzen_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr)
{
  int status;

  /* Initialize the mutex. */
  status = pthread_mutex_init(mutex, attr);
  if (status != 0)
    {
      bzen_thread_print_error("pthread_mutex_init", status);
    }

  return status;
}

/* Send pthreads error message to stderr. */
void bzen_thread_print_error(const char* fn_name, int code)
{
  /* @todo: mimic perror() for the pthreads errnos */
  fprintf(stderr, "\n\terror in pthreads function %s %d\n", fn_name, code);
}
