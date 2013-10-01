#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/table_static.h"
#include "zipolib_cpp/include/z_stl_map.h"



class zb_source //: public z_obj
{
public:
	zb_source();
	z_string _name;
	virtual ~zb_source(){};
	virtual zb_status open()=0;
	virtual zb_status get_tables()=0;
	virtual zb_status close()=0;
	virtual zb_status get_table_desc(ctext ds_table_name,zb_desc& desc)=0;
	virtual bool is_open()=0;
	//virtual int get_record()=0;
	virtual zb_st_master* get_tbl_master()=0;
	virtual zb_table* get_tbl(ctext ds_table_name)=0;


	z_map<zb_table> _tables;

};
class zb_ds_text
{
public:
	zb_ds_text();
	virtual ~zb_ds_text();
	virtual zb_status open(ctext name);
	virtual zb_status close();
	virtual bool is_open();
};
#ifdef ZB_SQLITE

zb_status get_zb_status_sqlite(int sql_status);

class zb_src_sl3 : public zb_source
{
	sqlite3 *_handle;
protected:
	zb_status _read_stucture();
public:
	ZO_OBJ_H;
	z_string _path;
	z_string _param_table_name;


	zb_src_sl3();
	int _last_error;
	zb_src_sl3(ctext  path) : zb_source()
	{
		_path=path;
		_last_error=SQLITE_OK;
		_handle=0;
	}
	virtual zb_status open();
	virtual zb_status close();
	zb_status dump_master();
	zb_status get_table_info();
	virtual bool is_open();

	virtual zb_status get_table_desc(ctext ds_table_name,zb_desc& desc);


	virtual zb_st_master* get_tbl_master();
	virtual zb_table* get_tbl(ctext ds_table_name);
	virtual zb_status get_tables();


	//sqlite specific
	sqlite3* get_handle();

	zb_status _get_sql_recset_sql(zb_src_set_sl3*& recset,ctext sqltext);
	zb_status _get_sql_recset(zb_src_set_sl3*& recset,ctext tbl_name,ctext where_clause);

	ctext get_last_error_msg();



};
#endif

#endif
