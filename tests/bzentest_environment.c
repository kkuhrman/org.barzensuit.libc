/**
 * @file:	bzentest_environment.c
 * @brief:	Unit test the test environment settings.
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
#include <dirent.h>

/* libzenc includes */

int main (int argc, char *argv[])
{
  int result = 0;
  const char* destdir;
  DIR* testdir;
  
  /* Attempt to open the directory for test file output. */
  destdir = getenv("BZENTEST_TEMP_DIR");
  if (destdir == NULL)
    {
      perror("getenv");
      fprintf (stderr, "Could not read environmkent variable BZENTEST_TEMP_DIR");
      result = -1;
      goto END_TEST;
    }
  testdir = opendir(destdir);
  if (testdir == NULL)
    {
      perror("opendir");
      fprintf (stderr, "socket temp dir = %s\n", destdir);
      result = -1;
      goto END_TEST;
    }
  
  /* Close the test directory. */
  closedir(testdir);

END_TEST:
  return result;
}
