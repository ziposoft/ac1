#ifndef TABLE_H
#define TABLE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/record.h"

class zb_ds_table  
{
protected:
	zb_source* _ds;
	z_string _id;
	zb_ds_desc _ds_desc;
public:
	ctext get_ds_id() { return   _id; }
	zb_ds_desc& get_desc() { return   _ds_desc; }
	ctext get_map_key();
	zb_ds_table(zb_source* ds,ctext unique_id);
	virtual z_status record_add(zb_record* rec);
	virtual z_status open();
	virtual size_t get_record_count();
	virtual z_status get_record_by_index(size_t index,zb_rec_ptr** cursor);
	virtual z_status delete_record_by_index(size_t index);


};

class zb_table_base : public z_refcount
{
protected:
	z_string _name;
	z_string _id;
	zb_desc _desc;
	zb_source* _ds;
	zb_ds_table* _ds_table;
	zb_key _key;
public:
	zb_table_base(zb_source* ds,zb_key key,ctext name);
	zb_table_base();
	z_status dump();
	virtual ctext get_map_key()
	{
		return _id;
	}
	zb_desc& get_desc()
	{
		return _desc;
	}
	virtual zb_key get_key()
	{
		return _id;
	}
	virtual ctext get_name()
	{
		return _name;
	}
	zb_record* new_default_rec();
	z_status get_default_rec(zb_record *rec);
	z_status record_add(zb_record *rec);

};

class zb_table_dynamic : public zb_table_base
{
protected:
public:
	
	zb_table_dynamic(zb_source* ds,ctext name) : zb_table_base()
	{
	}
	zb_table_dynamic() : zb_table_base()
	{
	}
};
class zb_table_static : public zb_table_base
{
protected:
public:
	zb_table_static(zb_source* ds,zb_key key,
		ctext name) : zb_table_base(ds,key,name)
	{
	}
	zb_table_static() : zb_table_base()
	{
	}
};

#endif
