#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/table_static.h"


z_status zb_validate_identifier(ctext text);


 enum type_ds_type {
	type_ds_invalid,
	type_ds_csv,
	type_ds_hamster,
	type_ds_metakit,
	type_ds_sqlite3,
	type_ds_sql,
};

class zb_ds_rec_ptr;
enum type_ds_field {
	type_ds_field_invalid,
	type_ds_field_string,
	type_ds_field_int,
	type_ds_field_max,
};
class zb_ds_field  
{
protected:
	type_ds_field _type;
public:
	zb_ds_field(ctext id);
	z_string _id;
	int _index;
	virtual void index_set(int i) {  _index=i; };
	virtual int index_get(){  return _index; };

	virtual ctext get_map_key() { return _id;};

	virtual z_status set_string(zb_ds_rec_ptr *rec,ctext s);
	virtual z_status get_string(zb_ds_rec_ptr *rec,z_string& s);
	virtual z_status set_int32(zb_ds_rec_ptr *rec,I32 i);
	virtual z_status get_int32(zb_ds_rec_ptr *rec,I32& i);

	virtual z_status get_name(z_string& s);

	//virtual z_status find_record_match(zb_ds_table* tbl,);

};
class zb_ds_desc : public z_obj_map<zb_ds_field>,public z_refcount
{
public:
	zb_ds_desc(){}
	virtual ~zb_ds_desc(){}
	zb_ds_field* get_ds_field(ctext name);
	z_status get_default_rec(zb_record *rec);
	virtual void init_child(zb_ds_field *fld)
	{
		int i=size()-1;
		fld->index_set(i);

	}

};
/*__________________________________________________________________________

zb_ds_table
____________________________________________________________________________*/
class zb_ds_table 
{
protected:
	enum status {
		status_error,
		status_closed,
		status_corrupt,
		status_cant_open,
		status_created,
		status_opened_read,
		status_opened_write,
		status_opened_need_commit
	} _status;
	zb_source* _ds;
public:
	z_string _id;
	zb_ds_desc _ds_desc;
	zb_ds_table();
	zb_ds_table(zb_source* ds,ctext unique_id);
	virtual  ~zb_ds_table(){};

	ctext get_ds_id() { return   _id; }
	zb_ds_desc& get_desc() { return   _ds_desc; }
	ctext get_map_key();
	virtual z_status record_add(zb_ds_rec_ptr* rec);
	virtual z_status open(bool writable);
	virtual size_t get_record_count();
	// z_status get_record_by_key(zb_key key,zb_ds_rec_ptr** cursor);
	virtual z_status get_record_by_index(size_t index,zb_ds_rec_ptr** cursor);
	virtual z_status delete_record_by_index(size_t index);

	/*______________________________________

	zb_ds_table Virtual Funcs
	______________________________________*/
	virtual z_status commit(){	return Z_ERROR_NOT_IMPLEMENTED;	}
	virtual z_status close(){	return Z_ERROR_NOT_IMPLEMENTED;	}
	virtual zb_ds_field* ds_field_string_new(ctext id){ Z_ERROR_NOT_IMPLEMENTED;return 0;};
	virtual zb_ds_field* ds_field_int32_new(ctext id){ Z_ERROR_NOT_IMPLEMENTED;return 0;};
	virtual zb_ds_rec_ptr* record_solo_new(){ Z_ERROR_NOT_IMPLEMENTED;return 0;};
	virtual z_status field_new(type_ds_field type,ctext id,zb_ds_field*& fld)	{ return Z_ERROR_NOT_IMPLEMENTED;};

	virtual z_status field_add(zb_ds_field* fld);

	/*
	virtual int get_num_cols()=0; 
	virtual void  dump(z_file* fp)=0; 
	virtual z_status ptr_increment()=0; 
	*/


	/*______________________________________

	zb_ds_table Interface
	______________________________________*/
	bool auto_open;
	z_string _param_field_name;
	z_strlist _param_new_data;
	int _param_record_index;
	int _param_new_field_type;
	virtual z_status act_dump_records();
	virtual z_status act_record_get();
	virtual z_status act_record_del();
	virtual z_status act_record_add();
	virtual z_status act_add_field();
	virtual z_status act_open()
	{
		return open(true);;
	}

};
/*__________________________________________________________________________

zb_ds_rec_ptr
____________________________________________________________________________*/
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


/*___________________________________________________________________________

						zb_source
___________________________________________________________________________*/
class zb_source 
{

protected:	
	virtual z_status _table_new(ctext ds_table_name,zb_ds_table*& tbl){ return Z_ERROR_NOT_IMPLEMENTED;};

public:
	z_string _name;
	z_string _fullpath;
	enum status {
		status_does_not_exist,
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




	z_obj_map<zb_ds_table> _ds_tables;
	ctext get_map_key() 
	{ 
		return _name; 
	}
	ctext get_full_path() 
	{ 
		return _fullpath; 
	}

 	/*_________________________
	 
	 Funcs
	 _________________________*/


	virtual z_status create_or_open();


 	/*_________________________
	Instrumentation
	_________________________*/
	z_string _param_db_name;
	z_string _param_table_new_name;
	z_status act_table_new();
	virtual z_status commit();
	virtual z_status act_open();
	virtual z_status act_delete_datasource();
	bool auto_open;

	/*_________________________
	Datasource Virtual Funcs
	_________________________*/
	virtual z_status check_exists(){ return Z_ERROR_NOT_IMPLEMENTED;};
	virtual z_status open(bool create,bool writable){ return Z_ERROR_NOT_IMPLEMENTED;};
	virtual z_status close();
	virtual z_status delete_datasource(){ return Z_ERROR_NOT_IMPLEMENTED;};


	/*_________________________
	????
	_________________________*/
	virtual z_status get_tables(){ return Z_ERROR_NOT_IMPLEMENTED;};
	virtual z_status get_table_list(z_obj_vector_map<zb_ds_table> & list){ return Z_ERROR_NOT_IMPLEMENTED;};
	virtual z_status get_table_desc(ctext ds_table_name,zb_desc& desc){ return Z_ERROR_NOT_IMPLEMENTED;};
	virtual bool is_open();
	virtual zb_st_master* get_tbl_master(){ Z_ERROR_NOT_IMPLEMENTED;return 0;};
	virtual zb_ds_table* get_tbl(ctext ds_table_name,zb_ds_field& desc){Z_ERROR_NOT_IMPLEMENTED; return 0;};

	virtual z_status ds_table_new(ctext ds_table_name,zb_ds_table*& tbl);
	virtual z_status ds_table_get(ctext ds_table_name,zb_ds_table*& tbl);


};

z_status zb_datasource_open(bool create,type_ds_type type,ctext path,zb_source* &ds);
z_status zb_datasource_create(type_ds_type type,ctext path,zb_source* &ds);
z_status zb_datasource_delete(type_ds_type type,ctext path);

#endif
