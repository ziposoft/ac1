#include "zb_pch.h"

#include "zbase_lib/include/ds_hamster.h"
#ifdef ZB_INCLUDE_DS_HAMSTER
/*___________________________________________________________________________

	zb_ds_hmt_tbl 
____________________________________________________________________________*/


zb_ds_hmt_tbl::zb_ds_hmt_tbl():zb_ds_table(0,0)
{
}

zb_ds_hmt_tbl::zb_ds_hmt_tbl(zb_ds_hmt* ds,ctext unique_id):zb_ds_table(ds,unique_id)
{
	_ds=ds;
}
z_status zb_ds_hmt_tbl::record_add(zb_ds_rec_ptr* rec)
{
	_current_row=dynamic_cast<zb_rec_ptr_hmt*>(rec);
	
	if(!_current_row)
	{
		return Z_ERROR(zs_bad_parameter);
	}
	_data.push_back(_current_row);
	
	return zs_ok;
}

z_status zb_ds_hmt_tbl::field_new(type_ds_field type,ctext id,zb_ds_field*& fld)
{
	z_status status=zb_validate_identifier(id);
	if(status)
		return Z_ERROR_MSG(zs_syntax_error,"cannot create CSV field. id=\"%s\" is invalid\n",id);

	fld=0;
	if((type>=type_ds_field_max)||(type<=type_ds_field_invalid))
		return Z_ERROR(zs_bad_parameter);

	switch(	type)
	{
 	case type_ds_field_string:
		fld=z_new zb_ds_field_text_string(id);

		break;

	case type_ds_field_int:

	default:
		return Z_ERROR(zs_operation_not_supported);

	};
	return 	 zs_ok;

}


zb_ds_field* zb_ds_hmt_tbl::field_string_new(ctext id)
{
	z_status status=zb_validate_identifier(id);
	if(status)
	{
		Z_ERROR_MSG(zs_syntax_error,"cannot create CSV field. id=\"%s\" is invalid\n",id);
		return 	0;
	}

	return z_new zb_ds_field_text_string(id);
}
zb_ds_rec_ptr* zb_ds_hmt_tbl::record_solo_new()
{
	zb_rec_ptr_txt* pRec=	z_new zb_rec_ptr_txt(true);
	return pRec;
}
ctext zb_ds_hmt_tbl::get_file_name()
{
	if(!_file_name)
		_file_name=	_id+".csv";
	return _file_name;

}
z_status zb_ds_hmt_tbl::close()
{
	int i;

	get_desc().clear();

	_status=status_closed;
	return zs_ok;

}

z_status zb_ds_hmt_tbl::commit()
{
	Z_ASSERT(_id);
	z_status s=_file.open(get_file_name(),"wb");
	if(s)
		return Z_ERROR(zs_could_not_open_file);
	ZT("opened file %s",get_file_name());
	size_t i_rec;
	zb_ds_field *fld;
	bool header=true;
	z_string data;
	zb_ds_rec_ptr* rec;


	z_map_iter i_fld;
	bool comma=false;
	while(fld=_ds_desc.get_next(i_fld))
	{
		if(comma)
			_file <<',';
		data=fld->_id;
		z_csv_encode_string(data);
		_file <<data;
		comma=true;
	}
	_file <<'\n';


	for(i_rec=0;i_rec<get_record_count();i_rec++)
	{
		i_fld.reset();
		comma=false;
		while(fld=_ds_desc.get_next(i_fld))
		{
			if(comma)
				_file <<',';
			rec=_data[i_rec];
			fld->get_string(rec,data);
			z_csv_encode_string(data);
			_file <<data;
			comma=true;
		}
		_file <<'\n';
	}
	_file.close();
	return zs_ok;

}
z_status zb_ds_hmt_tbl::delete_record_by_index(size_t index)
{
 	if(_status<	status_opened_read)
		return Z_ERROR(zs_not_open);
	if(index>=get_record_count())
		return Z_ERROR(zs_out_of_range);
	_data.erase( _data.begin() + index)	;
	return zs_ok;

}



z_status zb_ds_hmt_tbl::open(bool writable)
{

	_current_column=0;
	if(_status!=status_closed)
		return zs_already_open;

	ctext flags="r";
	/*
	current we just read it all in then write it all out on commit
	if(writable)
	{
		if(z_file_exists(_id)==0)
			flags="rb+";
		else
  			flags="wb+";

	}
	*/

	z_status s=_file.open(get_file_name(),flags);
	if(s)
	{
		if(writable)
			return zs_ok; //this is a new file, it will be created on commit

		return Z_ERROR(zs_could_not_open_file);
	}

	z_string data;
	_file.read_all(data);
	ParseBuffer(data.c_str(),data.size());
	_file.close();
	_status=(writable? status_opened_write:status_opened_read);

	return 0;
}


size_t zb_ds_hmt_tbl::get_record_count()
{
	return _data.size();
}
z_status zb_ds_hmt_tbl::get_record_by_index(size_t index,
											  zb_ds_rec_ptr** cursor)
{
	if(index>=get_record_count())
		return Z_ERROR(zs_out_of_range);
	if(cursor==0)
		return Z_ERROR(zs_bad_parameter);

	//zb_rec_ptr_txt* r=dynamic_cast<zb_rec_ptr_txt*>(*cursor);
	*cursor=_data[index];
	return zs_ok;
}



/*___________________________________________________________________________

	zb_ds_hmt 
____________________________________________________________________________*/

zb_ds_hmt::zb_ds_hmt(ctext name) : zb_source()
{

}


zb_ds_hmt::~zb_ds_hmt()
{
	//TODO delete 
}
z_status zb_ds_hmt::open(ctext name,bool create,bool writable)
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
	_dir.get_files_by_extension("hamster",list);

	for(i=0;i<list.size();i++)
	{
		z_string name,path,ext;
		z_filesys_get_filename_from_path(list[i],path,name,ext);
		
		_ds_tables << new 
		//_tables << new 
			zb_ds_hmt_tbl(this,name);

	}

	status=z_change_dir(_name,false);
	if(status)
  		return Z_ERROR_MSG(zs_could_not_open_file,"can't change to directory");

	_status=status_opened_write;
	 return 0;
}




z_status zb_ds_hmt::close()
{
	 return Z_ERROR_NOT_IMPLEMENTED;

}
z_status zb_ds_hmt::_table_new(ctext ds_table_name,zb_ds_table*& tbl)
{

	zb_ds_hmt_tbl* tbl_text=z_new zb_ds_hmt_tbl(this,ds_table_name);
	tbl=tbl_text;
	return  zs_ok;
}

z_status zb_ds_hmt::commit()
{

	return zb_source::commit();

}

#endif