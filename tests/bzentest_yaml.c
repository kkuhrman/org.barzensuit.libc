/**
 * @file:	bzentest_yaml.c
 * @brief:	Unit test YAML processing functions.
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
#include <stdlib.h>
#include <fcntl.h>

/* libzenc includes */
#include "bzentest.h"
#include "bzenyaml.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  const char* testfilev, testfilei;

  bzen_yaml_parser_t* parser;
  bzen_yaml_event_t* event;

  /* Test bzen_yaml_parser_create(). */
  parser = bzen_yaml_parser_create();
  if (BZENPASS != BZENTEST_TRUE(parser != NULL))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }
  
  /* Test bzen_yaml_event_create(). */
  event = bzen_yaml_event_create();
  if (BZENPASS != BZENTEST_TRUE(event != NULL))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test parser with valid YAML from a file. */
  testfilev = getenv("BZENTEST_FILE_YAML_VALID");
  FILE* fin = fopen(testfilev, "r");
  if (BZENPASS != BZENTEST_TRUE(__freadable(fin) != 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }
  bzen_yaml_parser_set_input_file(parser, fin);
  if (BZENPASS != BZENTEST_TRUE(parser->error == BZEN_YAML_ERROR_NONE))
    {
      fprintf(stderr, "\n\tYAML parser produced error code %d file name %s\n", 
	      parser->error,
	      testfilev);
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* @todo: test parsing function(s). */

  /* Test the restore state function. */
  bzen_yaml_parser_restore(parser);
  if (BZENPASS != BZENTEST_TRUE(parser->error == BZEN_YAML_ERROR_NONE))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Close the file and make sure set input fn fails. */
  fclose(fin);
  bzen_yaml_parser_set_input_file(parser, fin);
  if (BZENPASS != BZENTEST_TRUE(parser->error == BZEN_YAML_ERROR_ACCESS))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test bzen_yaml_event_destroy(). */
  status = bzen_yaml_event_destroy(event);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test bzen_yaml_parser_destroy(). */
  status = bzen_yaml_parser_destroy(parser);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

 END_TEST:
  return result;
}
