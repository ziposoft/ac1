


#ifndef DS_HAMSTER_H
#define DS_HAMSTER_H
#include "zbase_lib/include/zb.h"

#ifdef ZB_INCLUDE_DS_HAMSTER

#include "zbase_lib/include/datasource.h"
#include "ham/hamsterdb.hpp"

class zb_ds_ham; 


/*___________________________________________________________________________

	zb_rec_ptr_ham 
____________________________________________________________________________*/
class zb_rec_ptr_ham  : public zb_ds_rec_ptr 
{

public:
	zb_rec_ptr_ham(bool solo=false);
 	virtual ~zb_rec_ptr_ham();
	virtual void set(zb_ds_table* rs,size_t index);

	ctext get_string(size_t index);
	void set_string(size_t index,ctext str);

};
 /*___________________________________________________________________________

	zb_ds_field_ham 
____________________________________________________________________________*/
class zb_ds_field_ham  : public zb_ds_field
{
	hamsterdb::db db;      /* hamsterdb database object */
	U16 _ham_id;
public:
	zb_ds_field_ham(U16 ham_id,ctext id);
	virtual ~zb_ds_field_ham();




// Overrides 


};

/*___________________________________________________________________________

	zb_ds_field_ham_string 
____________________________________________________________________________*/
class zb_ds_field_ham_string  : public zb_ds_field_ham
{

public:
	zb_ds_field_ham_string(U16 ham_id,ctext id);
	virtual ~zb_ds_field_ham_string();
	//For creating temp row, for filtering
	

// Overrides 

 	virtual z_status set_string(zb_ds_rec_ptr *rec,ctext s);
	virtual z_status get_string(zb_ds_rec_ptr *rec,z_string& s);
};


/*___________________________________________________________________________

	zb_ds_ham_tbl 
____________________________________________________________________________*/
class zb_ds_ham_tbl : public zb_ds_table	 
{


	zb_rec_ptr_ham* _current_row;
	hamsterdb::env _env;
	z_string _file_name;
	zb_ds_ham* _ham_ds;

public:
	zb_ds_ham_tbl();
	zb_ds_ham_tbl(zb_ds_ham* ds,ctext unique_id);
	virtual ~zb_ds_ham_tbl();

	ctext get_file_name();

	virtual z_status create_new();

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

	zb_ds_ham 
____________________________________________________________________________*/
class zb_ds_ham: public zb_source
{
	z_directory _dir;

protected:	
	virtual z_status _table_new(ctext ds_table_name,zb_ds_table*& tbl);
public:
	zb_ds_ham(ctext name);
	zb_ds_ham();
	virtual ~zb_ds_ham();

 	 /*________________________________________
	 Datasource Virtual Funcs
	 __________________________________________*/
	
	virtual z_status open(ctext name,bool create,bool writable);
	virtual z_status close();
	virtual z_status commit();	


};


#endif
#endif
