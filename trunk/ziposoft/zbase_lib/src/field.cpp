#include "zbase_lib/include/field.h"
#include "zbase_lib/include/record.h"
zb_field::zb_field(zb_key key,ctext name)
{
	_key=key;
	_name=name;
	_index=-1;
}

ctext zb_field::get_name() 
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
zb_status zb_field_int32::get_data_text(zb_recset* rec,z_string& text)
{
	
	return zb_ok;
}


zb_field_key::zb_field_key(zb_key key):zb_field_int32(key,"Key")
{


}

zb_field_string::zb_field_string(zb_key key,ctext name):zb_field(key,name)
{


}
zb_status zb_field_string::get_data_text(zb_recset* rec,z_string& text)
{
	//ZTF;
	return rec->get_val_string(text,this);
}



