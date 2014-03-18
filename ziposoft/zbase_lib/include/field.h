#ifndef FIELD_H
#define FIELD_H
#include "zbase_lib/include/zb.h"


class zb_field //:  public z_obj
{
	z_string _name;
	zb_key   _key;
	int _index; //TODO-ugly, make private or something
public:
	
	zb_field(zb_key key,ctext name);
	//virtual zb_status get_data_text(zb_recp* rec,ctext& text)=0;
	virtual zb_status get_data_text(zb_recset* rs,z_string& text)=0;
	virtual ctext get_key();
	const z_string& get_name_str() { return _name; }
	virtual int get_index() { return _index; }
	virtual void set_index(int i) {  _index=i; }
	virtual ctext  get_type_name() =0;

};

class zb_field_int32 : public zb_field
{
public:
	zb_field_int32(zb_key key,ctext name);
	virtual zb_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "int32"; }
};
class zb_field_key : public zb_field_int32
{
public:
	zb_field_key();
	virtual ctext  get_type_name() { return "key"; }
};
const zb_field_key g_field_key;
class zb_field_int64: public zb_field
{
public:
	zb_field_int64(zb_key key,ctext name);
	virtual zb_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "int64"; }
};

class zb_field_string: public zb_field
{
public:
	zb_field_string(zb_key key,ctext name);
	virtual zb_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "string"; }
};


class zb_desc : public z_map<zb_field>,public z_refcount
{
public:
	zb_desc(){}
	virtual ~zb_desc(){}
	zb_field* get_field(ctext name);
	
};
#endif
