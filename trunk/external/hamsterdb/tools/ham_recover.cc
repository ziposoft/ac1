/*
 * Copyright (C) 2005-2014 Christoph Rupp (chris@crupp.de).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ham/hamsterdb.h>

#include "getopts.h"
#include "common.h"

#define ARG_HELP      1

/*
 * command line parameters
 */
static option_t opts[] = {
  {
    ARG_HELP,         // symbolic name of this option
    "h",          // short option
    "help",         // long option
    "this help screen",   // help string
    0 },          // no flags
  { 0, 0, 0, 0, 0 } /* terminating element */
};

static void
error(const char *foo, ham_status_t st) {
  printf("%s() returned error %d: %s\n", foo, st, ham_strerror(st));
  exit(-1);
}

int
main(int argc, char **argv) {
  unsigned opt;
  char *param, *filename = 0;

  ham_status_t st;
  ham_env_t *env;

  getopts_init(argc, argv, "ham_recover");

  while ((opt = getopts(&opts[0], &param))) {
    switch (opt) {
      case GETOPTS_PARAMETER:
        if (filename) {
          printf("Multiple files specified. Please specify "
               "only one filename.\n");
          return (-1);
        }
        filename = param;
        break;
      case ARG_HELP:
        print_banner("ham_recover");

        printf("usage: ham_recover file\n");
        printf("usage: ham_recover -h\n");
        printf("     -h:     this help screen (alias: --help)\n");
        return (0);
      default:
        printf("Invalid or unknown parameter `%s'. "
             "Enter `ham_dump --help' for usage.", param);
        return (-1);
    }
  }

  if (!filename) {
    printf("Filename is missing. Enter `ham_recover --help' for usage.\n");
    return (-1);
  }

  /* open the environment and check if recovery is required */
  st = ham_env_open(&env, filename,
        HAM_ENABLE_RECOVERY | HAM_ENABLE_TRANSACTIONS, 0);
  if (st == HAM_FILE_NOT_FOUND) {
    printf("File `%s' not found or unable to open it\n", filename);
    return (-1);
  }
  if (st == 0) {
    printf("File `%s' does not need to be recovered\n", filename);
    return (0);
  }
  else if (st != HAM_NEED_RECOVERY)
    error("ham_env_open", st);

  /* now start the recovery */
  st = ham_env_open(&env, filename,
        HAM_AUTO_RECOVERY | HAM_ENABLE_TRANSACTIONS, 0);
  if (st)
    error("ham_env_open", st);

  /* we're already done */
  st = ham_env_close(env, 0);
  if (st != HAM_SUCCESS)
    error("ham_env_close", st);

  return (0);
}
