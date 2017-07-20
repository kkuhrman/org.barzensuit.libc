/**
 * @file:	bzenapi.h
 * @brief:      Public Interface (API).
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

#ifndef _BZEN_API_H_
#define _BZEN_API_H_

#include <config.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

/**
 * @defgroup ipc Interprocess Communications
 * @{
 */

/**
 * @typedef bzen_ipc_option_t
 */
typedef struct _bzen_ipc_option_s
{
  size_t buffer_size;
  size_t queue_length;
  uint32_t port;
} bzen_ipc_option_t;

/**
 * Fork child process and run as daemon listening on BSP socket.
 *
 * @param[in] void* exitfn Callback function passed to atexit().
 * @param[in] void* ipcfn Callback function for ipc thread.
 * @param[in] void* bzen_ipc_option_t ipcot IPC options
 * @param[in] void* @todo bzen_log_option_t logopt Options for Logging
 */
int bzen_ipc_fork(void* exitfn, void* ipcfn, void* ipcopt, void* logopt);
/**
 * @}
 */

/**
 * @defgroup stream Stream Manipulation
 * @{
 */
#define BZEN_OPENTYPE_SIZE 3
#define BZEN_STREAM_MIN_BUFFER_SIZE 32

/**
 * @typedef bzen_stream_t
 * @{
 */
typedef struct _bzen_stream_s
{
  /** The encapsulated stream. */
  FILE* file;

  /** Size of the stream buffer in bytes. */
  size_t size;

  /** Read/write attributes of stream. */
  char opentype[BZEN_OPENTYPE_SIZE];

  /** Buffer for file name or IO.  */
  char** buffer;

} bzen_stream_t;
/**
 * @}
 */

/**
 * Close stream.
 *
 * @param[in,out] bzen_stream_t* stream A pointer to an open stream.
 *
 * @return @c 0 on success otherwise -1.
 */
int bzen_stream_close(bzen_stream_t* stream);

/**
 * Free memory for given bzen_stream_t struct.
 *
 * @param[in,out] bzen_stream_t* stream Address of memort block to free.
 *
 * @return @c 0 on success otherwise @c -1 on error.
 */
int bzen_stream_delete(bzen_stream_t* stream);

/**
 * Open stream as a file.
 *
 * @param[in,out] bzen_stream_t* stream A pointer to the opened stream.
 * @param[in] const char* name Name of file.
 * @param[in] const char* type Open type per fopen() define.
 *
 * @return @c 0 on success otherwise -1.
 */
int bzen_stream_fopen(bzen_stream_t* stream,
		      const char* name,
		      const char* type);

/**
 * Open stream as a fixed size buffer in memory.
 *
 * @param[in,out] bzen_stream_t* stream A pointer to the opened stream.
 * @param[in] size_t size The size of the I/O buffer in bytes.
 * @param[in] const char* type Open type per fopen() define.
 *
 * @return @c 0 on success otherwise -1.
 */
int bzen_stream_fmemopen(bzen_stream_t* stream,
			 size_t size,
			 const char* type);

/**
 * Get a character from the given stream.
 *
 * This function reads the next character as an unsigned char from the
 * stream and returns its value, converted to an int. If an end-of-file 
 * condition or read error occurs, EOF is returned instead. 
 *
 * @param[in] bzen_stream_t* stream Stream to get character from.
 *
 * @return int Next character in stream.
 */
int bzen_stream_getc(bzen_stream_t* stream);

/**
 * Open stream as a dynamic buffer in memory.
 *
 * @param[in,out] bzen_stream_t* stream A pointer to the opened stream.
 *
 * @return @c 0 on success otherwise -1.
 */
int bzen_stream_open_memstream(bzen_stream_t* stream);

/**
 * Allocate memory for a bzen_stream_t struct.
 *
 * @param[in,out] bzen_stream_t** stream Address of newly allocated struct.
 *
 * @return @c 0 on success otherwise @c -1 on error.
 */
int bzen_stream_new(bzen_stream_t** stream);

/**
 * Put a character to the given buffer.
 *
 * The fputc function converts the character c to type unsigned char, and writes
 * it to the stream. EOF is returned if a write error occurs; otherwise the 
 * character c is returned. 
 *
 * @param int c The character to put.
 * @param[in, out] bzen_stream_t* stream Stream to put character to.
 *
 * @return int The put character or EOF.
 */
int bzen_stream_putc(int c, bzen_stream_t* stream);

/**
 * Set file position to beginning of stream and reset error indicator.
 *
 * @param[in,out] bzen_stream_t* stream Stream to rewind.
 *
 * @return @c 0 on success otherwise @c -1 on error.
 */
int  bzen_stream_rewind(bzen_stream_t* stream);

/**
 * @}
 */
#endif /* _BZEN_API_H_ */
