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
#include <fcntl.h>
#include "bzendbug.h"
#include "bzenmem.h"
#include "bzenyaml.h"

/* Allocate memory for a new YAML event. */
bzen_yaml_event_t* bzen_yaml_event_create()
{
  bzen_yaml_event_t* event;
  size_t event_size;
  int status;

  /* Allocate memory for event. */
  event_size = xcast_size_t(sizeof(bzen_yaml_event_t));
  event = (bzen_yaml_event_t*)bzen_malloc(event_size);
  if (event == NULL)
    {
      goto CREATE_FAIL;
    }

  status = bzen_yaml_event_restore(event);
  if (status != 0)
    {
      bzen_yaml_event_destroy(event);
      goto CREATE_FAIL;
    }

 CREATE_FAIL:

  return event;
}

/* Free memory allocated for given YAML event. */
int bzen_yaml_event_destroy(bzen_yaml_event_t* event)
{
  int return_code;
  double timeout = 3; /* time in seconds to wait for lock on mutex. */

  if (event == NULL)
    {
      /* Nothing to do. */
      return_code = 0;
      goto DESTROY_FAIL;
    }

  /* Deallocate memory for event. */
  bzen_free(event);
  return_code = 0;

 DESTROY_FAIL:

  return return_code;
}

/* Restore given YAML event to default state. */
int bzen_yaml_event_restore(bzen_yaml_event_t* event)
{
  int return_code = -1;

  /* A non-null pointer is expected. */
  BZEN_ASSERT(event);

  /* Initialize event members. */
  event->type = BZEN_YAML_EVENT_NONE;
  return_code = 0;

 RESTORE_FAIL:
  
  return return_code;
}

/* Allocate memory for a new YAML parser. */
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

/* Free memory allocated for given YAML parser. */
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

/* Get next event in YAML stream. */
int bzen_yaml_parser_next_event(bzen_yaml_parser_t* parser, bzen_yaml_event_t* event)
{
  int status;
  int return_code = 0;

  /* Expect valid pointers. */
  BZEN_ASSERT(parser);
  BZEN_ASSERT(event);

  /* Restore event to default state. */
  status = bzen_yaml_event_restore(event);
  if (status != 0)
    {
      return_code = -1;
      goto PARSE_FAIL;
    }

  /* @todo: get next event data. */

 PARSE_FAIL:

  return return_code;
}

/* Restore given YAML parser to default state. */
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

/* Pass FILE* open for reading as input to YAML parser. */
void bzen_yaml_parser_set_input_file(bzen_yaml_parser_t* parser, FILE* file)
{
  int fd; /* File descriptor */
  int status;

  /** Non-null pointers are expected. */
  BZEN_ASSERT(parser);
  BZEN_ASSERT(file);

  /** A valid file descriptor is expected. */
  fd = fileno(file);
  status = fcntl(fd, F_GETFL);
  /* status = __freadable(file); */
  if (status < 0)
    {
      /* File cannot be read. */
      perror("fcntl");
      /* fprintf(stderr, "\n\t__freadable(fd) returns %d\n", status); */
      parser->error = BZEN_YAML_ERROR_ACCESS;
      goto SET_INPUT_FAIL;
    }
  
  /** Set buffer input. */
  parser->input = bzen_sbuf_create_file(file);
  if (parser->input == NULL)
    {
      /* Failed to allocate memory for bzen_cbuflock_t. */
      parser->error = BZEN_YAML_ERROR_MALLOC;
      goto SET_INPUT_FAIL;
    }

 SET_INPUT_FAIL:

  return;
}

/* Pass string as input to YAML parser. */
void bzen_yaml_parser_set_input_string(bzen_yaml_parser_t* parser,
				       const char* input,
				       size_t size)
{
}
