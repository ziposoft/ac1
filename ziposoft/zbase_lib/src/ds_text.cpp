#include "zb_pch.h"
#include "zb.h"



#ifdef ZB_INCLUDE_DS_TEXT

#include "ds_text.h"

/*___________________________________________________________________________

	zb_ds_text 
____________________________________________________________________________*/
zb_ds_text::zb_ds_text() : zb_source()
{

}


zb_ds_text::~zb_ds_text()
{
	//TODO delete 
}
z_status zb_ds_text::open(ctext name,bool create,bool writable)
{
	z_status status;
	size_t i;
	_name=name;
	status=_dir.open(_name,create);
	if(status)
		return Z_ERROR_MSG(zs_could_not_open_file,"can't open directory");

	z_strlist list;
	_dir.get_files_by_extension("txt",list);

	for(i=0;i<list.size();i++)
	{
		_tables << new 
			zb_ds_table_txt(this,list[i]);

	}

	status=z_change_dir(_name,false);
	if(status)
  		return Z_ERROR_MSG(zs_could_not_open_file,"can't change to directory");


	 return 0;
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
zb_ds_field* zb_ds_text::ds_field_string_new(ctext id)
{
	return z_new zb_ds_field_text_string(id);
}
zb_ds_rec_ptr* zb_ds_text::record_solo_new()
{
	zb_rec_ptr_txt* pRec=	z_new zb_rec_ptr_txt(true);
	return pRec;
}
z_status zb_ds_text::commit()
{
	z_map_iter i;
	zb_ds_table_txt* t;
	while(t=_tables.get_next(i))
	{
		t->commit();

	}
	return zs_ok;

}

/*___________________________________________________________________________

	zb_ds_table_txt 
____________________________________________________________________________*/



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
z_status zb_ds_table_txt::commit()
{
	z_status s=_file.open(_id,"wb");
	if(s)
		return Z_ERROR(zs_could_not_open_file);

	size_t i_rec;
	zb_ds_field *fld;
	z_string data;
	zb_ds_rec_ptr* rec;
	for(i_rec=0;i_rec<get_record_count();i_rec++)
	{
		z_map_iter i_fld;
		bool comma=false;
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
z_status zb_ds_table_txt::open(bool writable)
{
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
	z_status s=_file.open(_id,flags);
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

	return 0;
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
	
	_current_row=new zb_rec_ptr_txt();
	_data.push_back(_current_row);
	_current_column=0;
	return true;
}
bool zb_ds_table_txt::EndRowCallback()
{

	return true;
}
bool zb_ds_table_txt::NewValueCallback(const z_string & value)
{
	if(_current_row==0)
	{
		Z_ERROR(zs_data_error);
		return false;
	}
	_current_row->set_string(_current_column,value);
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


zb_ds_field_text::zb_ds_field_text(ctext id)
{
	_id=id;

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