#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"
#include "zbase_lib/include/ds_hamster.h"
#include "zbase_lib/include/ds_text.h"

/*===========================================================================

	Functions
=============================================================================*/


z_status zb_datasource_new(type_ds_type type,ctext path,zb_source* &ds)
{
	ds=0;
	switch(type)
	{
		case type_ds_csv:
			ds=new zb_ds_text(path);
			break;
 		case type_ds_hamster:
			ds=new zb_ds_ham(path);
			break;
		default:
			return Z_ERROR_NOT_IMPLEMENTED;
			break;
	}
	return 	zs_ok;
}
z_status zb_datasource_open(bool create,type_ds_type type,ctext path,zb_source* &ds)
{
	z_status status=zb_datasource_new(type,path,ds);
	status= ds->open(false,true);
	if(	status)
		delete ds;
	return 	status;
}
z_status zb_datasource_create(type_ds_type type,ctext path,zb_source* &ds)
{
	z_status status=zb_datasource_new(type,path,ds);
	status= ds->open(true,true);
	if(	status)
		delete ds;
	return 	status;
}
#if 0
z_status zb_datasource_delete(type_ds_type type,ctext path)
{
	z_status status=zb_datasource_new(type,path,ds);
	status= ds->delete_datasource();
	if(	status)
		delete ds;
	return 	status;
}
#endif 