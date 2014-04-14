#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"
#include "zbase_lib/include/record.h"

ctext zb_ds_table::get_map_key()
{
	return _id;
}
zb_ds_table::zb_ds_table(zb_source* ds,ctext unique_id)
{
	_ds=ds;
	_id=unique_id;

}



zb_source::zb_source(ctext name)
{
	_name=name;
	_status=status_closed;
}
bool zb_source::is_open()
{ 

	Z_TODO;
	return false;
}
