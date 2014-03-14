#ifndef TABLE_H
#define TABLE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/record.h"


class zb_table_base : public z_obj
{
protected:
	z_string _name;
	zb_desc _desc;
	zb_source* _ds;
	zb_ds_table* _ds_table;
	zb_key _key;
public:
	zb_table_base(zb_source* ds,zb_key key,ctext name)
	{
		_ds=ds;
		_name=name;
		_desc , &g_field_key;
	}
	zb_table_base()
	{
		_ds=0;
		_name="unknown";
		_key=-1;
	}
	zb_status dump();
	virtual ctext get_name()
	{
		return _name;
	}
	zb_desc& get_desc()
	{
		return _desc;
	}
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
