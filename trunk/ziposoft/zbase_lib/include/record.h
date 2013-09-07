#ifndef RECORD_H
#define RECORD_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/field.h"

/*
class zb_recp
{
	zb_recset* _recset;
	zb_key     _key;
public:
	zb_recp()
	{
		_key=-1;
		_recset=0;
	}
};

class zb_recp_sqlite3 : public zb_recp
{
public:
	zb_recp_sqlite3() :zb_recp()
	{
		_recset=0;

	}
	zb_src_set_sl3* _recset;
};
*/
class zb_record
{
public:
	zb_record()
	{

	}
};
class zb_record_sqlite3: public zb_record
{
public:
	zb_record_sqlite3() :zb_record()
	{

	}
};
class zb_src_set
{
public:
	zb_src_set()
	{
		_recset=0;

	}
	zb_recset* _recset;

	virtual zb_key_size get_num_records()=0; 
	virtual int get_num_cols()=0; 
	virtual void  dump(z_file* fp)=0; 
	virtual zb_status ptr_increment()=0; 
	virtual zb_status ds_create_desc_from_source(zb_desc* desc)=0;
	virtual zb_status ds_get_val_string(z_string& val,zb_field* field)=0;
};
class zb_src_set_vector : public zb_src_set
{
public:
	zb_src_set_vector()
	{
		_recset=0;

	}
	zb_recset* _recset;

	virtual zb_key_size get_num_records()=0; 
	virtual int get_num_cols()=0; 
	virtual zb_status ptr_increment()=0; 
};


class zb_recset
{
	zb_src_set* _ds_recset;
protected:
public:
	zb_desc _desc;
	zb_recset(zb_src_set* ds_recset)
	{
		_ds_recset=ds_recset;
	}
	zb_status create_desc_from_source();
	virtual zb_status ptr_increment(); 
	virtual zb_status get_val_string(z_string& val,zb_field* field);
	zb_field* get_field(ctext name);

};


class zb_src_set_sl3 : public zb_src_set
{
	sqlite3_stmt *_stmt;
	zb_src_sl3 *_file_sqlite;
public:
	zb_src_set_sl3() :zb_src_set()
	{
		_stmt=0;
		_file_sqlite=0;
	}
	zb_src_set_sl3(zb_src_sl3* ds) :zb_src_set()
	{
		_stmt=0;
		_file_sqlite=ds;
	}

	//-------------------------------
	//sqlite specific
	zb_status exec_sql(ctext sqltext);
	ctext get_column_name(int index);
	ctext ptr_get_column_text(int index);
	ctext ptr_get_column_type(int index);


	//-------------------------------
	//zb_src_set - implements
	virtual zb_key_size get_num_records(); 
	virtual int get_num_cols(); 
	virtual zb_status ptr_increment();
	virtual zb_status ds_create_desc_from_source(zb_desc* desc);
	virtual zb_status ds_get_val_string(z_string& val,zb_field* field);
	virtual void  dump(z_file* fp); 



};
#endif