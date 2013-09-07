#ifndef ZB_SQLITE_H
#define ZB_SQLITE_H
#include "zbase_lib/include/zb.h"
#include "sqlite/include/sqlite3.h"

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
