#ifndef RECORD_H
#define RECORD_H


#include "zbase_lib/include/field_static.h"

class zb_rec_ptr
{
protected:
	zb_recset* _p_recset;
	zb_rec_ptr();
	virtual ~zb_rec_ptr(){};
public:
};
class zb_record : public zb_rec_ptr
{
protected:

	zb_desc* _p_desc;
public:
	//zb_record();
	zb_record();
	virtual ~zb_record(){};
	zb_record(zb_desc* desc);
	zb_record(zb_recset* recset);

	

};


class zb_recset
{
	zb_ds_recordset* _ds_recset;
protected:
public:
	zb_desc _desc;
	zb_recset(zb_ds_recordset* ds_recset)
	{
		_ds_recset=ds_recset;
	}
	z_status create_desc_from_source();
	virtual z_status ptr_increment(); 
	virtual z_status get_val_string(z_string& val,zb_field* field);
	zb_field* get_field(ctext name);
	zb_desc* get_desc();

};


class zb_ds_recordset
{
public:
	zb_ds_recordset()
	{
		_recset=0;

	}
	zb_recset* _recset;

	virtual zb_key_size get_num_records()=0; 
	virtual int get_num_cols()=0; 
	virtual void  dump(z_file* fp)=0; 
	virtual z_status ptr_increment()=0; 
	virtual z_status ds_create_desc_from_source(zb_desc* desc)=0;
	virtual z_status ds_get_val_string(z_string& val,zb_field* field)=0;
};
class zb_ds_recordset_vector : public zb_ds_recordset
{
public:
	zb_ds_recordset_vector()
	{
		_recset=0;

	}
	zb_recset* _recset;

	virtual zb_key_size get_num_records()=0; 
	virtual int get_num_cols()=0; 
	virtual z_status ptr_increment()=0; 
};




#endif