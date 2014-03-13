#ifdef ZB_SQLITE
#ifndef DSSQL_H
#define DSSQL_H

#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"





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
