#ifndef zipolib_cpp_pch_h
#define zipolib_cpp_pch_h
/*_______________________________________________________________________*\

   zipo.h

\*_______________________________________________________________________*/
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_util.h"
#include "zipolib/include/z_dbg.h"
#include "zipolib/include/z_time.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#ifdef UNIX
#include <stdarg.h>
#define stricmp strcasecmp
#endif
#endif
#include "zipolib/include/z_string.h"
#include "zipolib/include/z_stl_map.h"
#include "zipolib/include/z_stl_list.h"
#include "zipolib/include/z_stl_vector.h"
#include "zipolib/include/ptypes_cset.h"
#include "zipolib/include/z_trace.h"
#include "zipolib/include/z_error.h"