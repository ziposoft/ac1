#include "zb_pch.h"

#include "zbase_lib/include/record.h"
#include "zbase_lib/include/datasource.h"
 zb_record::zb_record()
{
	_p_desc=0;
	_p_recset=0;
}
zb_record::zb_record(zb_desc* desc)
{
	_p_desc=desc;
	_p_recset=0;
}
zb_record::zb_record(zb_recset* recset)
{
	_p_desc=recset->get_desc();
	_p_recset=recset;
}




z_status zb_recset::create_desc_from_source()
{
	return _ds_recset->ds_create_desc_from_source(&_desc);

}
zb_field* zb_recset::get_field(ctext name)
{
	//if(!_pdesc)  		return 0;
	return _desc.get_field( name);
}


z_status zb_recset::get_val_string(z_string& val,zb_field* field)
{
	return _ds_recset->ds_get_val_string(val,field);
}

z_status zb_recset::ptr_increment()
{
	return _ds_recset->ptr_increment();
}



