#ifndef FIELD_H
#define FIELD_H
#include "zbase_lib/include/zb.h"
//#include "zbase_lib/include/datum.h"


class zb_field : public z_refcount
{
	z_string _name;
	zb_key   _key;
	z_string _id;

	int _index; //TODO-ugly, make private or something
protected:
	zb_ds_field* _ds_field;	 // actually we could have more than one! 
public:
	
	zb_field(zb_key key,ctext name);
	//virtual z_status get_data_text(zb_recp* rec,ctext& text)=0;
	virtual z_status get_data_text(zb_recset* rs,z_string& text)=0;
	virtual ctext get_map_key();
	virtual const z_string& get_id();
	const z_string& get_name_str() { return _name; }
	virtual int get_index() { return _index; }
	virtual void set_index(int i) {  _index=i; }
	virtual ctext  get_type_name() =0;
	virtual z_status set_default(zb_rec_ptr *rec)=0;

	virtual ctext get_id_prefix()=0;

	zb_ds_field * get_ds_field();

};
class zb_field_view
{
	zb_field* _feild;
public:




};


class zb_field_int32 : public zb_field
{
	int _default;
public:
	zb_field_int32(zb_key key,ctext name);
	virtual z_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "int32"; }
	virtual z_status set_default(zb_rec_ptr *rec);
	virtual ctext get_id_prefix() { return "I"; }

};

class zb_field_key : public zb_field_int32
{
public:
	zb_field_key();
	virtual ctext  get_type_name() { return "key"; }
	virtual z_status set_default(zb_rec_ptr *rec);
	virtual ctext get_id_prefix() { return "K"; }
};

class zb_field_int64: public zb_field
{
public:
	zb_field_int64(zb_key key,ctext name);
	virtual z_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "int64"; }
	virtual z_status set_default(zb_rec_ptr *rec);
	virtual ctext get_id_prefix() { return "I64_"; }
};


class zb_field_string: public zb_field
{
	z_string _default_value;

public:
	zb_field_string(zb_source* ds,zb_key key,ctext name);
	virtual z_status get_data_text(zb_recset* rec,z_string& text);
	virtual ctext  get_type_name() { return "string"; }
	virtual z_status set_default(zb_rec_ptr *rec);
	virtual z_status set(zb_rec_ptr *rec,ctext s);
	virtual ctext get_id_prefix() { return "S"; }
	//virtual z_status
};


class zb_desc : public z_map_obj<zb_field>,public z_refcount
{
public:
	zb_desc(){}
	virtual ~zb_desc(){}
	zb_field* get_field(ctext name);
	z_status get_default_rec(zb_record *rec);
	
};
#endif
