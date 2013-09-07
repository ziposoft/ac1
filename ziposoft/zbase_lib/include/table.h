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
public:
	ZO_OBJ_H;
	zb_table_base(zb_source* ds,ctext name)
	{
		_ds=ds;
		_name=name;
	}
	zb_table_base()
	{
		_ds=0;
		_name="unknown";
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
class zb_table : public zb_table_base
{
protected:
public:
	ZO_OBJ_H;
	zb_table(zb_source* ds,ctext name) : zb_table_base(ds,name)
	{
	}
	zb_table() : zb_table_base()
	{
	}
};
class zb_table_static : public zb_table_base
{
protected:
public:
	ZO_OBJ_H;
	zb_table_static(zb_source* ds,ctext name) : zb_table_base(ds,name)
	{
	}
	zb_table_static() : zb_table_base()
	{
	}
};

#endif
