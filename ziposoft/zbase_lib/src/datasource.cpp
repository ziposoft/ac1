#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"


/*__________________________________________________________________________

	zb_ds_field
____________________________________________________________________________*/
z_status zb_ds_field::set_string(zb_ds_rec_ptr *rec,ctext s){ return ZB_ERROR(zb_status_not_implemented);}
z_status zb_ds_field::get_string(zb_ds_rec_ptr *rec,z_string& s){ return ZB_ERROR(zb_status_not_implemented);}
z_status zb_ds_field::set_int32(zb_ds_rec_ptr *rec,I32 i){ return ZB_ERROR(zb_status_not_implemented);}
z_status zb_ds_field::get_int32(zb_ds_rec_ptr *rec,I32& i){ return ZB_ERROR(zb_status_not_implemented);}

/*____________________________________________________________________________
	zb_ds_rec_ptr
____________________________________________________________________________*/


/*____________________________________________________________________________
	zb_ds_rec_ptr
____________________________________________________________________________*/
 zb_ds_rec_ptr::zb_ds_rec_ptr()
{


}
void zb_ds_rec_ptr::set(zb_ds_table* rs,size_t index)
{
	  ZB_ERROR(zb_status_not_implemented);

}

/*__________________________________________________________________________

	zb_ds_table
____________________________________________________________________________*/
z_status zb_ds_table::record_add(zb_ds_rec_ptr* rec)
{
	 return ZB_ERROR(zb_status_not_implemented);
}
z_status zb_ds_table::open(bool writable)
{
	 return ZB_ERROR(zb_status_not_implemented);
}
size_t zb_ds_table::get_record_count()
{
	ZB_ERROR(zb_status_not_implemented);
	 return 0;
}
z_status zb_ds_table::get_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	 return ZB_ERROR(zb_status_not_implemented);
}
z_status zb_ds_table::test_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	 return ZB_ERROR(zb_status_not_implemented);
}
z_status zb_ds_table::delete_record_by_index(size_t index)
{
	 return ZB_ERROR(zb_status_not_implemented);
}
ctext zb_ds_table::get_map_key()
{
	return _id;
}
zb_ds_table::zb_ds_table(zb_source* ds,ctext unique_id)
{
	_ds=ds;
	_id=unique_id;
}

/*__________________________________________________________________________

	zb_source
____________________________________________________________________________*/

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
