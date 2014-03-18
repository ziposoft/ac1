#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"
#include "zbase_lib/include/record.h"

ctext zb_ds_table::get_key()
{
	return _id;
}
zb_ds_table::zb_ds_table(ctext unique_id)
{
	_id=unique_id;
}



zb_source::zb_source()
{


}
