/**
 * @file:	bzenyaml.c
 * @brief:	YAML parsing and emitting functions.
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
#include "bzendbug.h"
#include "bzenmem.h"
#include "bzenyaml.h"

/**
 * Allocate memory for a new YAML parse process state data struct.
 *
 * Application is responsible for freeing allocated memory by calling 
 * @see bzen_yaml_parser_destroy(). 
 *
 * @return bzen_yaml_parser_t* Newly allocated struct or NULL on fail.
 */
bzen_yaml_parser_t* bzen_yaml_parser_create()
{
  bzen_yaml_parser_t* parser;
  size_t parser_size;
  int status;

  /* Allocate memory for parser. */
  parser_size = xcast_size_t(sizeof(bzen_yaml_parser_t));
  parser = (bzen_yaml_parser_t*)bzen_malloc(parser_size);
  if (parser == NULL)
    {
      goto CREATE_FAIL;
    }

  status = bzen_yaml_parser_restore(parser);
  if (status != 0)
    {
      bzen_yaml_parser_destroy(parser);
      goto CREATE_FAIL;
    }
  
 CREATE_FAIL:

  return parser;
}

/**
 * Free memory allocated for given data structure.
 *
 * @param[in] bzen_yaml_parser_t* Pointer to memory block to free.
 *
 * @return @c 0 on success @c -1 on error.
 */
int bzen_yaml_parser_destroy(bzen_yaml_parser_t* parser)
{
  int return_code;
  double timeout = 3; /* time in seconds to wait for lock on mutex. */

  if (parser == NULL)
    {
      /* Nothing to do. */
      return_code = 0;
      goto DESTROY_FAIL;
    }

  /* Destroy input buffer lock. This does not close and destroy input 
     stream unless the keep_open flag is set to zero. Otherwise, this
     responsibility belongs to the application. */  
  return_code = bzen_sbuf_destroy(parser->input, timeout);
  if (return_code != 0)
    {
      goto DESTROY_FAIL;
    }

  /* Deallocate memory for parser. */
  bzen_free(parser);

 DESTROY_FAIL:

  return return_code;
}

/* Restore parser data structure to initial state. */
int bzen_yaml_parser_restore(bzen_yaml_parser_t* parser)
{
  int return_code = -1;

  /* A non-null pointer is expected. */
  BZEN_ASSERT(parser);

  /* Initialize parser members. */
  parser->input = NULL;
  parser->error = BZEN_YAML_ERROR_NONE;

  return_code = 0;

 RESTORE_FAIL:
  
  return return_code;
}

/**
 * Pass FILE* open for reading as input to YAML parser.
 *
 * @param[in,out] bzen_yaml_parser_t* parser YAML Parser.
 * @param[in] FILE* file Descriptor of a file open for reading.
 * 
 * @return void.
 */
void bzen_yaml_parser_set_input_file(bzen_yaml_parser_t* parser, FILE* file)
{
}

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
				       size_t size)
{
}
