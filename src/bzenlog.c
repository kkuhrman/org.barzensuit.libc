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
static bzen_loglock_t** log_locks = NULL;

/**
 * Encapsulated buffer counters.
 */
static size_t logs_used = 0;
static size_t logs_allocated = 0;

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
static const char* bzen_log_event_line(bzenlog_severity_code_t code)
{
  time_t now;
  struct tm* local_time;
  char event_time[BZEN_TIME_STR_BUFFER_SIZE];
  const char* micro_time = "0000"; /* reserved for future implementation */
  char* event_line;
  uid_t effective_user_id;
  gid_t effective_group_id;
  uid_t real_user_id;
  gid_t real_group_id;
  char symbolic_code;

  /* Severity code defaults to 'info' */
  symbolic_code = (code <= BZENLOG_DEBUG) ? BZENLOG_SEVERITY_CODE_SYMBOL[code] : 'I';

  /* Format current local date-time. */
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

  /* Allocate memory for the event line. */
  event_line = (char*)bzen_malloc(xcast_size_t(sizeof(char) *
					       (BZEN_LOG_LINE_MAX_CHARS + 1)));

  /* Write formatted event line. */
  sprintf(event_line, "%s %s %c %6d %6d %6d %6d\n", 
	  event_time, 
	  micro_time,
	  symbolic_code,
	  effective_user_id,
	  effective_group_id,
	  real_user_id,
	  real_group_id);

  return event_line;
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
static const char* bzen_log_format_message(const char* message)
{
  char* formatted_message;
  size_t formatted_message_size;
  size_t unfmtmsg_size;
  size_t num_new_lines;
  int raw_pos;
  int fmt_pos;
  int line_char_count;
  int last_space;
  char move_char;
  
  /* Fail on NULL */
  if (message == NULL)
    {
      formatted_message_size = xcast_size_t(sizeof(char) * 
					    strlen(BZEN_LOG_ENTRY_DELIMITER));
      formatted_message = (char*)bzen_malloc(formatted_message_size);
      sprintf(formatted_message, "%s", BZEN_LOG_ENTRY_DELIMITER);
      goto SKIP_FMT;
    }

  /* Size of unformatted message. */
  unfmtmsg_size = strlen(message);

  /* If n chars is less than max per line, skip all the word-wrap logic. */
  if (unfmtmsg_size <= BZEN_LOG_LINE_MAX_CHARS)
    {
      formatted_message_size = xcast_size_t(sizeof(char) * unfmtmsg_size);
      formatted_message = (char*)bzen_malloc(formatted_message_size);
      sprintf(formatted_message, "%s", message);
      goto SKIP_FMT;
    }

  /* Estimate number of new line chars. */
  num_new_lines = unfmtmsg_size/BZEN_LOG_LINE_MAX_CHARS;
  num_new_lines += (unfmtmsg_size%BZEN_LOG_LINE_MAX_CHARS) ? 1 : 1;

  /* Allcoate memory for formatted message. */
  formatted_message_size = xcast_size_t(sizeof(char) * (unfmtmsg_size + num_new_lines));
  formatted_message = (char*)bzen_malloc(formatted_message_size);

  /* Copy characters to fomatted message buffer. */
  raw_pos = 0;
  fmt_pos = 0;
  line_char_count = 0;
  last_space = 0;
  while (raw_pos < unfmtmsg_size)
    {
      /* Get next char in message. */
      move_char = message[raw_pos];

      if (line_char_count < BZEN_LOG_LINE_MAX_CHARS)
	{
	  /* Bookmark if char is punctuation or whitespace. */
	  if (ispunct(move_char) || isspace(move_char))
	    {
	      last_space = raw_pos;
	    }

	  /* Copy character to formatted message buffer. */
	  formatted_message[fmt_pos];

	  /* Next char. */
	  raw_pos++;
	  fmt_pos++;
	  line_char_count++;
	}
      else
	{
	  /* Limit reached. Check if in middle of word. */
	  if (last_space != raw_pos)
	    {
	      /* Char pointer is in middle of word. Force new line after last bookmark. */
	      fmt_pos = last_space + 1;
	      formatted_message[fmt_pos] = '\n';
	      
	      /* Reset char counter to beginning of last word. */
	      raw_pos = last_space + 2;
	      fmt_pos++;
	      line_char_count = 0;
	    }
	}
    }

 SKIP_FMT:
  
  return formatted_message;
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
  size_t actual_name_allocate;
  size_t actual_path_allocate;
  size_t actual_lock_allocate;
  int log_id;
  int log_find;
  int status;
  int result;

  /* Attempt to open the directory for test file output. */
  logdir = (char*)getenv("BZEN_LOG_DIR");
  result = access(logdir, R_OK | W_OK);
  if (result < 0)
  {
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
	      actual_name_allocate = logs_allocated;
	      actual_path_allocate = logs_allocated;
	      actual_lock_allocate = logs_allocated;
	      log_names = (char**)bzen_realloc(log_names,
					       &actual_name_allocate,
					       xcast_size_t(sizeof(char*)));
	      log_paths = (char**)bzen_realloc(log_paths,
					       &actual_path_allocate,
					       xcast_size_t(sizeof(char*)));

	      /* use smaller of actual allcoated sizes. @see next comment.  */
	      actual_name_allocate = (actual_name_allocate < actual_path_allocate) ?
		actual_name_allocate : actual_path_allocate;

	      log_locks = (bzen_loglock_t**)bzen_realloc(log_locks,
							 &actual_lock_allocate,
							 xcast_size_t(sizeof(bzen_loglock_t*)));

	      /* @todo: A  disparity in number of names vs. locks presents a 
	         potentially unsafe situation. We need to correct any disparity 
	         prior to proceeding. The gnulib xrealloc() increases block size
	         by a factor of 1.5. At present, we simply pick smaller or two. */
	      logs_allocated = (actual_lock_allocate < actual_name_allocate) ?
		actual_lock_allocate : actual_name_allocate;
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
      /* @todo: error loging */
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
      /* @todo: error loging */
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
  /* fmtstr = bzen_log_event_line(code); */
  status = fputs(bzen_log_event_line(code), log_locks[log_id]->fd);
  /* bzen_free(fmtstr); */
  if (status == EOF)
    {
      result = -1;
      goto WRITE_FAIL_UNLOCK;
    }

  /* Write the message. */
  /* fmtstr = bzen_log_format_message(message); */
  status = fputs(bzen_log_format_message(message), log_locks[log_id]->fd);
  /* bzen_free(fmtstr); */
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
  /* fmtstr = bzen_log_event_line(code); */
  status = fputs(bzen_log_event_line(code), log_locks[log_id]->fd);
  /* bzen_free(fmtstr); */
  if (status == EOF)
    {
      result = -1;
      goto WRITE_FAIL_UNLOCK;
    }

  /* Write the message. */
  /* fmtstr = bzen_log_format_message(message); */
  status = fputs(bzen_log_format_message(message), log_locks[log_id]->fd);
  /* bzen_free(fmtstr); */
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
