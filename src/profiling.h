/***********************************************************************************
 *                         This file is part of dynareadout
 *                    https://github.com/PucklaMotzer09/dynareadout
 ***********************************************************************************
 * Copyright (c) 2022 PucklaMotzer09
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product,
 * an acknowledgment in the product documentation would be appreciated but is
 * not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ************************************************************************************/

#ifndef PROFILING_H
#define PROFILING_H

#include <stddef.h>
#include <string.h>
#include <time.h>

typedef struct {
  char const **execution_times_names;
  double *execution_times;
  size_t num_execution_times;
} profiling_context_t;

extern profiling_context_t profiling_context;

#ifdef __cplusplus
extern "C" {
#endif

void _END_PROFILE_SECTION(const char *name, clock_t start_time);
void END_PROFILING(const char *out_file_name);

#ifdef __cplusplus
}
#endif

#define BEGIN_PROFILE_FUNC() clock_t func_profiling_start_time = clock()
#define END_PROFILE_FUNC()                                                     \
  _END_PROFILE_SECTION(__FUNCTION__, func_profiling_start_time)

#endif
