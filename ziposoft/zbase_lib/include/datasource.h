#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/table_static.h"
#include "zipolib_cpp/include/z_stl_map.h"


class zb_ds_table  
{
public:

};
class zb_ds_field  
{
public:

};
class zb_ds_record  
{
public:

};
class zb_source 
{
public:
	zb_source();
	z_string _name;
	virtual ~zb_source(){};
	virtual zb_status open(){ return zb_not_implemented;};
	virtual zb_status get_tables(){ return zb_not_implemented;};
	virtual zb_status close(){ return zb_not_implemented;};
	virtual zb_status get_table_desc(ctext ds_table_name,zb_desc& desc){ return zb_not_implemented;};
	virtual bool is_open(){ return false;};
	//virtual int get_record()=0;
	virtual zb_st_master* get_tbl_master(){ return 0;};
	virtual zb_ds_table* get_tbl(ctext ds_table_name){ return 0;};


	z_map<zb_table> _tables;

};

#endif
