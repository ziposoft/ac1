#include "zb_pch.h"


#include "zbase_lib/include/datasource.h"
#include "zbase_lib/include/table_static.h"



zb_table_base::zb_table_base(zb_source* ds,zb_key key,ctext name)
{
	_ds=ds;
	_name=name;
	_id="T";
	_id+=key;
	_ds_table=0;
	
}
zb_table_base::zb_table_base()
{
	_ds=0;
	_name="unknown";
	_key=-1;
	_ds_table=0;
	
}
/*
z_status zb_table_base::load_from_ds()
{
	_ds_table=_ds->get_tbl(_id,get_desc());
	if(_ds_table)
		return zb_status_ok;

	return ZB_ERROR(zb_status_ds_data_error);

}
*/

z_status zb_table_base::dump()
{

	 return ZB_ERROR(zb_status_not_implemented);

}
zb_record* zb_table_base::new_default_rec()
{
	//ZB_ERROR(zb_status_not_implemented);
	zb_record* rec=0;
//	rec=_ds->record_solo_new();

	return rec;

}
z_status zb_table_base::record_add(zb_record *rec)
{

	return ZB_ERROR(zb_status_not_implemented);

}
