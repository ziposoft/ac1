#ifndef DSTEXT_H
#define DSTEXT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"

class zb_ds_text;
class zb_rec_ptr_txt;
class zb_ds_field_text_string;
					 
class zb_ds_table_txt  : public zb_ds_table	  ,public z_parse_csv
{
	z_file _file;
	std::vector<zb_rec_ptr_txt*>   _data;
	zb_rec_ptr_txt* _current_row;
	int _current_column;
	bool _dirty;
public:
	zb_ds_table_txt(zb_ds_text* ds,ctext unique_id);
	zb_ds_text* _ds;

	virtual z_status record_add(zb_ds_rec_ptr* rec);
	virtual z_status open(bool writable);
	virtual z_status commit();
	virtual size_t get_record_count();
	virtual z_status get_record_by_index(size_t index,zb_ds_rec_ptr** cursor);



	virtual bool EndRowCallback();
	virtual bool NewRowCallback();
	virtual bool NewValueCallback(const z_string & value);
};

class zb_ds_text: public zb_source
{
	z_directory _dir;
	z_map_obj<zb_ds_table_txt> _tables;

public:
	zb_ds_text();
	virtual ~zb_ds_text();
	virtual z_status open(ctext name,bool create,bool writable);
	virtual z_status close();
	virtual z_status commit();
 	virtual zb_ds_table* ds_table_new(ctext ds_table_name);
	virtual zb_ds_rec_ptr* record_solo_new();
	virtual zb_ds_field* ds_field_string_new(ctext id);
};

class zb_rec_solo : public z_strmap 
{
public:
 	zb_rec_solo();
	virtual ~zb_rec_solo();

};



class zb_rec_ptr_txt  : public zb_ds_rec_ptr ,public z_strlist
{
	zb_ds_table_txt* _set;
	size_t _index;


public:
	zb_rec_ptr_txt(bool solo=false);
 	virtual ~zb_rec_ptr_txt();
	virtual void set(zb_ds_table* rs,size_t index);

	ctext get_string(size_t index);
	void set_string(size_t index,ctext str);
	void output_csv(z_file& out);

};
class zb_ds_field_text  : public zb_ds_field
{
protected:
public:
	zb_ds_field_text(ctext id);
	virtual ~zb_ds_field_text();



};
class zb_ds_field_text_string  : public zb_ds_field_text
{

public:
	zb_ds_field_text_string(ctext id);
	virtual ~zb_ds_field_text_string();
	//For creating temp row, for filtering
	

// Overrides 

 	virtual z_status set_string(zb_ds_rec_ptr *rec,ctext s);
	virtual z_status get_string(zb_ds_rec_ptr *rec,z_string& s);
};

#endif
