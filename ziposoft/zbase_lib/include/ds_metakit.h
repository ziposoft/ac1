#ifndef DS_METAKIT_H
#define DS_METAKIT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"
#include "metakit/include/mk4.h"
#include "metakit/include/mk4str.h"


class zb_ds_metakit: public zb_source
{

    c4_Storage      *_pStore;
	z_string		_filename;

 	z_status _get_view_for_table(c4_View& view,zb_table_base* tbl);
	z_status _get_view(c4_View& view,ctext viewid,zb_ds_desc & desc);

public:
	zb_ds_metakit(ctext name);
	virtual ~zb_ds_metakit(void);
	virtual z_status open(bool writable);
	virtual z_status close();
	virtual z_status commit();
	virtual zb_ds_field* ds_field_string_new(ctext id);
	virtual zb_record* get_solo_record(zb_table_base* tbl);

	virtual zb_ds_table* get_tbl(ctext ds_table_name,zb_desc& desc);
 	virtual zb_ds_table* ds_table_new(ctext ds_table_name);
	virtual z_status     ds_table_open(zb_ds_table* tbl);


};

class zb_record_mk  : public zb_record
{
public:
	zb_record_mk();
 	virtual ~zb_record_mk(){};

 	c4_Row _row;


};
class zb_ds_table_mk  : public zb_ds_table
{
 	c4_View _mk_view;

public:
	zb_ds_metakit* _ds;
	c4_View& get_mk_view() { return _mk_view;}

	virtual z_status record_add(zb_record* rec);
	zb_ds_table_mk(zb_ds_metakit* ds,ctext unique_id);
};

class zb_ds_recordset_mk  : public zb_ds_recordset
{
public:
 	c4_View _mk_view;



};



class zb_ds_field_mk  : public zb_ds_field
{
protected:
	z_string		_id;
public:
	zb_ds_field_mk(ctext id);
	virtual ~zb_ds_field_mk();

    virtual const c4_Property& GetProperty()=0;
	virtual void MakeDesc(z_string &str)=0;


};
class zb_ds_field_mk_string  : public zb_ds_field_mk
{

    c4_StringProp* _pStrProp;
public:
	zb_ds_field_mk_string(ctext id);
	virtual ~zb_ds_field_mk_string();
	//For creating temp row, for filtering
	c4_Row operator[] (ctext i) const
	{
		return (*_pStrProp)[i];
	};
	//For creating temp row, for filtering
	c4_Row row(ctext i) const
	{
		return (*_pStrProp)[i];
	};
	//For creating sorts
    const c4_Property& GetProperty() { return *_pStrProp ; };
    const c4_StringProp& GetStrProp() { return *_pStrProp ; };

	virtual void MakeDesc(z_string &str) { 
        str+=_id; 
        str+=":S"; 
    };

// Overrides 

 	virtual z_status set_string(zb_record *rec,ctext s);
	virtual z_status get_string(zb_record *rec,z_string& s);
};
#endif
