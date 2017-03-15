/**
 * @file:	bzenlog.c
 * @brief:	Manages output to log files.
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
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include "bzenmem.h"
#include "bzentime.h"
#include "bzenlog.h"

/**
 * Default number of logs.
 */
const int BZEN_DEFAULT_NUMBER_LOGS = 4;
#define BZEN_DEFAULT_NUMBER_LOGS BZEN_DEFAULT_NUMBER_LOGS

/**
 * Symbolic severity code constants.
 */
const char BZENLOG_SEVERITY_CODE_SYMBOL[BZENLOG_DEBUG + 1] =
  {
    'E', 'W', 'S', 'I', 'D'
  };

/**
 * Encapsulated logs and corresponding locks.
 */
static char** log_names = NULL;
static char** log_paths = NULL;
static char** log_evtline_buffers = NULL;
static char** log_message_buffers = NULL;
static bzen_loglock_t** log_locks = NULL;

/**
 * Encapsulated buffer counters.
 */
static size_t logs_used = 0;
static size_t logs_allocated = 0;

/* Report to syslog failure to access log resource. */
static void bzen_log_handle_access_fail(const char* package,
					const char* resource,
					const char* function,
					int code)
{
  char syslog_msg[BZEN_LOG_MESSAGE_MAX_CHARS];

  /* Open syslog facility. */
  openlog (package, LOG_PERROR, LOG_USER);

  /* Format message. */
  sprintf(syslog_msg, "%s failed to access log resource %s. Function %s returned %d",
	    package, resource, function, code);

  /* Send message to syslog. */
  syslog (LOG_WARNING, syslog_msg);

  /* Close log. */
  closelog ();
}

/* Close the given named log. */
int bzen_log_close(const char* name)
{
  int log_id;
  int result;

  log_id = bzen_log_find_id(name);
  if (log_id >= 0)
    {
      if (log_locks[log_id]->status == 'o')
	{
	  /* Close the file. */
	  result = fclose(log_locks[log_id]->fd);
	  if (result != 0)
	    {
	      /* @todo: problem closing file, log error. */
	    }
	  else
	    {
	      log_locks[log_id]->fd = NULL;
	      log_locks[log_id]->status = 'c';
	    }
	}
    }

  return result;
}

/* Close all open log files. */
int bzen_log_close_all()
{
  int log_id;
  int result;

  /* @todo: only process/thread, which opened the log should be permitted to 
     to close it, no? */
  for (log_id = 0; log_id < logs_used; log_id++)
    {
      if (log_locks[log_id]->status == 'o')
	{
	  /* Close the file. */
	  result = fclose(log_locks[log_id]->fd);
	  if (result != 0)
	    {
	      /* @todo: problem closing file, log error. */
	    }
	  else
	    {
	      log_locks[log_id]->fd = NULL;
	      log_locks[log_id]->status = 'c';
	    }
	}
    }

  return result;
}

/* Generate event line text. */
static int  bzen_log_event_line(bzenlog_severity_code_t code, 
				char* buffer,
				size_t size)
{
  time_t now;
  struct tm* local_time;
  char event_time[BZEN_LOG_EVENT_LINE_DTM_LEN];
  const char* micro_time = "0000"; /* reserved for future implementation */
  uid_t effective_user_id;
  gid_t effective_group_id;
  uid_t real_user_id;
  gid_t real_group_id;
  char symbolic_code;
  int result;

  /* Check buffer and size. */
  if ((buffer == NULL) || (size < BZEN_LOG_EVENT_LINE_MAX_CHARS))
    {
      result = -1;
      goto FMT_FAIL;
    }

  /* Severity code defaults to 'info' */
  symbolic_code = (code <= BZENLOG_DEBUG) ? BZENLOG_SEVERITY_CODE_SYMBOL[code] : 'I';

  /* Get current local date-time. */
  now = time(NULL);
  local_time = localtime(&now);
  strftime(event_time, 
	   BZEN_TIME_STR_BUFFER_SIZE,
	   "%Y-%m-%d %H:%M:%S",
	   local_time);

  /* Get process persona data. */
  effective_user_id = geteuid();
  effective_group_id = getegid();
  real_user_id = getuid();
  real_group_id = getgid();

  /* Write formatted event line. */
  sprintf(buffer,
	  "%s %s %c %6d %6d %6d %6d\n", 
	  event_time, 
	  micro_time,
	  symbolic_code,
	  effective_user_id,
	  effective_group_id,
	  real_user_id,
	  real_group_id);

  /* SUCCESS */
  result = 0;

 FMT_FAIL:

  return result;
}

/* Find corresponding id of named log if exists. */
static int bzen_log_find_id(const char* name)
{
  int log_find;
  int log_id;

  log_id = -1;
  for (log_find = 0; log_find < logs_used; log_find++)
    {
      if (0 == strcmp(name, log_names[log_find]))
	{
	  log_id = log_find;
	  break;
	}
    }

  return log_id;
}

/* Formats a log message so no line exceeds max chars and no words are broken. */
static int  bzen_log_format_message(const char* message,
				    char* buffer,
				    size_t size)
{
  size_t unfmtmsg_size;
  int raw_pos;
  int fmt_pos;
  int line_char_count;
  int last_space;
  char read_char;
  char write_char;
  int result;
  
  /* Check buffer and size. */
  if ((buffer == NULL) || (size < BZEN_LOG_MESSAGE_MAX_CHARS))
    {
      result = -1;
      goto SKIP_FMT;
    }

  /* Fail on NULL */
  if (message == NULL)
    {
      sprintf(buffer, "%s", BZEN_LOG_ENTRY_DELIMITER);
      result = 0;
      goto SKIP_FMT;
    }

  /* Size of unformatted message. */
  unfmtmsg_size = strlen(message);

  /* If n chars is less than max per line, skip all the word-wrap logic. */
  if (unfmtmsg_size <= BZEN_LOG_LINE_MAX_CHARS)
    {
      sprintf(buffer, "%s", message);
      result = 0;
      goto SKIP_FMT;
    }

  /* Copy characters to fomatted message buffer. */
  raw_pos = 0;
  fmt_pos = 0;
  line_char_count = 0;
  last_space = 0;
  while (raw_pos < unfmtmsg_size)
    {
      /* Get next char in message. */
      read_char = message[raw_pos];

      if (line_char_count < BZEN_LOG_LINE_MAX_CHARS)
	{
	  /* Bookmark if char is punctuation or whitespace. */
	  if (ispunct(read_char) || isspace(read_char))
	    {
	      last_space = raw_pos;
	    }
	  
	  /* Send char to output. */
	  write_char = read_char;

	  /* Next char. */
	  raw_pos++;
	  line_char_count++;
	}
      else
	{
	  /* Limit reached. Check if in middle of word. */
	  if (last_space == raw_pos)
	    {
	      write_char = read_char;
	    }
	  else
	    {
	      /* Char pointer is in middle of word. Force new line after last bookmark. */
	      fmt_pos = last_space + 1;
	      write_char  = '\n';
	      
	      /* Reset char counter to beginning of last word. */
	      raw_pos = last_space + 1;
	      line_char_count = 0;
	    }
	}
      
      /* Copy character to formatted message buffer. */
      buffer[fmt_pos] = write_char;
      fmt_pos++;
    }
  
  result = 0;

 SKIP_FMT:
  
  return result;
}

/* Check if given log file is open. */
int bzen_log_is_open(const char* name)
{
  int result;
  int log_id;

  result = -1;
  log_id = bzen_log_find_id(name);
  if (log_id >= 0)
    {
      if (log_locks[log_id]->status == 'o')
	{
	  result = 0;
	}
    }

  return result;
}

/** Open a log file with given attributes. */
int bzen_log_open(const char* name, const char* attr)
{
  const char* logdir;
  FILE* log;
  size_t log_name_size;
  size_t log_path_size;
  size_t log_lock_size;
  size_t actual_cbuf_allocate;
  size_t actual_cbuf_allocate_min;
  size_t actual_lock_allocate;
  int log_id;
  int log_find;
  int status;
  int result;

  /* Attempt to open the directory for test file output. 
     @todo: This should really be in an /etc/conf file.
   */
  logdir = (char*)getenv("BZEN_LOG_DIR");
  result = access(logdir, R_OK | W_OK);
  if (result < 0)
  {
    bzen_log_handle_access_fail(PACKAGE_NAME, name, "access", result);
    goto OPEN_FAIL;
  }

  /* Allocate FILE and mutex for log if not exists. */
  if (log_locks == NULL)
    {
      /* Allocate memory for log names and locks. */
      log_id = -1;
      logs_allocated = BZEN_DEFAULT_NUMBER_LOGS;
      log_names = (char**)bzen_malloc(xcast_size_t(sizeof(char*)) * 
						logs_allocated);
      log_paths = (char**)bzen_malloc(xcast_size_t(sizeof(char*)) * 
						logs_allocated);
      log_evtline_buffers = (char**)bzen_malloc(xcast_size_t(sizeof(char*)) * 
						logs_allocated);
      log_message_buffers = (char**)bzen_malloc(xcast_size_t(sizeof(char*)) * 
						logs_allocated);
      log_locks = (bzen_loglock_t**)bzen_malloc(xcast_size_t(sizeof(bzen_loglock_t*)) * 
						logs_allocated);
    }
  else
    {
      /* Check if log is already registered and open. */
      status = bzen_log_is_open(name);
      if (status == 0)
	{
	  /* File is open - nothing left to do. */
	  goto LOG_OPEN;
	}
      
      /* Otherwise check if log is registered but closed. */
      log_id = bzen_log_find_id(name);
      if (log_id < 0)
	{
	  /* No match found. Ensure there is space for new entry. */
	  if (logs_used == logs_allocated)
	    {
	      /* Out of space. Grow by default size. */
	      logs_allocated += BZEN_DEFAULT_NUMBER_LOGS;
	      actual_cbuf_allocate = logs_allocated;
	      actual_lock_allocate = logs_allocated;

	      /* See comment below about log_locks allocated count. 
	         We want to use minimum allocated size as upper index. */
	      log_names = (char**)bzen_realloc(log_names,
					       &actual_cbuf_allocate,
					       xcast_size_t(sizeof(char*)));
	      actual_cbuf_allocate_min = actual_cbuf_allocate;

	      log_paths = (char**)bzen_realloc(log_paths,
					       &actual_cbuf_allocate,
					       xcast_size_t(sizeof(char*)));
	      actual_cbuf_allocate_min = (actual_cbuf_allocate_min < actual_cbuf_allocate) ?
		actual_cbuf_allocate_min : actual_cbuf_allocate;

	      log_evtline_buffers = (char**)bzen_realloc(log_evtline_buffers,
					       &actual_cbuf_allocate,
					       xcast_size_t(sizeof(char*)));
	      actual_cbuf_allocate_min = (actual_cbuf_allocate_min < actual_cbuf_allocate) ?
		actual_cbuf_allocate_min : actual_cbuf_allocate;

	      log_message_buffers = (char**)bzen_realloc(log_message_buffers,
					       &actual_cbuf_allocate,
					       xcast_size_t(sizeof(char*)));
	      actual_cbuf_allocate_min = (actual_cbuf_allocate_min < actual_cbuf_allocate) ?
		actual_cbuf_allocate_min : actual_cbuf_allocate;

	      log_locks = (bzen_loglock_t**)bzen_realloc(log_locks,
							 &actual_lock_allocate,
							 xcast_size_t(sizeof(bzen_loglock_t*)));

	      /* @todo: A  disparity in number of names vs. locks presents a 
	         potentially unsafe situation. We need to correct any disparity 
	         prior to proceeding. The gnulib xrealloc() increases block size
	         by a factor of 1.5. At present, we simply pick smaller or two. */
	      logs_allocated = (actual_lock_allocate < actual_cbuf_allocate_min) ?
		actual_lock_allocate : actual_cbuf_allocate_min;
	    }
	}
    }

  /* Is log registered under given name? */
  if (log_id < 0)
    {
      /* No. Create new entry.  Allocate memory for name.  */
      log_id = logs_used++;
      log_name_size = xcast_size_t(sizeof(char) * strlen(name));
      log_names[log_id] = (char*)bzen_malloc(log_name_size);
      memcpy(log_names[log_id], name, log_name_size);
      log_names[log_id][log_name_size] = '\0';

      /* Store full path to  log file. */
      log_path_size = xcast_size_t(sizeof(char) *
				   strlen(logdir) +
				   strlen(name) +
				   2); /* path delimiter & terminating null */
      log_paths[log_id] = (char*)bzen_malloc(log_path_size);
      sprintf(log_paths[log_id], "%s%s%s", 
	      logdir,
	      PATH_DELIMITER, 
	      name);

      /* Allocate memory for string buffers. */
      log_evtline_buffers[log_id] = (char*)bzen_malloc(xcast_size_t(sizeof(char) *
								    BZEN_LOG_EVENT_LINE_MAX_CHARS));
      log_message_buffers[log_id] = (char*)bzen_malloc(xcast_size_t(sizeof(char) *
								    BZEN_LOG_MESSAGE_MAX_CHARS));

      /* Allocate memory for lock. */
      log_lock_size = xcast_size_t(sizeof(bzen_loglock_t));
      log_locks[log_id] = (bzen_loglock_t*)bzen_malloc(log_lock_size);

      /* Initialize the mutex. */
      status = bzen_mutex_init(&log_locks[log_id]->mutex, NULL);
      if (status != 0)
	{
	  bzen_free(log_names[log_id]);
	  bzen_free(log_locks[log_id]);
	  goto OPEN_FAIL;
	}
    }

  /* Lock mutex. */
  status = pthread_mutex_lock(&log_locks[log_id]->mutex);
  if (status != 0)
    {
      result = -1;
      goto OPEN_FAIL;
    }

  /* Attempt to open file. */
  log_locks[log_id]->fd = fopen(log_paths[log_id], attr);
  if (log_locks[log_id]->fd == NULL)
    {
      bzen_mutex_destroy(&log_locks[log_id]->mutex);
      bzen_free(log_names[log_id]);
      bzen_free(log_paths[log_id]);
      bzen_free(log_locks[log_id]);
      result = -1;
      goto OPEN_FAIL;
    }

  /* Unlock mutex. */
  status = pthread_mutex_unlock(&log_locks[log_id]->mutex);
  if (status != 0)
    {
      bzen_mutex_destroy(&log_locks[log_id]->mutex);
      bzen_free(log_names[log_id]);
      bzen_free(log_locks[log_id]);
      result = -1;
      goto OPEN_FAIL;
    }

  /* SUCCESS  */
  log_locks[log_id]->status = 'o';
  result = 0;
  goto LOG_OPEN;

 OPEN_FAIL:
  /* @todo: clean up on open fail. */

 LOG_OPEN:

  return result;
}

/* Write a message to the given log. */
int bzen_log_write(const char* name, 
		   bzenlog_severity_code_t code, 
		   const char* message)
{
  /* char* fmtstr; */
  int log_id;
  int status;
  int result;

  /* @todo: check if we are logging given sevirity and bail if not. */
  
  /* Check if log file is open. */
  result = bzen_log_is_open(name);
  if (result < 0)
    {
      goto WRITE_FAIL;
    }

  /* Get the log file id. */
  log_id = bzen_log_find_id(name);
  if (log_id < 0)
    {
      result = -1;
      goto WRITE_FAIL;
    }

  /* Lock mutex. */
  status = pthread_mutex_lock(&log_locks[log_id]->mutex);
  if (status != 0)
    {
      result = -1;
      goto WRITE_FAIL;
    }

  /* Write the event line. */
  bzen_log_event_line(code, log_evtline_buffers[log_id], BZEN_LOG_EVENT_LINE_MAX_CHARS);
  status = fputs(log_evtline_buffers[log_id], log_locks[log_id]->fd);
  if (status == EOF)
    {
      result = -1;
      goto WRITE_FAIL_UNLOCK;
    }

  /* Write the message. */
  bzen_log_format_message(message, log_message_buffers[log_id], BZEN_LOG_MESSAGE_MAX_CHARS);
  status = fputs(log_message_buffers[log_id], log_locks[log_id]->fd);
  if (status == EOF)
    {
      result = -1;
      goto WRITE_FAIL_UNLOCK;
    }

  status = fputc('\n', log_locks[log_id]->fd);
  status = fputs(BZEN_LOG_ENTRY_DELIMITER, log_locks[log_id]->fd);

  /* Flush the stream buffer. */
  fflush(log_locks[log_id]->fd);

 WRITE_FAIL_UNLOCK:

  /* Unlock mutex. */
  status = pthread_mutex_unlock(&log_locks[log_id]->mutex);
  if (status != 0)
    {
      result = -1;
      goto WRITE_FAIL;
    }

 WRITE_FAIL:

  return result;
}

/* Write a message to the given log. Open log file if necessary. */
int bzen_log_write_stat(const char* name, 
			bzenlog_severity_code_t code, 
			const char* message)
{
  /* char* fmtstr; */
  int log_id;
  int status;
  int result;

  /* Check if log file is open. */
  result = bzen_log_is_open(name);
  if (result < 0)
    {
      /* Attempt to open log file. */
      status = bzen_log_open(name, BZEN_LOG_FOPEN_DEFAULT_ATTR);
      if (status < 0)
	{
	  goto WRITE_FAIL;
	}
    }

  /* Get the log file id. */
  log_id = bzen_log_find_id(name);
  if (log_id < 0)
    {
      result = -1;
      goto WRITE_FAIL;
    }

  /* Lock mutex. */
  status = pthread_mutex_lock(&log_locks[log_id]->mutex);
  if (status != 0)
    {
      result = -1;
      goto WRITE_FAIL;
    }

    /* Write the event line. */
  bzen_log_event_line(code, log_evtline_buffers[log_id], BZEN_LOG_EVENT_LINE_MAX_CHARS);
  status = fputs(log_evtline_buffers[log_id], log_locks[log_id]->fd);
  if (status == EOF)
    {
      result = -1;
      goto WRITE_FAIL_UNLOCK;
    }

  /* Write the message. */
  bzen_log_format_message(message, log_message_buffers[log_id], BZEN_LOG_MESSAGE_MAX_CHARS);
  status = fputs(log_message_buffers[log_id], log_locks[log_id]->fd);
  if (status == EOF)
    {
      result = -1;
      goto WRITE_FAIL_UNLOCK;
    }

  status = fputc('\n', log_locks[log_id]->fd);
  status = fputs(BZEN_LOG_ENTRY_DELIMITER, log_locks[log_id]->fd);

  /* Flush the stream buffer. */
  fflush(log_locks[log_id]->fd);

  /* If result is nonzero, log file had to be opened for write. Close it now. */
  if (result < 0)
    {
      status = bzen_log_close(name);
      if (status < 0)
	{
	  result = -1;
	  goto WRITE_FAIL_UNLOCK;
	}
    }

  /* SUCCESS. */
  result = 0;

 WRITE_FAIL_UNLOCK:

  /* Unlock mutex. */
  status = pthread_mutex_unlock(&log_locks[log_id]->mutex);
  if (status != 0)
    {
      result = -1;
      goto WRITE_FAIL;
    }

 WRITE_FAIL:

  return result;
}
