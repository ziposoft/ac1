#ifndef TABLE_H
#define TABLE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/record.h"

class zb_ds_table  
{
public:
	z_string _id;
	ctext get_map_key();
	zb_ds_table(ctext unique_id);
	virtual z_status record_add(zb_record* rec);

	zb_ds_desc
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
	virtual z_status load_from_ds();

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
