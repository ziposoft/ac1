#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/table_static.h"

class zb_ds_rec_ptr;

class zb_ds_field  
{
protected:
	z_string _id;
	int _index;

public:
	virtual void index_set(int i) {  _index=i; };
	virtual int index_get(){  return _index; };

	virtual ctext get_map_key() { return _id;};

	virtual z_status set_string(zb_ds_rec_ptr *rec,ctext s);
	virtual z_status get_string(zb_ds_rec_ptr *rec,z_string& s);
	virtual z_status set_int32(zb_ds_rec_ptr *rec,I32 i);
	virtual z_status get_int32(zb_ds_rec_ptr *rec,I32& i);

};
class zb_ds_desc : public z_map_obj<zb_ds_field>,public z_refcount
{
public:
	zb_ds_desc(){}
	virtual ~zb_ds_desc(){}
	zb_field* get_field(ctext name);
	z_status get_default_rec(zb_record *rec);
	virtual void init_child(zb_ds_field *fld)
	{
		int i=size()-1;
		fld->index_set(i);

	}
	
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
	virtual z_status record_add(zb_ds_rec_ptr* rec);
	virtual z_status open(bool writable);
	virtual size_t get_record_count();
	virtual z_status test_record_by_index(size_t index,zb_ds_rec_ptr** cursor);
	virtual z_status get_record_by_index(size_t index,zb_ds_rec_ptr** cursor);
	virtual z_status delete_record_by_index(size_t index);


	/*
	virtual int get_num_cols()=0; 
	virtual void  dump(z_file* fp)=0; 
	virtual z_status ptr_increment()=0; 
	*/

};

class zb_ds_rec_ptr 
{
public:
	virtual void set(zb_ds_table* rs,size_t index);
	zb_ds_rec_ptr();
	virtual  ~zb_ds_rec_ptr(){};

};
/*
class zb_ds_rec_ptr : public zb_ds_rec_ptr
{
protected:

public:
	//zb_record();
	zb_ds_rec_ptr();
	virtual ~zb_ds_rec_ptr(){};

};
*/

class zb_ds_recptr_native  
{
	z_map<zb_datum> _data;
public:

};

class zb_source 
{

protected:	
	
public:
	z_string _name;
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
	virtual z_status commit(){ return Z_ERROR(zs_not_implemented);};
	virtual z_status open(bool create,bool writable){ return Z_ERROR(zs_not_implemented);};
	virtual z_status close(){ return Z_ERROR(zs_not_implemented);};
	virtual z_status get_table_desc(ctext ds_table_name,zb_desc& desc){ return Z_ERROR(zs_not_implemented);};
	virtual bool is_open();
	virtual zb_st_master* get_tbl_master(){ Z_ERROR(zs_not_implemented);return 0;};
	virtual zb_ds_table* get_tbl(ctext ds_table_name,zb_ds_field& desc){Z_ERROR(zs_not_implemented); return 0;};

	virtual zb_ds_table* ds_table_new(ctext ds_table_name){Z_ERROR(zs_not_implemented); return 0;};
 	virtual z_status     ds_table_open(zb_ds_table* tbl){ return Z_ERROR(zs_not_implemented);};

	virtual zb_ds_field* ds_field_string_new(ctext id){ Z_ERROR(zs_not_implemented);return 0;};
	virtual zb_ds_field* ds_field_int32_new(ctext id){ Z_ERROR(zs_not_implemented);return 0;};
	virtual zb_ds_rec_ptr* record_solo_new(){ Z_ERROR(zs_not_implemented);return 0;};

	z_map_obj<zb_ds_table> _ds_tables;


	//crap
	virtual z_status get_tables(){ return Z_ERROR(zs_not_implemented);};

};

#endif
