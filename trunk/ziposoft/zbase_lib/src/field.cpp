#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"





/*____________________________________________________________________________

	zb_field
____________________________________________________________________________*/
zb_field::zb_field(zb_key key,ctext name)
{
	_key=key;
	
	_name=name;
	_index=-1;
	_ds_field=0;
}
zb_ds_field * zb_field::get_ds_field()
{
	Z_ASSERT((_ds_field));
	return _ds_field;
}

ctext zb_field::get_map_key() 
{ 
	return _name; 
}
const z_string& zb_field::get_id()
{
	if(!_id)
	{
		_id=  get_id_prefix();
		_id+=_key;

	}
	return _id;

}
zb_field* zb_desc::get_field(ctext name)
{
    return get(name);
}
/*____________________________________________________________________________

	zb_field_int32
____________________________________________________________________________*/

zb_field_int32::zb_field_int32(zb_key key,ctext name):zb_field(key,name)
{


}
z_status zb_field_int32::get_data_text(zb_recset* rec,z_string& text)
{
	
	return zs_ok;
}
z_status zb_field_int32::set_default(zb_rec_ptr* rec)
{
	
	return Z_ERROR(zs_not_implemented);
}

 /*____________________________________________________________________________

	zb_field_key
____________________________________________________________________________*/


zb_field_key::zb_field_key():zb_field_int32(zk_sf_1_key,"Key")
{

}
z_status zb_field_key::set_default(zb_rec_ptr* rec)
{
	
	return Z_ERROR(zs_not_implemented);
}


 /*____________________________________________________________________________

	zb_field_string
____________________________________________________________________________*/

zb_field_string::zb_field_string(zb_source* ds,zb_key key,ctext name):zb_field(key,name)
{
	_ds_field=ds->ds_field_string_new(get_id());
}
z_status zb_field_string::get_data_text(zb_recset* rec,z_string& text)
{
	//ZTF;
	return rec->get_val_string(text,this);
}
z_status zb_field_string::set_default(zb_rec_ptr* rec)
{
	
	return Z_ERROR(zs_not_implemented);
}
z_status zb_field_string::set(zb_rec_ptr *rec,ctext s)
{
	//return get_ds_field()->set_string(rec,s);
	return Z_ERROR(zs_not_implemented);


}

 /*____________________________________________________________________________

	zb_desc
____________________________________________________________________________*/
z_status zb_desc::get_default_rec(zb_record *rec)
{
	z_map_iter i;
	zb_field* f=0;
	while(f=get_next(i))
	{
		f->set_default(rec);

	}

	return zs_ok;
}



