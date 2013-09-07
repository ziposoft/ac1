#ifndef FIELD_H
#define FIELD_H
#include "zbase_lib/include/zb.h"


class zb_field :  public z_obj
{
	z_string _name;
	zb_key   _key;
	int _index; //TODO-ugly, make private or something
public:
	ZO_OBJ_H;
	zb_field(zb_key key,ctext name);
	//virtual zb_status get_data_text(zb_recp* rec,ctext& text)=0;
	virtual zb_status get_data_text(zb_recset* rs,z_string& text)=0;
	virtual ctext get_name();
	const z_string& get_name_str() { return _name; }
	virtual int get_index() { return _index; }
	virtual void set_index(int i) {  _index=i; }
};

class zb_field_int32 : public zb_field
{
public:
	zb_field_int32(zb_key key,ctext name);
	virtual zb_status get_data_text(zb_recset* rec,z_string& text);
};
class zb_field_key : public zb_field_int32
{
public:
	zb_field_key(zb_key key);
};
class zb_field_int64: public zb_field
{
public:
	zb_field_int64(zb_key key,ctext name);
	virtual zb_status get_data_text(zb_recset* rec,z_string& text);
};

class zb_field_string: public zb_field
{
public:
	zb_field_string(zb_key key,ctext name);
	virtual zb_status get_data_text(zb_recset* rec,z_string& text);
};


class zb_desc : public z_obj_map<zb_field>
{
public:
	zb_desc(){}
	virtual ~zb_desc(){}
	zb_field* get_field(ctext name);
	
};
#endif
