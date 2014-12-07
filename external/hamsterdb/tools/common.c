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
#include <time.h>

#include <ham/hamsterdb_int.h>

#include "common.h"

void
print_banner(const char *program_name)
{
  uint32_t maj, min, rev;
  ham_get_version(&maj, &min, &rev);

  printf("hamsterdb %s%d.%d.%d - Copyright (C) 2005-2014 "
       "Christoph Rupp (chris@crupp.de).\n\n",
       ham_is_pro() ? "pro " : "", maj, min, rev);

  if (!ham_is_pro())
    printf(
"Licensed under the Apache License, Version 2.0 (the \"License\");\n"
"you may not use this file except in compliance with the License.\n"
"You may obtain a copy of the License at\n"
"\n"
"    http://www.apache.org/licenses/LICENSE-2.0\n"
"\n"
"Unless required by applicable law or agreed to in writing, software\n"
"distributed under the License is distributed on an \"AS IS\" BASIS,\n"
"WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n"
"See the License for the specific language governing permissions and\n"
"limitations under the License.\n\n");
  else {
    time_t end = ham_is_pro_evaluation();
    if (end != 0)
      printf("Commercial evaluation version; valid till %s.\n", ctime(&end));
    else
      printf("Commercial version.\n\n");
  }
}

