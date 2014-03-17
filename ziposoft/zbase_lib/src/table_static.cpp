#include "zb_pch.h"


#include "zbase_lib/include/table_static.h"

zb_st_master::zb_st_master(zb_source* ds):zb_table_static(ds,zk_st_master,"master")
{
	_desc 
		
		,new zb_field_string(zk_sf_1_ds_tbl_name,"ds_name")
		,new zb_field_string(zk_sf_1_tbl_name,"name")
		,new zb_field_string(zk_sf_1_label,"label")
		,new zb_field_string(zk_sf_1_desc,"desc")

		;

}
zb_st_test::zb_st_test(zb_source* ds):zb_table_static(ds,zk_st_test,"test")
{
	_desc 
		
		,_f_name=new zb_field_string(zk_sf_test_str,"name")


		;

}
