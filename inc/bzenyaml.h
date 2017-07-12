/**
 * @file:	bzenyaml.h
 * @brief:	YAML parsing and emitting functions.
 * 
 * ROADMAP
 *
 * Load Process
 *   Parse
 *     character stream --> series of events
 *     fail on ill-formed input
 *   Compose
 *     series of events --> representation graph
 *     failure conditions
 *   Construct
 *     representation graph --> native data structures
 *     failure conditions
 *
 * Dump Process
 *   Represent
 *   Serialize
 *   Present
 *
 * The Barzensuit YAML parsing and emitting functions are tailored to suit the 
 * purposes of the Barzensuit project(s). The code is written specifically for
 * the Barzensuit project(s), however based on study of the YAML 1.2 spec.
 * @see http://www.yaml.org/spec/1.2/spec.html and the LibYAML C source code,
 * written by Kirill Simonov and released under the MIT license, which is 
 * included for compliance reasons. @see http://pyyaml.org/wiki/LibYAML.
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

#ifndef _BZEN_YAML_H_
#define _BZEN_YAML_H_

#include <config.h>
#include "bzensbuf.h"

/**
 * YAML process error codes.
 *
 * @typedef bzen_yaml_error_code_t
 * @{
 */
typedef enum bzen_yaml_error_code_e 
  {
    /** No error. */
    BZEN_YAML_ERROR_NONE,

    /** An error occurred trying to access a resource (e.g. file). */
    BZEN_YAML_ERROR_ACCESS, 

    /** An error occurred on allocating memory. */
    BZEN_YAML_ERROR_MALLOC,

    /** An error occurred on attempting to read YAML stream. */
    BZEN_YAML_ERROR_READ,

    /** An error occurred on attempting to scan YAML stream. */
    BZEN_YAML_ERROR_SCAN,

    /** An error occurred on attempting to parse YAML stream. */
    BZEN_YAML_ERROR_PARSE
  } bzen_yaml_error_code_t;
/**
 * @}
 */

/**
 * @typedef bzen_yaml_event_code_t
 * @{
 */
typedef enum bzen_yaml_event_code_e
  {
    /** No event */
    BZEN_YAML_EVENT_NONE
  } bzen_yaml_event_code_t;
/**
 * @}
 */

/**
 * @typedef bzen_yaml_event_t
 * @{
 */
typedef struct bzen_yaml_event_s
{
  /** The event type. */
  bzen_yaml_event_code_t type;

} bzen_yaml_event_t;
/**
 * @}
 */

/**
 * @typedef bzen_yaml_parser_t Encapsulates YAML load parsing process state
 */
typedef struct _bzen_yaml_parser_s
{
  /**
   * @name Error handling.
   * @{
   */
  
  /** Error code. */
  bzen_yaml_error_code_t error;

  /**
   * @}
   */

  /**
   * @name Reader
   * @{
   */

  /** Encapsulated input stream. */
  bzen_cbuflock_t* input;

  /**
   * @}
   */
} bzen_yaml_parser_t;

/**
 * Allocate memory for a new YAML event.
 * 
 * @return bzen_yaml_event_t* Newly allocated struct or NULL on fail.
 */
bzen_yaml_event_t* bzen_yaml_event_create();

/**
 * Free memory allocated for given YAML event.
 *
 * @param[in] bzen_yaml_event* Pointer to memory block to free.
 *
 * @return @c 0 on success, @c -1 on error.
 */
int bzen_yaml_event_destroy(bzen_yaml_event_t* event);

/**
 * Restore given YAML event to default state.
 *
 * @param[in,out] bzen_yaml_event_t* event Pointer to event.
 *
 * @return @c 0 on success, @c -1 on fail.
 */
int bzen_yaml_event_restore(bzen_yaml_event_t* event);

/**
 * Allocate memory for a new YAML parse process state data struct.
 *
 * @return bzen_yaml_parser_t* Newly allocated struct or NULL on fail.
 */
bzen_yaml_parser_t* bzen_yaml_parser_create();

/**
 * Free memory allocated for given YAML parser.
 *
 * @param[in] bzen_yaml_parser_t* Pointer to memory block to free.
 *
 * @return @c 0 on success @c -1 on error.
 */
int bzen_yaml_parser_destroy(bzen_yaml_parser_t* parser);

/**
 * Get next event in YAML stream.
 *
 * @param[in,out] bzen_yaml_parser_t* parser Parser encapsulating YAML stream.
 * @param[in,out] bzen_yaml_event_t* event Sink for event data.
 *
 * @return @c 0 on success, @c -1 on error.
 */
int bzen_yaml_parser_next_event(bzen_yaml_parser_t* parser, bzen_yaml_event_t* event);

/**
 * Restore given YAML parser to default state.
 *
 * @param[in,out] bzen_yaml_parser_t* parser Pointer to the parser structure.
 * 
 * @return @c 0 on success, @c -1 on error.
 */
int bzen_yaml_parser_restore(bzen_yaml_parser_t* parser);

/**
 * Pass FILE* open for reading as input to YAML parser.
 *
 * @param[in,out] bzen_yaml_parser_t* parser YAML Parser.
 * @param[in] FILE* file Descriptor of a file open for reading.
 * 
 * @return void.
 */
void bzen_yaml_parser_set_input_file(bzen_yaml_parser_t* parser, FILE* file);

/**
 * Pass string as input to YAML parser.
 *
 * @param[in,out] bzen_yaml_parser_t* parser YAML Parser.
 * @param[in] const char* input The input data.
 * @param[in] size_t size The size of the input data in bytes.
 * 
 * @return void.
 */
void bzen_yaml_parser_set_input_string(bzen_yaml_parser_t* parser,
				       const char* input,
				       size_t size);

#endif /* _BZEN_YAML_H_ */
