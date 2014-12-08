#ifndef ZB_H
#define ZB_H
#include "zbase_lib/include/zb_pch.h"

#define Z_PROFILING


#define ZB_INCLUDE_DS_TEXT
//#define ZB_INCLUDE_DS_METAKIT
//#define ZB_INCLUDE_DS_SQLITE
#define ZB_INCLUDE_DS_HAMSTER



typedef U32 zb_key_size;
typedef zb_key_size zb_key;


class zb_ds_rec_ptr;
class zb_ds_recptrset;

class zb_datum;
class zb_record;
class zb_recset;
class zb_rec_ptr;

class zb_table_dynamic;
class zb_table_static;
class zb_table_base;

class zb_ds_field;


class zb_zipobase;
class zb_source;
class zb_ds_sl3;
class zb_ds_table_sl3;


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
