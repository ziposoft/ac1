#ifdef ZB_SQLITE
#ifndef DSSQL_H
#define DSSQL_H

#include "zbase_lib/include/datasource.h"



class zb_record_sqlite3: public zb_record
{
public:
	zb_record_sqlite3() :zb_record()
	{

	}
};

class zb_ds_table_sl3 : public zb_ds_table
{
	sqlite3_stmt *_stmt;
	zb_src_sl3 *_file_sqlite;
public:

	zb_ds_table_sl3(zb_src_sl3* ds,ctext unique_id) ;

	//-------------------------------
	//sqlite specific
	z_status exec_sql(ctext sqltext);
	ctext get_column_name(int index);
	ctext ptr_get_column_text(int index);
	ctext ptr_get_column_type(int index);


	//-------------------------------
	//zb_ds_recptrset - implements
	virtual zb_key_size get_num_records(); 
	virtual int get_num_cols(); 
	virtual z_status ptr_increment();
	virtual z_status ds_create_desc_from_source(zb_desc* desc);
	virtual z_status ds_get_val_string(z_string& val,zb_field* field);
	virtual void  dump(z_file* fp); 

};

z_status get_zb_status_sqlite(int sql_status);

class zb_src_sl3 : public zb_source
{
	sqlite3 *_handle;
protected:
	z_status _read_stucture();
public:
	z_string _path;
	z_string _param_table_name;


	int _last_error;
	zb_src_sl3();
	virtual z_status open(ctext name);
	virtual z_status close();
	z_status dump_master();
	z_status get_table_info();
	virtual bool is_open();

	virtual z_status get_table_desc(ctext ds_table_name,zb_desc& desc);


	virtual zb_st_master* get_tbl_master();
	virtual zb_ds_table* get_tbl(ctext ds_table_name);
	virtual z_status get_tables();


	//sqlite specific
	sqlite3* get_handle();

	z_status _get_sql_recset_sql(zb_ds_table_sl3*& recset,ctext sqltext);
	z_status _get_sql_recset(zb_ds_table_sl3*& recset,ctext tbl_name,ctext where_clause);

	ctext get_last_error_msg();



};

#endif
#endif
