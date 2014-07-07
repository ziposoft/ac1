#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"


/*__________________________________________________________________________

	zb_ds_field
____________________________________________________________________________*/
z_status zb_ds_field::set_string(zb_ds_rec_ptr *rec,ctext s){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::get_string(zb_ds_rec_ptr *rec,z_string& s){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::set_int32(zb_ds_rec_ptr *rec,I32 i){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::get_int32(zb_ds_rec_ptr *rec,I32& i){ return Z_ERROR_NOT_IMPLEMENTED;}

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
	  Z_ERROR_NOT_IMPLEMENTED;

}

/*__________________________________________________________________________

	zb_ds_table
____________________________________________________________________________*/
z_status zb_ds_table::record_add(zb_ds_rec_ptr* rec)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
z_status zb_ds_table::open(bool writable)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
size_t zb_ds_table::get_record_count()
{
	Z_ERROR_NOT_IMPLEMENTED;
	 return 0;
}
z_status zb_ds_table::get_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
z_status zb_ds_table::test_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
z_status zb_ds_table::delete_record_by_index(size_t index)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
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
zb_ds_table::zb_ds_table()
{
	_ds=0;
	Z_ASSERT(0);
}
/*__________________________________________________________________________

	zb_source
____________________________________________________________________________*/

zb_source::zb_source()
{
	_name="";
	_status=status_closed;
}
bool zb_source::is_open()
{ 

	Z_TODO;
	return false;
}
