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

#ifndef HAM_BENCH_MISC_HPP
#define HAM_BENCH_MISC_HPP

#include <stdio.h>

#define LOG_VERBOSE(x)  while (m_config->verbose) { printf x; break; }
#define LOG_TRACE(x)    do { printf("[info] "); printf x; } while (0)
#define LOG_ERROR(x)    do { printf("[error] "); printf x; } while (0)

#endif /* HAM_BENCH_MISC_HPP */
