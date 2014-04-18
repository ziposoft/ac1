#include "zb_pch.h"

#include "zbase_lib/include/record.h"
#include "zbase_lib/include/datasource.h"


zb_rec_ptr::zb_rec_ptr()
{
}
/*____________________________________________________________________________
	zb_record
____________________________________________________________________________*/
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

/*____________________________________________________________________________
	zb_recset
____________________________________________________________________________*/

zb_desc* zb_recset::get_desc()
{
	return &_desc;
}


z_status zb_recset::create_desc_from_source()
{
	//return _ds_recset->ds_create_desc_from_source(&_desc);
	return ZB_ERROR(zb_status_not_implemented);

}
zb_field* zb_recset::get_field(ctext name)
{
	//if(!_pdesc)  		return 0;
	return _desc.get_field( name);
}


z_status zb_recset::get_val_string(z_string& val,zb_field* field)
{
	//return _ds_recset->ds_get_val_string(val,field);
	return ZB_ERROR(zb_status_not_implemented);
}

z_status zb_recset::ptr_increment()
{
	//return _ds_recset->ptr_increment();
	return ZB_ERROR(zb_status_not_implemented);
}



