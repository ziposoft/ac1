#include "zb_pch.h"


#include "zbase_lib/include/table_static.h"

zb_table_base::zb_table_base(zb_source* ds,zb_key key,ctext name)
{
	_ds=ds;
	_name=name;
	_id=key;
	_desc , &g_field_key;
}




zb_status zb_table_base::dump()
{


	return zb_ok;

}