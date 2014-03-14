#ifndef ZB_H
#define ZB_H
#include "zipolib_cpp/include/z_string.h"
#include "zipolib_cpp/include/z_stl_map.h"
//#include "zipolib_cpp/include/z_time.h"
//#include "zipolib/include/z_obj.h"
//#include "sqlite/include/sqlite3.h"
#define Z_PROFILING
//#include "zipolib/include/z_trace.h"


typedef U32 zb_key_size;
typedef zb_key_size zb_key;


class zb_ds_record;
class zb_record;
class zb_recset;

class zb_table_dynamic;
class zb_table_static;
class zb_table_base;
class zb_ds_table;
class zb_ds_field;


class zb_zipobase;
class zb_source;
class zb_src_sl3;
class zb_ds_recordset_sl3;


enum zb_status
{
	zb_ok,
	zb_end_of_data,
	zb_not_supported,
	zb_not_implemented,
	zb_bad_param,
	zb_cant_open_file,
	zb_unknown_data_error,
	zb_data_source_not_open

};
extern ctext zb_status_text[];

#ifdef ZB_NO_OBJ_CLEANUP

class z_obj
{
};

#define ZPTR_COPY(_PTR_) (_PTR_)
#define ZPTR_DEL(_PTR_) 

#else

class z_obj
{
	int _ref_count;
public:
	z_obj();
	virtual ~z_obj() {}
	void ref_inc();
	int ref_dec();
};
inline z_obj*  z_obj_ptr_copy(z_obj* p) ;
inline void z_obj_ptr_delete(z_obj* p) ;


#define ZPTR_COPY(_PTR_) z_obj_ptr_copy(_PTR_);
#define ZPTR_DEL(_PTR_) z_obj_ptr_delete(_PTR_);
#endif
#endif
