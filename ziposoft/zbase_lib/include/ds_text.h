#ifndef DSTEXT_H
#define DSTEXT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"

class zb_ds_text;
class zb_rec_ptr_txt;

class zb_ds_table_txt  : public zb_ds_table
{
	z_file _file;
public:
	zb_ds_table_txt(zb_ds_text* ds,ctext unique_id);
	zb_ds_text* _ds;

	virtual z_status record_add(zb_ds_rec_ptr* rec);
	virtual z_status open(bool writable);
	virtual size_t get_record_count();
	virtual z_status get_record_by_index(size_t index,zb_ds_rec_ptr** cursor);
};

class zb_ds_text: public zb_source
{
	z_directory _dir;
	z_string _name;
	z_map<zb_ds_table_txt> _tables;

public:
	zb_ds_text(ctext name);
	virtual ~zb_ds_text();
	virtual z_status open(bool create,bool writable);
	virtual z_status close();
	//virtual zb_record* get_solo_record(zb_table_base* tbl)=0;
};
class zb_rec_ptr_txt  : public zb_ds_rec_ptr
{
	zb_ds_table_txt* _set;
	size_t _index;


public:
	zb_rec_ptr_txt(bool solo);
 	virtual ~zb_rec_ptr_txt();
	virtual void set(zb_ds_table* rs,size_t index);

};


#endif
