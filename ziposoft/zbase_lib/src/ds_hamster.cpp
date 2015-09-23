#include "zb_pch.h"
#include "zb.h"
#ifdef ZB_INCLUDE_DS_HAMSTER
#include "ds_hamster.h"

struct z_ham_error_tanslate_st
{
	int ham_code;
	z_status status;
};
const z_ham_error_tanslate_st z_ham_error_tanslate_list[]=
{
	{HAM_SUCCESS,zs_ok},
	{HAM_INV_RECORD_SIZE,zs_unknown_error},
	{HAM_INV_KEY_SIZE,zs_unknown_error},
	{HAM_INV_FILE_VERSION,zs_unknown_error},
	{HAM_DUPLICATE_KEY,zs_unknown_error},
	{HAM_WRITE_PROTECTED,zs_unknown_error},

};
const size_t z_ham_error_tanslate_list_size=sizeof(z_ham_error_tanslate_list)/sizeof(z_ham_error_tanslate_st);

 z_status z_get_zerror_from_ham_error(int ham_error)
{
	size_t i;
	for(i=0;i<z_ham_error_tanslate_list_size;i++)
	{
		if(z_ham_error_tanslate_list[i].ham_code==ham_error)
			return z_ham_error_tanslate_list[i].status;
	}
	return zs_unknown_error;
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
z_status zb_ds_ham::open(bool create,bool writable)
{
	z_status status;
	size_t i;

	if(_status>	status_opened_read)
		return zs_ok;  //zs_already_open??

	if(_status==status_opened_read)
	{
		if(!writable)
			return zs_ok;  //zs_already_open??
		//we have a call to 
		close();//close and reopen?
	}


	status=_dir.open(_name,create);
	if(status)
		return Z_ERROR_MSG(zs_could_not_open_file,"can't open directory");

	z_strlist list;
	_dir.get_files_by_extension("ham",list);

	for(i=0;i<list.size();i++)
	{
		z_string name;
		z_filesys_get_path_parts(list[i],0,&name,0);

		_ds_tables << z_new 
			zb_ds_ham_tbl(this,name);

	}

	_status=status_opened_write;
	return zs_ok;
}
z_status zb_ds_ham::delete_datasource()															   
{
	z_status status=close();//close any handles
	if(z_file_exists(_fullpath)!=zs_ok)
		return zs_ok;

	if(status==zs_ok)
		status=z_directory_delete_tree(_fullpath);

	_status=status_does_not_exist;
	return  status;


}



z_status zb_ds_ham::close()
{
	//nothing to do?
	return zb_source::close();

}
z_status zb_ds_ham::_table_new(ctext ds_table_name,zb_ds_table*& tbl)
{

	zb_ds_ham_tbl* tbl_text=z_new zb_ds_ham_tbl(this,ds_table_name);
	tbl=tbl_text;
	return tbl_text->create_new();
}

z_status zb_ds_ham::commit()
{

	return zb_source::commit();

}

/*___________________________________________________________________________

zb_ds_ham_tbl 
____________________________________________________________________________*/


zb_ds_ham_tbl::zb_ds_ham_tbl():zb_ds_table(0,0)
{
	_env=0;
}
zb_ds_ham_tbl::~zb_ds_ham_tbl()
{
}
zb_ds_ham_tbl::zb_ds_ham_tbl(zb_ds_ham* ds,ctext unique_id):zb_ds_table(ds,unique_id)
{
	_ds=ds;
	_env=0;
}

z_status zb_ds_ham_tbl::open(bool writable)															   
{
	ham_status_t st_ham;          /* status variable */

	if(_status>	status_opened_read)
		return zs_ok;  //zs_already_open??
	if(_status==status_opened_read)
	{
		if(!writable)
			return zs_ok;  //zs_already_open??
		//we have a call to 
		close();//close and reopen?
	}
	st_ham=ham_env_open(&_env, get_file_name(),  0,  0);
	if(st_ham==HAM_SUCCESS)
	{
		_status=(writable? status_opened_write:status_opened_read);
		return zs_ok;
	}
	if(st_ham!=HAM_FILE_NOT_FOUND)
	{
		return Z_ERROR_MSG(zs_could_not_open_file,"Could not open DS: (%d) %s",st_ham,ham_strerror(st_ham));
	}
	if(writable)
		return create_new();

	_status=status_closed;
	z_status s=create_new();
	if(s==zs_ok)
		_status=(writable? status_opened_write:status_opened_read);

	return Z_ERROR(zs_could_not_open_file);
}
z_status zb_ds_ham_tbl::create_new()
{
	ham_status_t st_ham;          /* status variable */
	if(_status!=status_closed)
		return zs_already_open;

	st_ham=ham_env_create(&_env, get_file_name(),  0, 0664, 0);
	if(st_ham)
	{
		return Z_ERROR_MSG(zs_could_not_open_file,"Could not create DS: %s",ham_strerror(st_ham));
	}
	_status=status_opened_write;
	return zs_ok;
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
		return Z_ERROR_MSG(zs_syntax_error,"cannot create HAM field. id=\"%s\" is invalid\n",textid);

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
	ham_status_t st_ham;         

	st_ham = ham_env_close(_env, HAM_AUTO_CLEANUP);
	if(st_ham)
	{
		Z_ERROR_MSG(zs_could_not_open_file,"Could not close ham ds: %s",ham_strerror(st_ham));
	}

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
	if(!_file_path)
	{
		_file_path=_ds->get_full_path();
		_file_path << '/';
		_file_path <<	_id <<".ham";
	}
	return _file_path;

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



#endif