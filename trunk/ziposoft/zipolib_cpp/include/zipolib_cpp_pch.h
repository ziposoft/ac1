#ifndef zipolib_cpp_pch_h
#define zipolib_cpp_pch_h
/*_______________________________________________________________________*\

   zipolib_cpp.h

\*_______________________________________________________________________*/
#include "zipolib_c/include/zipo.h"
#include "zipolib_c/include/z_util.h"
#include "zipolib_c/include/z_dbg.h"
#include "zipolib_c/include/z_time.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#ifdef UNIX
#include <stdarg.h>
#define stricmp strcasecmp
#endif
#endif
#include <stdarg.h>
#include <stdio.h>