#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/table_static.h"





class zb_ds_record  
{
public:

};
class zb_ds_record_native  
{
	z_map<zb_datum> _data;
public:

};

class zb_source 
{

protected:	
	
	z_string _name;
public:
	enum status {
		status_corrupt,
		status_closed,
		status_cant_open,
		status_created,
		status_opened_read,
		status_opened_write,
		status_opened_need_commit
	} _status;

	zb_source(ctext name);
	virtual ~zb_source(){};
	virtual z_status commit(){ return ZB_ERROR(zb_status_not_implemented);};
	virtual z_status open(bool writable){ return ZB_ERROR(zb_status_not_implemented);};
	virtual z_status get_tables(){ return ZB_ERROR(zb_status_not_implemented);};
	virtual z_status close(){ return ZB_ERROR(zb_status_not_implemented);};
	virtual z_status get_table_desc(ctext ds_table_name,zb_desc& desc){ return ZB_ERROR(zb_status_not_implemented);};
	virtual bool is_open();
	//virtual int get_record()=0;
	virtual zb_st_master* get_tbl_master(){ return 0;};
	virtual zb_ds_table* get_tbl(ctext ds_table_name,zb_ds_field& desc){ return 0;};

	virtual zb_ds_table* ds_table_new(ctext ds_table_name){ return 0;};
 	virtual z_status     ds_table_open(zb_ds_table* tbl){ return 0;};

	virtual zb_ds_field* ds_field_string_new(ctext id){ return 0;};
	virtual zb_ds_field* ds_field_int32_new(ctext id){ return 0;};
	//virtual zb_record* get_solo_record(zb_table_base* tbl){ return 0;};
	virtual zb_record* record_solo_new(){ return 0;};

	z_map<zb_ds_table> _ds_tables;


};

#endif
