#include "zb_pch.h"

#include "zbase_lib/include/ds_hamster.h"
#ifdef ZB_INCLUDE_DS_HAMSTER

/*___________________________________________________________________________

zb_rec_ptr_ham 
____________________________________________________________________________*/

#define ZB_HAM_COL_META 1
#define ZB_HAM_COL_NAMES 2


zb_rec_ptr_ham::zb_rec_ptr_ham(bool solo)
{


}

zb_rec_ptr_ham::~zb_rec_ptr_ham()
{



}
void zb_rec_ptr_ham::set(zb_ds_table* rs,size_t index)
{
	//_set=dynamic_cast<zb_rec_ptr_txt*>(rs);
	//_index=index;

}
void zb_rec_ptr_ham::set_string(size_t index,ctext str)
{

}
ctext zb_rec_ptr_ham::get_string(size_t index)
{
	return "";
}
/*___________________________________________________________________________

zb_ds_field_ham
____________________________________________________________________________*/

zb_ds_field_ham::zb_ds_field_ham(U16 ham_id,ctext id) : zb_ds_field(id)
{
	_ham_id=ham_id;

}
zb_ds_field_ham::~zb_ds_field_ham()
{
}


/*___________________________________________________________________________

zb_ds_field_ham_string 
____________________________________________________________________________*/

zb_ds_field_ham_string::zb_ds_field_ham_string(U16 ham_id,ctext id) : zb_ds_field_ham(ham_id,id)
{

}
zb_ds_field_ham_string::~zb_ds_field_ham_string()
{
}



z_status zb_ds_field_ham_string::set_string(zb_ds_rec_ptr *rec,ctext s)
{ 
	/*
	zb_rec_ptr_txt* rectxt=dynamic_cast<zb_rec_ptr_txt*>(rec);
	if(!rectxt)
	return Z_ERROR(zs_bad_parameter);
	int i=index_get();
	rectxt->set_string(i,s);
	*/
	return zs_ok;
}
z_status zb_ds_field_ham_string::get_string(zb_ds_rec_ptr *rec,z_string& s)
{ 
	/*
	zb_rec_ptr_txt* rectxt=dynamic_cast<zb_rec_ptr_txt*>(rec);
	if(!rectxt)
	return Z_ERROR(zs_bad_parameter);
	int i=index_get();
	ctext str=	   rectxt->get_string(i);
	if(!str)
	return Z_ERROR(zs_out_of_range);
	s=str;
	*/
	return zs_ok;
}


/*___________________________________________________________________________

zb_ds_ham_tbl 
____________________________________________________________________________*/


zb_ds_ham_tbl::zb_ds_ham_tbl():zb_ds_table(0,0)
{
}
zb_ds_ham_tbl::~zb_ds_ham_tbl()
{
}
zb_ds_ham_tbl::zb_ds_ham_tbl(zb_ds_ham* ds,ctext unique_id):zb_ds_table(ds,unique_id)
{
	_ds=ds;
}
z_status zb_ds_ham_tbl::record_add(zb_ds_rec_ptr* rec)
{
	_current_row=Z_RECAST(zb_rec_ptr_ham,rec);

	if(!_current_row)
	{
		return Z_ERROR(zs_bad_parameter);
	}

	return zs_ok;
}

z_status zb_ds_ham_tbl::field_new(type_ds_field type,ctext textid,zb_ds_field*& fld)
{
	z_status status=zb_validate_identifier(textid);
	U16 ham_id=0;
	if(status)
		return Z_ERROR_MSG(zs_syntax_error,"cannot create CSV field. id=\"%s\" is invalid\n",textid);

	fld=0;
	if((type>=type_ds_field_max)||(type<=type_ds_field_invalid))
		return Z_ERROR(zs_bad_parameter);

	switch(	type)
	{
	case type_ds_field_string:
		fld=z_new zb_ds_field_ham_string(ham_id,textid);

		break;

	case type_ds_field_int:

	default:
		return Z_ERROR(zs_operation_not_supported);

	};
	return 	 zs_ok;

}


zb_ds_field* zb_ds_ham_tbl::field_string_new(ctext id)
{
	zb_ds_field* fld=0;
	z_status status=field_new(type_ds_field_string,id,fld);


	return fld;
}
zb_ds_rec_ptr* zb_ds_ham_tbl::record_solo_new()
{
	zb_rec_ptr_ham* pRec=	z_new zb_rec_ptr_ham(true);
	return pRec;
}

z_status zb_ds_ham_tbl::close()
{
	int i;

	get_desc().clear();

	_status=status_closed;
	return zs_ok;

}

z_status zb_ds_ham_tbl::commit()
{
	Z_ASSERT(_id);

	return zs_ok;

}
z_status zb_ds_ham_tbl::delete_record_by_index(size_t index)
{
	if(_status<	status_opened_read)
		return Z_ERROR(zs_not_open);
	if(index>=get_record_count())
		return Z_ERROR(zs_out_of_range);

	return zs_ok;

}

ctext zb_ds_ham_tbl::get_file_name()
{
	if(!_file_name)
		_file_name=	_id+".ham";
	return _file_name;

}
z_status zb_ds_ham_tbl::open(bool writable)															   
{

	if(_status!=status_closed)
		return zs_already_open;
	try 
	{
		_env.open(get_file_name());
		_status=(writable? status_opened_write:status_opened_read);

		return 0;
	}
	catch (hamsterdb::error &e) 
	{
		ham_status_t s=	e.get_errno();
		if(s!=HAM_FILE_NOT_FOUND)
		{
			return Z_ERROR_MSG(zs_could_not_open_file,"Could not open DB: (%d) %s",e.get_errno(),e.get_string());
		}
	}
	_status=status_closed;
	z_status s=create_new();
	if(s==zs_ok)
		_status=(writable? status_opened_write:status_opened_read);


	//ham_parameter_t params;
	//_env.get_parameters(&params);

	return s;
}
z_status zb_ds_ham_tbl::create_new()
{
	if(_status!=status_closed)
		return zs_already_open;
	try 
	{
		_env.create(get_file_name());
	}
	catch (hamsterdb::error &e) 
	{
		return Z_ERROR_MSG(zs_could_not_open_file,"Could not create DB: %s",e.get_string());
	}
	return 0;
}

size_t zb_ds_ham_tbl::get_record_count()
{
	return 0;
}
z_status zb_ds_ham_tbl::get_record_by_index(size_t index,
											zb_ds_rec_ptr** cursor)
{
	if(index>=get_record_count())
		return Z_ERROR(zs_out_of_range);
	if(cursor==0)
		return Z_ERROR(zs_bad_parameter);

	return zs_ok;
}



/*___________________________________________________________________________

zb_ds_ham 
____________________________________________________________________________*/

zb_ds_ham::zb_ds_ham(ctext name) : zb_source(name)
{

}
zb_ds_ham::zb_ds_ham() : zb_source("ham")
{

}


zb_ds_ham::~zb_ds_ham()
{
	//TODO delete 
}
z_status zb_ds_ham::open(ctext name,bool create,bool writable)
{
	z_status status;
	size_t i;


	if(_status!=status_closed)
		return zs_already_open;

	_name=name;
	status=_dir.open(_name,create);
	if(status)
		return Z_ERROR_MSG(zs_could_not_open_file,"can't open directory");

	z_strlist list;
	_dir.get_files_by_extension("ham",list);

	for(i=0;i<list.size();i++)
	{
		z_string name,path,ext;
		z_filesys_get_filename_from_path(list[i],path,name,ext);

		_ds_tables << new 
			zb_ds_ham_tbl(this,name);

	}

	status=z_change_dir(_name,false);
	if(status)
		return Z_ERROR_MSG(zs_could_not_open_file,"can't change to directory");

	_status=status_opened_write;
	return 0;
}




z_status zb_ds_ham::close()
{
	return Z_ERROR_NOT_IMPLEMENTED;

}
z_status zb_ds_ham::_table_new(ctext ds_table_name,zb_ds_table*& tbl)
{

	zb_ds_ham_tbl* tbl_text=z_new zb_ds_ham_tbl(this,ds_table_name);
	tbl=tbl_text;
	tbl_text->create_new();
	return  zs_ok;
}

z_status zb_ds_ham::commit()
{

	return zb_source::commit();

}

#endif