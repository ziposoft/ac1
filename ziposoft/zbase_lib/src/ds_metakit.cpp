#include "zb_pch.h"

#include "zbase_lib/include/ds_metakit.h"


 /*____________________________________________________________________________

	Metakit hacks  
____________________________________________________________________________*/

bool g_notify_on=true;	//TODO: metakit performance experiment 
void  d4_assert(bool a)
{
	if(!a)
	{
		Z_ASSERT(0);
		Z_ERROR(zb_status_ds_data_error,"metakit error");
	}
}
/*____________________________________________________________________________

	Metakit public functions 
____________________________________________________________________________*/



zb_ds_metakit::zb_ds_metakit(ctext name) : zb_source( name)
{
	_pStore=0;
	_filename<<name <<".mtk";

}
zb_ds_metakit:: ~zb_ds_metakit(void)
{
    if (_pStore)
	{   delete _pStore;  }

}

zb_ds_record* zb_ds_metakit::record_solo_new()
{
	zb_record_mk* pRec=	z_new zb_record_mk();
	return pRec;
}
zb_ds_table* zb_ds_metakit::ds_table_new(ctext ds_table_name)
{

	zb_ds_table_mk* tbl=z_new zb_ds_table_mk(this,ds_table_name);
	return  tbl;
}
z_status zb_ds_metakit::ds_table_open(zb_ds_table* tbl)
{
	zb_ds_table_mk* mtbl= dynamic_cast<zb_ds_table_mk*>(tbl);
	_get_view(mtbl->get_mk_view(),mtbl->get_ds_id(),mtbl->get_desc());
	return zb_status_ok;
}

zb_ds_field* zb_ds_metakit::ds_field_string_new(ctext id)
{
	return z_new zb_ds_field_mk_string(id);
}
	//virtual z_status     ds_table_open(zb_ds_table* tbl);


z_status zb_ds_metakit::open(bool writable) 
{
	_pStore= z_new c4_Storage(_filename,(writable?1:0));
	if(_pStore->Strategy().IsValid())
	{
		_status=status_opened_read;
		if(writable) 
			_status=status_opened_write;
		return zb_status_ok;
	}
	delete _pStore;
	_pStore=0;
	_status=status_cant_open;

	return zb_status_cant_open_file;

}
z_status zb_ds_metakit::commit() 
{
    if (_status==status_opened_need_commit) //wait this aint gonna work
	{
		if (_pStore) 
		{
			ZT(("commiting %s",	_filename.c_str() ));

			_pStore->Commit(true);
			_status=status_opened_write;
		}
	}
	else
	{
		ZT(("file=%x, %s commit not needed ",this,	_filename.c_str() ));
	}

	return zb_status_ok;
}
z_status zb_ds_metakit::close() 
{
	if (_pStore) delete _pStore;
	_pStore=0;
	_status=status_closed;
	return zb_status_ok;
}
/*____________________________________________________________________________

	Metakit private functions 
____________________________________________________________________________*/

z_status zb_ds_metakit::_get_view(c4_View& view,ctext viewid,zb_ds_desc & desc)
{

	bool metakit_blocked_view=false; //TODO: metakit blocked views
    z_string str=viewid;

	if(metakit_blocked_view) str+="[_B";
	str+="[";

	z_map_iter i;
	zb_ds_field* f=0;
	bool comma=false;
	while(f=desc.get_next(i))
	{
        if(comma) str+=",";
		zb_ds_field_mk * mkf=(zb_ds_field_mk*)f;

        mkf->MakeDesc(str);
		comma=true;
	}
    str+="]";
	if(metakit_blocked_view) str+="]";


	
	ZT(("metakit view: %s, viewstr=%s",viewid ,str.c_str()));	

	c4_View temp_view;
	try
	{
#if BLOCKED		
		temp_view=GetFile()->_pStore->GetAs(str); //get_mk_view(str); //TODO catch assertions HERE, for corrupt databases.
		if(_use_blocked_view) 
		{
			view=temp_view.Blocked();
		}
#else
		view=_pStore->GetAs(str); //TODO catch assertions HERE, for corrupt databases.

#endif
	}

	catch(...)
	{
		_status=status_corrupt;
		//Z_ASSERT(0);
		return Z_ERROR_RETURN(	zb_status_ds_data_error,"Error accessing %s",viewid);

	}
	return zb_status_ok;
}

z_status zb_ds_metakit::_get_view_for_table(c4_View& view,zb_table_base* tbl)
{

	bool metakit_blocked_view=false; //TODO: metakit blocked views
    z_string str="T";
	str+=tbl->get_key();

	if(metakit_blocked_view) str+="[_B";
	str+="[";

	z_map_iter i;
	zb_field* f=0;
	bool comma=false;
	while(f=tbl->get_desc().get_next(i))
	{
        if(comma) str+=",";
		zb_ds_field_mk * mkf=(zb_ds_field_mk*)f->get_ds_field();

        mkf->MakeDesc(str);
		comma=true;
	}
    str+="]";
	if(metakit_blocked_view) str+="]";


	
	ZT(("Table# %x, %s, viewstr=%s", tbl->get_key(),tbl->get_name(),str.c_str()));	

	c4_View temp_view;
	try
	{
#if BLOCKED		
		temp_view=GetFile()->_pStore->GetAs(str); //get_mk_view(str); //TODO catch assertions HERE, for corrupt databases.
		if(_use_blocked_view) 
		{
			view=temp_view.Blocked();
		}
#else
		view=_pStore->GetAs(str); //TODO catch assertions HERE, for corrupt databases.

#endif
	}

	catch(...)
	{
		_status=status_corrupt;
		//Z_ASSERT(0);
		return Z_ERROR_RETURN(	zb_status_ds_data_error,"Error accessing %s",tbl->get_name());

	}
	return zb_status_ok;
}

 /*____________________________________________________________________________

	 Metakit Fields
____________________________________________________________________________*/


zb_ds_field_mk::zb_ds_field_mk(ctext id)
{
	_id=id;

}
zb_ds_field_mk::~zb_ds_field_mk()
{
}
zb_ds_field_mk_string::zb_ds_field_mk_string(ctext id) :zb_ds_field_mk(id)
{
	_pStrProp=z_new c4_StringProp(id);

}
zb_ds_field_mk_string::~zb_ds_field_mk_string()
{
}
z_status zb_ds_field_mk_string::set_string(zb_rec_ptr *rec,ctext s)
{ 
	
	zb_rec_ptr_mk* mk_rec=dynamic_cast<zb_rec_ptr_mk*>(rec);
	if(!mk_rec)
		return ZB_ERROR(zb_status_bad_param);
 	(*_pStrProp).Set(mk_rec->get_row_ref(),s);

	return zb_status_ok;
}
z_status zb_ds_field_mk_string::get_string(zb_rec_ptr *rec,z_string& s)
{ 
	zb_rec_ptr_mk* mk_rec=dynamic_cast<zb_rec_ptr_mk*>(rec);
	if(!mk_rec)
		return ZB_ERROR(zb_status_bad_param);
	s=(*_pStrProp).Get(mk_rec->get_row_ref());

	return zb_status_ok;
}

/*____________________________________________________________________________

	 Metakit zb_record_mk
____________________________________________________________________________*/
zb_record_mk::zb_record_mk()   :	 zb_ds_record()
{


}

/*____________________________________________________________________________

	 Metakit Recordsets
____________________________________________________________________________*/


/*____________________________________________________________________________

	 Metakit Tables
____________________________________________________________________________*/

zb_ds_table_mk::zb_ds_table_mk(zb_ds_metakit* ds,ctext unique_id):zb_ds_table(ds,unique_id)
{
	_ds=ds;
}
z_status zb_ds_table_mk::record_add(zb_ds_record* rec)
{
	zb_record_mk* mk_rec=dynamic_cast<zb_record_mk*>(rec);
	int index=_mk_view.Add(mk_rec->get_row_ref());

	_ds->_status=zb_ds_metakit::status_opened_need_commit;
	return zb_status_ok;
}

z_status zb_ds_table_mk::open()
{
	
	return _ds->_get_view(get_mk_view(),get_ds_id(),get_desc());
}
size_t zb_ds_table_mk::get_record_count()
{
	return _mk_view.GetSize();
	
}
z_status zb_ds_table_mk::get_record_by_index(size_t index,zb_rec_ptr** cursor)
{
	if( index>=(size_t)_mk_view.GetSize())
		return ZB_ERROR(zb_status_index_out_of_range);
	if(cursor==0)
		return ZB_ERROR(zb_status_bad_param);

	zb_rec_ptr_mk* r=*(zb_rec_ptr_mk**)cursor;
	if(r==0)
		r=new zb_rec_ptr_mk();

	r->get_row_ref()=_mk_view[index];
	return zb_status_ok;
}
