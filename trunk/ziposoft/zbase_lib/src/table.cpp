#include "zbase_lib/include/table_static.h"



zb_st_master::zb_st_master(zb_source* ds):zb_table_static(ds,"master")
{
	_desc 
		,new zb_field_key(zk_sf_1_key)
		,new zb_field_string(zk_sf_1_ds_tbl_name,"ds_name")
		,new zb_field_string(zk_sf_1_tbl_name,"name")
		,new zb_field_string(zk_sf_1_label,"label")
		,new zb_field_string(zk_sf_1_desc,"desc")

		;

}


zb_status zb_table_base::dump()
{


	return zb_ok;

}