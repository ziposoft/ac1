#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"
#include "zbase_lib/include/ds_hamster.h"
#include "zbase_lib/include/ds_text.h"

/*===========================================================================

	Functions
=============================================================================*/

z_status zb_datasource_open(type_ds_type type,ctext path,zb_source* &ds)
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

	return zs_ok;

}
z_status zb_datasource_create(type_ds_type type,ctext path,zb_source* &ds)
{
	return Z_ERROR_NOT_IMPLEMENTED;
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
	return zs_ok;
}
z_status zb_datasource_delete(type_ds_type type,ctext path)
{
	return Z_ERROR_NOT_IMPLEMENTED;
	zb_source* ds=0;
	switch(type)
	{
		case type_ds_csv:
			break;
 		case type_ds_hamster:
			break;
		default:
			break;
	}
	return zs_ok;
}
