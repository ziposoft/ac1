#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/table_static.h"

class zb_ds_recptr;

class zb_ds_field  
{
protected:
	z_string _id;

public:
	virtual ctext get_map_key() { return _id;};

	virtual z_status set_string(zb_ds_recptr *rec,ctext s);
	virtual z_status get_string(zb_ds_recptr *rec,z_string& s);
	virtual z_status set_int32(zb_ds_recptr *rec,I32 i);
	virtual z_status get_int32(zb_ds_recptr *rec,I32& i);

};
class zb_ds_desc : public z_map_obj<zb_ds_field>,public z_refcount
{
public:
	zb_ds_desc(){}
	virtual ~zb_ds_desc(){}
	zb_field* get_field(ctext name);
	z_status get_default_rec(zb_record *rec);
	
};
class zb_ds_table 
{
protected:
	zb_source* _ds;
	z_string _id;
	zb_ds_desc _ds_desc;
public:
	zb_ds_table(zb_source* ds,ctext unique_id);
	virtual  ~zb_ds_table(){};

	ctext get_ds_id() { return   _id; }
	zb_ds_desc& get_desc() { return   _ds_desc; }
	ctext get_map_key();
	virtual z_status record_add(zb_ds_record* rec);
	virtual z_status open();
	virtual size_t get_record_count();
	virtual z_status get_record_by_index(size_t index,zb_ds_recptr** cursor);
	virtual z_status delete_record_by_index(size_t index);
	/*
	virtual int get_num_cols()=0; 
	virtual void  dump(z_file* fp)=0; 
	virtual z_status ptr_increment()=0; 
	*/

};

class zb_ds_recptr 
{
public:
	zb_ds_recptr();
	virtual  ~zb_ds_recptr(){};
	zb_ds_table* _set;
	size_t _index;
};
class zb_ds_record : public zb_ds_recptr
{
protected:

public:
	//zb_record();
	zb_ds_record();
	virtual ~zb_ds_record(){};

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
	virtual zb_ds_record* record_solo_new(){ return 0;};

	z_map<zb_ds_table> _ds_tables;


};

#endif
