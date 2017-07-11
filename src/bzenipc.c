/**
 * @file:	bzenipc.h
 * @brief:	Interprocess communication and process forking functions.
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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "bzenlog.h"
#include "bzenipc.h"

/* Fork child process and run as daemon listening on BSP socket. */
int bzen_ipc_fork(void* exitfn, void* ipcfn, void* ipcopt, void* logopt)
{
  pid_t process_id = 0;
  pid_t sid = 0;
  int exit_code;
  int* result = &exit_code;
  int status;
  bzen_log_option_t* logoptions;

  /* Create child process */
  process_id = fork();

  /* Exit with fail code if fork() fails  */
  if (process_id < 0)
    {
      /* @todo: error logging*/
      exit_code = -1;
      goto DAEMON_FAIL;
    }

  /* Terminate parent process. */
  if (process_id > 0)
    {
      exit(0);
    }

  /* Unmask file mode. */
  umask(0);

  /* Set new session. */
  sid = setsid();
  if(sid < 0)
    {
      /* @todo: error logging */
      exit_code = -1;
      goto DAEMON_FAIL;
    }

  /* Change pwd to root. */
  chdir("/");

  /* Close stdin. stdout and stderr. */
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  /* Cleanups on Exit */
  atexit (exitfn);

  /* @todo: if this cannot be option it must come from /etc/conf. */
  if (logopt != NULL)
    {
      logoptions = (bzen_log_option_t*)logopt;
      /* bzen_xmpp_srv_log_severity_level_set(logoptions->severity_code); */
    }

  /* @todo: Open message queue for processing. */

  
 DAEMON_FAIL:

  return exit_code;
}
