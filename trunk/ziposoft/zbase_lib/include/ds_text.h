#ifndef DSTEXT_H
#define DSTEXT_H
#include "zbase_lib/include/zb.h"

#ifdef ZB_INCLUDE_DS_TEXT
#include "zbase_lib/include/datasource.h"

class zb_ds_text;
class zb_rec_ptr_txt;
class zb_ds_field_text_string;
					 
/*___________________________________________________________________________

						zb_ds_table_txt
___________________________________________________________________________*/
class zb_ds_table_txt  : public zb_ds_table	  ,public z_parse_csv
{
	z_file _file;
	z_string _file_name;
	std::vector<zb_rec_ptr_txt*>   _data;
	zb_rec_ptr_txt* _current_row;
	int _current_column;
	bool _dirty;
protected:
	virtual bool EndRowCallback();
	virtual bool NewRowCallback();
	virtual bool NewValueCallback(const z_string & value);
	zb_ds_text* _ds;

public:
	zb_ds_table_txt();
	zb_ds_table_txt(zb_ds_text* ds,ctext unique_id);



	ctext get_file_name();
	//Interface

	 /*_________________________
	 zb_ds_table Virtual Funcs
	 _________________________*/
	virtual z_status record_add(zb_ds_rec_ptr* rec);
	virtual z_status open(bool writable);

	virtual z_status commit();
	virtual z_status get_record_by_index(size_t index,zb_ds_rec_ptr** cursor);
	virtual z_status delete_record_by_index(size_t index);
	virtual size_t get_record_count();
	virtual z_status field_new(type_ds_field type,ctext id,zb_ds_field*& fld);
	virtual zb_ds_field* field_string_new(ctext id);
	virtual zb_ds_rec_ptr* record_solo_new();
	virtual z_status close();




};
/*___________________________________________________________________________

						zb_ds_text
___________________________________________________________________________*/
class zb_ds_text: public zb_source
{
	z_directory _dir;
	//z_obj_map<zb_ds_table_txt> _tables;

protected:
 	virtual z_status _table_new(ctext ds_table_name,zb_ds_table*& tbl);
public:
	zb_ds_text();
	zb_ds_text(ctext name);
	virtual ~zb_ds_text();


 	 /*________________________________________
	 Datasource Virtual Funcs
	 __________________________________________*/
	
	virtual z_status open(bool create,bool writable);
	virtual z_status close();
	virtual z_status commit();	
	virtual z_status delete_datasource();

	/*________________________________________
	????
	________________________________________*/
	//virtual z_status get_table_list(z_obj_vector_map<zb_ds_table> & list);

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
#endif
