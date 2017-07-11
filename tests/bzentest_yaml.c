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

/* libzenc includes */
#include "bzentest.h"
#include "bzenyaml.h"

int main (int argc, char *argv[])
{
  int result = BZEN_TEST_EVAL_PASS;
  int status;
  const char* testfilev, testfilei;

  bzen_yaml_parser_t* parser;

  /* Test bzen_yaml_parser_create(). */
  parser = bzen_yaml_parser_create();
  if (BZENPASS != BZENTEST_TRUE(parser != NULL))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  /* Test bzen_yaml_parser_destroy(). */
  status = bzen_yaml_parser_destroy(parser);
  if (BZENPASS != BZENTEST_EQUALS_N(status, 0))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }

  testfilev = getenv("BZENTEST_FILE_YAML_VALID");
  FILE* fin = fopen(testfilev, "r");
  if (BZENPASS != BZENTEST_TRUE(fin != NULL))
    {
      BZENTEST_EXIT_FAIL(__FILE__, __LINE__);
    }
  fclose(fin);

 END_TEST:
  return result;
}
