#include "zb_pch.h"
#include "zb.h"



#ifdef ZB_INCLUDE_DS_TEXT

#include "ds_text.h"

/*___________________________________________________________________________

	zb_ds_text 
____________________________________________________________________________*/
zb_ds_text::zb_ds_text(ctext name) : zb_source(name  )
{

}
zb_ds_text::zb_ds_text() : zb_source("text"  )
{

}

zb_ds_text::~zb_ds_text()
{
	//TODO delete 
}
z_status zb_ds_text::open(bool create,bool writable)
{
	z_status status;
	size_t i;


	if(_status!=status_closed)
		return zs_already_open;

	status=_dir.open(_name,create);
	if(status)
		return Z_ERROR_MSG(zs_could_not_open_file,"can't open directory");

	z_strlist list;
	_dir.get_files_by_extension("csv",list);

	for(i=0;i<list.size();i++)
	{
		z_string name,path,ext;
		z_filesys_get_filename_from_path(list[i],path,name,ext);
		
		_ds_tables << new 
		//_tables << new 
			zb_ds_table_txt(this,name);

	}

	status=z_directory_change(_name,false);
	if(status)
  		return Z_ERROR_MSG(zs_could_not_open_file,"can't change to directory");

	_status=status_opened_write;
	 return zs_ok;
}

z_status zb_ds_text::delete_datasource()															   
{
	close();//close any handles
	z_directory_delete(_fullpath);

	_status=status_does_not_exist;
	return  zs_ok;


}



z_status zb_ds_text::close()
{
	 return Z_ERROR_NOT_IMPLEMENTED;

}
z_status zb_ds_text::_table_new(ctext ds_table_name,zb_ds_table*& tbl)
{

	zb_ds_table_txt* tbl_text=z_new zb_ds_table_txt(this,ds_table_name);
	tbl=tbl_text;
	return  zs_ok;
}

z_status zb_ds_text::commit()
{

	return zb_source::commit();

}

/*___________________________________________________________________________

	zb_ds_table_txt 
____________________________________________________________________________*/

zb_ds_table_txt::zb_ds_table_txt():zb_ds_table(0,0)
{
	_ds=0;
	_current_row=0;
	_current_column=0;
	_dirty=false;
}

zb_ds_table_txt::zb_ds_table_txt(zb_ds_text* ds,ctext unique_id):zb_ds_table(ds,unique_id)
{
	_ds=ds;
	_current_row=0;
	_current_column=0;
	_dirty=false;
}
z_status zb_ds_table_txt::record_add(zb_ds_rec_ptr* rec)
{
	_current_row=dynamic_cast<zb_rec_ptr_txt*>(rec);
	
	if(!_current_row)
	{
		return Z_ERROR(zs_bad_parameter);
	}
	_data.push_back(_current_row);
	
	return zs_ok;
}

z_status zb_ds_table_txt::field_new(type_ds_field type,ctext id,zb_ds_field*& fld)
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


zb_ds_field* zb_ds_table_txt::field_string_new(ctext id)
{
	z_status status=zb_validate_identifier(id);
	if(status)
	{
		Z_ERROR_MSG(zs_syntax_error,"cannot create CSV field. id=\"%s\" is invalid\n",id);
		return 	0;
	}

	return z_new zb_ds_field_text_string(id);
}
zb_ds_rec_ptr* zb_ds_table_txt::record_solo_new()
{
	zb_rec_ptr_txt* pRec=	z_new zb_rec_ptr_txt(true);
	return pRec;
}
ctext zb_ds_table_txt::get_file_name()
{
	if(!_file_name)
		_file_name=	_id+".csv";
	return _file_name;

}
z_status zb_ds_table_txt::close()
{
	size_t i;
	for(i=0;i<_data.size();i++)
	{
 		zb_ds_rec_ptr* rec=_data[i];
		if(rec)
		{
			z_delete rec;
		}
	}
	_data.clear();
	get_desc().clear();

	_status=status_closed;
	return zs_ok;

}

z_status zb_ds_table_txt::commit()
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
z_status zb_ds_table_txt::delete_record_by_index(size_t index)
{
 	if(_status<	status_opened_read)
		return Z_ERROR(zs_not_open);
	if(index>=get_record_count())
		return Z_ERROR(zs_out_of_range);
	_data.erase( _data.begin() + index)	;
	return zs_ok;

}



z_status zb_ds_table_txt::open(bool writable)
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

	return zs_ok;
}


size_t zb_ds_table_txt::get_record_count()
{
	return _data.size();
}
z_status zb_ds_table_txt::get_record_by_index(size_t index,
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
bool zb_ds_table_txt::NewRowCallback()
{
	if(_row_idx>0)
	{


		_current_row=new zb_rec_ptr_txt();
		_data.push_back(_current_row);
	}
	_current_column=0;
	return true;
}
bool zb_ds_table_txt::EndRowCallback()
{

	return true;
}
bool zb_ds_table_txt::NewValueCallback(const z_string & value)
{
	if(_row_idx==0)
	{
		//get fields.
		zb_ds_field* fld=field_string_new(value);
		if(!fld)
			return false;
		get_desc()<<fld;

	}
	else
	{
		if(_current_row==0)
		{
			Z_ERROR(zs_data_error);
			return false;
		}

		_current_row->set_string(_current_column,value);
	}
	_current_column++;
	return true;
}
/*___________________________________________________________________________

	zb_rec_solo 
____________________________________________________________________________*/
zb_rec_solo::zb_rec_solo()
{



}

zb_rec_solo::~zb_rec_solo()
{



}

/*___________________________________________________________________________

	zb_rec_ptr_txt 
____________________________________________________________________________*/


zb_rec_ptr_txt::zb_rec_ptr_txt(bool solo)
{
	_set=0;
	_index=0;


}

zb_rec_ptr_txt::~zb_rec_ptr_txt()
{



}
void zb_rec_ptr_txt::set(zb_ds_table* rs,size_t index)
{
	//_set=dynamic_cast<zb_rec_ptr_txt*>(rs);
	//_index=index;

}
void zb_rec_ptr_txt::set_string(size_t index,ctext str)
{
	if(size()<=index)
		resize(index+1);
	(*this)[index]=str;
}
ctext zb_rec_ptr_txt::get_string(size_t index)
{
	if(size()<=index)
		return 0;
	return (*this)[index];
}
void zb_rec_ptr_txt::output_csv(z_file& out)
{
	size_t i;
	z_string s;
	for(i=0;i<size();i++)
	{
		s=at(i);
		z_csv_encode_string(s);
		out<<s;

	}

}


/*___________________________________________________________________________

	Fields 
____________________________________________________________________________*/


zb_ds_field_text::zb_ds_field_text(ctext id) : zb_ds_field(id)
{

}
zb_ds_field_text::~zb_ds_field_text()
{
}

zb_ds_field_text_string::zb_ds_field_text_string(ctext id)	  :		   zb_ds_field_text(id)
{

}
zb_ds_field_text_string::~zb_ds_field_text_string()
{
}

z_status zb_ds_field_text_string::set_string(zb_ds_rec_ptr *rec,ctext s)
{ 
	
	zb_rec_ptr_txt* rectxt=dynamic_cast<zb_rec_ptr_txt*>(rec);
	if(!rectxt)
		return Z_ERROR(zs_bad_parameter);
	int i=index_get();
	rectxt->set_string(i,s);
	return zs_ok;
}
z_status zb_ds_field_text_string::get_string(zb_ds_rec_ptr *rec,z_string& s)
{ 
	zb_rec_ptr_txt* rectxt=dynamic_cast<zb_rec_ptr_txt*>(rec);
	if(!rectxt)
		return Z_ERROR(zs_bad_parameter);
	int i=index_get();
	ctext str=	   rectxt->get_string(i);
	if(!str)
		return Z_ERROR(zs_out_of_range);
	s=str;
	return zs_ok;
}


#endif