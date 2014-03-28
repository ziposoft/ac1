#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"
zb_field::zb_field(zb_key key,ctext name)
{
	_key=key;
	
	_name=name;
	_index=-1;
}
zb_ds_field * zb_field::get_ds_field()
{
	Z_ASSERT((_ds_field));
	return _ds_field;
}

ctext zb_field::get_key() 
{ 
	return _name; 
}

zb_field* zb_desc::get_field(ctext name)
{
    return get(name);
}


zb_field_int32::zb_field_int32(zb_key key,ctext name):zb_field(key,name)
{


}
z_status zb_field_int32::get_data_text(zb_recset* rec,z_string& text)
{
	
	return zb_status_ok;
}


zb_field_key::zb_field_key():zb_field_int32(zk_sf_1_key,"Key")
{

}

zb_field_string::zb_field_string(zb_source* ds,zb_key key,ctext name):zb_field(key,name)
{
	_id='s'+key;
	ds->get_ds_field_string(get_id());
}
z_status zb_field_string::get_data_text(zb_recset* rec,z_string& text)
{
	//ZTF;
	return rec->get_val_string(text,this);
}

z_status zb_desc::get_default_rec(zb_record *rec)
{
	z_map_iter i;
	zb_field* f=0;
	while(f=get_next(i))
	{
		f->set_default(rec);

	}

	return zb_status_ok;
}



