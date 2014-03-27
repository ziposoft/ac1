#ifndef ZB_H
#define ZB_H
#include "zbase_lib/include/zb_pch.h"

#define Z_PROFILING


typedef U32 zb_key_size;
typedef zb_key_size zb_key;


class zb_ds_record;
class zb_ds_recordset;

class zb_datum;
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


enum 
{
	zb_status_ok,
	zb_status_end_of_data,
	zb_status_not_supported,
	zb_status_not_implemented,
	zb_status_bad_param,
	zb_status_cant_open_file,
	zb_status_unknown_data_error,
	zb_status_ds_not_open,
	zb_status_ds_open_read,
	zb_status_ds_open_write,
	zb_status_ds_open_needs_commit,

};
extern ctext zb_status_text[];

#ifdef ZB_NO_OBJ_CLEANUP

class z_refcount
{
};

#define ZPTR_COPY(_PTR_) (_PTR_)
#define ZPTR_DEL(_PTR_) 

#else

class z_refcount
{
	int _ref_count;
public:
	z_refcount();
	virtual ~z_refcount() {}
	void ref_inc();
	int ref_dec();
};
inline z_refcount*  z_obj_ptr_copy(z_refcount* p) ;
inline void z_obj_ptr_delete(z_refcount* p) ;


#define ZPTR_COPY(_PTR_) z_obj_ptr_copy(_PTR_);
#define ZPTR_DEL(_PTR_) z_obj_ptr_delete(_PTR_);
#endif
#endif
