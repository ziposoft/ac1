#ifndef FIELD_H
#define FIELD_H
#include "zbase_lib/include/zb.h"
//#include "zbase_lib/include/datum.h"

class zb_ds_field  
{
public:
	virtual z_status set_string(zb_record *rec,ctext s){ return zb_status_not_implemented;};
	virtual z_status get_string(zb_record *rec,z_string& s){ return zb_status_not_implemented;};
	virtual z_status set_int32(zb_record *rec,I32 i){ return zb_status_not_implemented;};
	virtual z_status get_int32(zb_record *rec,I32& i){ return zb_status_not_implemented;};

};
class zb_field : public z_refcount
{
	zb_ds_field* _ds_field;
	z_string _name;
	zb_key   _key;
	int _index; //TODO-ugly, make private or something
protected:
	z_string _id;
public:
	
	zb_field(zb_key key,ctext name);
	//virtual z_status get_data_text(zb_recp* rec,ctext& text)=0;
	virtual z_status get_data_text(zb_recset* rs,z_string& text)=0;
	virtual ctext get_key();
	virtual const z_string& get_id();
	const z_string& get_name_str() { return _name; }
	virtual int get_index() { return _index; }
	virtual void set_index(int i) {  _index=i; }
	virtual ctext  get_type_name() =0;
	virtual z_status set_default(zb_record *rec);

	zb_ds_field * get_ds_field();

};
class zb_field_view
{
	zb_field* _feild;
public:




};


class zb_field_int32 : public zb_field
{
public:
	zb_field_int32(zb_key key,ctext name);
	virtual z_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "int32"; }
	virtual z_status set_default(zb_record *rec);
};

class zb_field_key : public zb_field_int32
{
public:
	zb_field_key();
	virtual ctext  get_type_name() { return "key"; }
	virtual z_status set_default(zb_record *rec);
};

class zb_field_int64: public zb_field
{
public:
	zb_field_int64(zb_key key,ctext name);
	virtual z_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "int64"; }
	virtual z_status set_default(zb_record *rec);
};


class zb_field_string: public zb_field
{
	z_string _default_value;

public:
	zb_field_string(zb_source* ds,zb_key key,ctext name);
	virtual z_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "string"; }
	virtual z_status set_default(zb_record *rec);
	virtual z_status set(zb_record *rec,ctext s);
	//virtual z_status
};


class zb_desc : public z_map<zb_field>,public z_refcount
{
public:
	zb_desc(){}
	virtual ~zb_desc(){}
	zb_field* get_field(ctext name);
	z_status get_default_rec(zb_record *rec);
	
};
#endif
