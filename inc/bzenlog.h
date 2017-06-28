/**
 * @file:	bzenlog.h
 * @brief:	Manages output to log files.
 * 
 * Log conventions for Project Barzensuit:
 *
 * 1. All logs are stored in a shared directory (currently manually configured).
 * 2. Error logs are given the name [package]-error
 * 3. Logs containing non-error messages are named after their parent package.
 * 4. Log lines are limited to 80 characters (not counting new line).
 * 5  Each logged message consists of two or more lines, event data and message.
 * 6. The event data consists of the following:
 * 6.1  Event fields are space delimited
 * 6.2  10 character date field YYYY-mm-dd
 * 6.3  8 character time field HH:MM:SS
 * 6.4  4 characters reserved for micro time
 * 6.4  1 character severity code field
 * 6.5  4 x 5 character fields for real and effective user/group ids
 * 6.6  The remaining 31 characters are reserved and currently not in use.
 * 7. The message occupies one or more lines after the event line.
 * 7.1  Message text is word wrapped with no line occupying more than 80 chars.
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

#ifndef _BZEN_LOG_H_
#define _BZEN_LOG_H_

#include <config.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * Default name of directory where log files are written to.
 */
#define BZEN_LOG_DIR_DEFAULT_NAME "bzenlog"

/**
 * Max number of chars in log path.
 */
#define BZEN_LOG_MAX_PATH_CHARS 255

/**
 * Log line lengths, string buffers  and delimiters etc.
 */
#define BZEN_LOG_LINE_MAX_CHARS 80

#define BZEN_LOG_ENTRY_DELIMITER  "\t--\n"

#define BZEN_LOG_EVENT_LINE_MAX_CHARS BZEN_LOG_LINE_MAX_CHARS

#define BZEN_LOG_MESSAGE_MAX_CHARS 1024

#define BZEN_LOG_EVENT_LINE_DTM_LEN (sizeof(char) * 32)

/**
 * Default fopen() attribute (append).
 */
#define BZEN_LOG_FOPEN_DEFAULT_ATTR "a"

/**
 * @typedef bzen_log_stat_t
 * 
 * @property char status 'o' for open, 'c' for closed, 'n' for NULL.
 */
typedef struct _bzen_loglock_s
{
  pthread_mutex_t mutex;
  FILE* fd;
  char status;
} bzen_loglock_t;

/**
 * @enum Log event severity codes.
 */
enum BZENLOG_SEVERITY_CODE 
  {
    BZENLOG_ERROR = 0, /* E */
    BZENLOG_WARNING,   /* W */
    BZENLOG_STATUS,    /* S */
    BZENLOG_INFO,      /* I */
    BZENLOG_DEBUG      /* D */
  };

/**
 * @typedef bzenlog_severity_code_t
 */
typedef unsigned short int bzenlog_severity_code_t;

/**
 * Report to syslog failure to access log resource.
 *
 * @param const char* package Name of package reporting error.
 * @param const char* resource Name of resource package could not access.
 * @param const char* function Name of function which returned a fail code.
 * @param const int code Error code returned by access function.
 *
 * @return void.
 */
static void bzen_log_handle_access_fail(const char* package,
					const char* resource,
					const char* function,
					int code);

/**
 * Close the given named log.
 *
 * The 'close()' functions simply close the encapsulated log file.
 * They do not free memory allocated on the heap nor do they effect
 * mutexes. The log files can be reopened and used again.
 *
 * Use the 'destroy()' functions to free memory on the heap and 
 * destroy mutexes.
 *
 * @param const char* name Name of log file to close.
 *
 * @return int 0 If log file is closed, otherwise -1.
 */
int bzen_log_close(const char* name);

/**
 * Close all open log files.
 *
 * @see bzen_log_close() about effect of close functions on memory
 * allocated on the heap and mutexes.
 *
 * @return int 0 If all log files are closed, otherwise -1.
 */
int bzen_log_close_all();

/**
 * Returns the full path to the log files directory.
 *
 * Function will first seek an accesible directory defined by the environment
 * variable BZEN_LOG_DIR. If this fails, it will look for an accesible directory
 * relative to the PWD of the program. If both searches fail, it returns NULL.
 *
 * @return const char* path.
 */
const char* bzen_log_dir();

/**
 * Generate event line text.
 *
 * Event lines are 80 chars plus terminating null and are dynamically allocated
 * on the heap. It is the callers responsibility to free memory after use. If an
 * invalid severity code is passed, default is  BZENLOG_INFO.
 *
 * @param bzenlog_severity_code_t code Severity code. 
 * @param char* buffer Buffer to write to.
 * @param size_t size Size of buffer.
 *
 * @return int 0 on SUCCESS otherwise -1.
 */
static int  bzen_log_event_line(bzenlog_severity_code_t code,
				char* buffer,
				size_t size);

/**
 * Find corresponding id of named log if exists.
 *
 * @param const char* name Name of log to search for.
 *
 * @return int Id of log file >= 0 if exists otherwise -1.
 */
static int bzen_log_find_id(const char* name);

/**
 * Formats a log message so no line exceeds max chars and no words are broken.
 *
 * @param const char* message Unformatted message.
 * @param char* buffer Buffer to write to.
 * @param size_t size Size of buffer.
 *
 * @return int 0 on SUCCESS otherwise -1.
 */
static int  bzen_log_format_message(const char* message,
				    char* buffer,
				    size_t size);

/**
 * Check if given log file is open. 
 *
 * @param const char* name Name of log file.
 *
 * @return int 0 If log file is open, otherwise -1.
 */
int bzen_log_is_open(const char* name);

/**
 * Open a log file with given attributes. 
 *
 * @param const char* name Name of log file.
 * @param const char* attr Open attributes (e.g. 'r', 'w') @see fopen().
 *
 * @return int 0 If log file is open, otherwise -1.
 */
int bzen_log_open(const char* name, const char* attr);

/**
 * Write a message to the given log.
 *
 * @param const char* name The name of the log to write to.
 * @param bzenlog_severity_code_t code Severity code. 
 * @param const char* message The message to write.
 *
 * @return int 0 if the message was written otherwise -1.
 */
int bzen_log_write(const char* name, 
                   bzenlog_severity_code_t code, 
                   const char* message);

/**
 * Write a message to the given log. Open log file if necessary.
 *
 * The purpose of bzen_log_write_stat() is to send messages to the log reliably
 * while maintaining the state of the target log file and minimizing the amount
 * of code to get the job done. The typical requirements to log a message are to
 * open the file, write the message and close the file. Of course, repeated open
 * and close calls may be expensive and must be weighed against the cost of 
 * keeping the file open indefinitely. This function allows the developer to 
 * decide which strategy suits their needs best and also allows others to send
 * messages to the log reliably without changing the open/close strategy in 
 * effect. If the file is closed, bzen_log_write_stat() will open it, write the
 * message and close it again. If the file is open bzen_log_write_stat() will 
 * write the message and leave the file open.
 *
 * @param const char* name The name of the log to write to.
 * @param bzenlog_severity_code_t code Severity code. 
 * @param const char* message The message to write.
 *
 * @return int 0 if the message was written otherwise -1.
 */
int bzen_log_write_stat(const char* name, 
                        bzenlog_severity_code_t code, 
                        const char* message);

#endif /* _BZEN_LOG_H_ */
