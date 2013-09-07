#ifndef ZB_H
#define ZB_H
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_string.h"
#include "zipolib/include/z_time.h"
#include "zipolib/include/z_obj.h"
#include "sqlite/include/sqlite3.h"
#define Z_PROFILING
#include "zipolib/include/z_trace.h"
typedef U64 zb_key_size;
typedef zb_key_size zb_key;
class zb_recp;
class zb_record;
class zb_recset;
class zb_table;
class zb_zipobase;
class zb_source;
class zb_src_sl3;
class zb_src_set_sl3;


enum zb_status
{
	zb_ok,
	zb_end_of_data,
	zb_bad_param,
	zb_cant_open_file,
	zb_unknown_data_error,
	zb_data_source_not_open

};
extern ctext zb_status_text[];


#endif
