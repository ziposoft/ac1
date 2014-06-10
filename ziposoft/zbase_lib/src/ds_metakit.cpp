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
		Z_ERROR_MSG(zs_error,"metakit error");
	}
}
/*____________________________________________________________________________

	Metakit public functions 
____________________________________________________________________________*/



zb_ds_metakit::zb_ds_metakit() : zb_source( )
{
	_pStore=0;

}
zb_ds_metakit:: ~zb_ds_metakit(void)
{
    if (_pStore)
	{   delete _pStore;  }

}

zb_ds_rec_ptr* zb_ds_metakit::record_solo_new()
{
	zb_rec_ptr_mk* pRec=	z_new zb_rec_ptr_mk(true);
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
	return zs_ok;
}

zb_ds_field* zb_ds_metakit::ds_field_string_new(ctext id)
{
	return z_new zb_ds_field_mk_string(id);
}
	//virtual z_status     ds_table_open(zb_ds_table* tbl);


z_status zb_ds_metakit::open(ctext name,bool create,bool writable) 
{
	//TODO - only create when asked to
	_filename<<name <<".mtk";

	_pStore= z_new c4_Storage(_filename,(writable?1:0));
	if(_pStore->Strategy().IsValid())
	{
		_status=status_opened_read;
		if(writable) 
			_status=status_opened_write;
		return zs_ok;
	}
	delete _pStore;
	_pStore=0;
	_status=status_cant_open;

	return zs_could_not_open_file;

}
z_status zb_ds_metakit::commit() 
{
    if (_status==status_opened_need_commit) //wait this aint gonna work
	{
		if (_pStore) 
		{
			ZT("commiting %s",	_filename.c_str() );

			_pStore->Commit(true);
			_status=status_opened_write;
		}
	}
	else
	{
		ZT("file=%x, %s commit not needed ",this,	_filename.c_str() );
	}

	return zs_ok;
}
z_status zb_ds_metakit::close() 
{
	if (_pStore) delete _pStore;
	_pStore=0;
	_status=status_closed;
	return zs_ok;
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


	
	ZT("metakit view: %s, viewstr=%s",viewid ,str.c_str());	

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
		return Z_ERROR_MSG(	zs_data_error,"Error accessing %s",viewid);

	}
	return zs_ok;
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


	
	ZT("Table# %x, %s, viewstr=%s", tbl->get_key(),tbl->get_name(),str.c_str());	

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
		return Z_ERROR_MSG(	zs_data_error,"Error accessing %s",tbl->get_name());

	}
	return zs_ok;
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
z_status zb_ds_field_mk_string::set_string(zb_ds_rec_ptr *rec,ctext s)
{ 
	
	zb_rec_ptr_mk* mk_rec=dynamic_cast<zb_rec_ptr_mk*>(rec);
	if(!mk_rec)
		return Z_ERROR(zs_bad_parameter);
 	(*_pStrProp).Set(mk_rec->get_row_ref(),s);

	return zs_ok;
}
z_status zb_ds_field_mk_string::get_string(zb_ds_rec_ptr *rec,z_string& s)
{ 
	zb_rec_ptr_mk* mk_rec=dynamic_cast<zb_rec_ptr_mk*>(rec);
	if(!mk_rec)
		return Z_ERROR(zs_bad_parameter);
	c4_RowRef rr=mk_rec->get_row_ref();
	s=(*_pStrProp).Get(rr);

	return zs_ok;
}
/*____________________________________________________________________________

	 Metakit zb_rec_ptr_mk
____________________________________________________________________________*/
c4_RowRef zb_rec_ptr_mk::get_row_ref()   
{
	if(_row)
		return *_row;
	return _set->get_mk_view()[_index]	;

}
zb_rec_ptr_mk::zb_rec_ptr_mk(bool solo)   :	 zb_ds_rec_ptr()
{
	_set=0;
	_index=0;
	if(solo)
		_row=z_new c4_Row();
	else
		_row=0;
}
zb_rec_ptr_mk::~zb_rec_ptr_mk() 
{
	if(_row)
		z_delete _row;
}
void zb_rec_ptr_mk::set(zb_ds_table* rs,size_t index)
{
	_set=dynamic_cast<zb_ds_table_mk*>(rs);
	_index=index;

}
/*____________________________________________________________________________

	 Metakit zb_record_mk
____________________________________________________________________________*/

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
z_status zb_ds_table_mk::record_add(zb_ds_rec_ptr* rec)
{
	zb_rec_ptr_mk* mk_rec=dynamic_cast<zb_rec_ptr_mk*>(rec);
	int index=_mk_view.Add(mk_rec->get_row_ref());

	_ds->_status=zb_ds_metakit::status_opened_need_commit;
	return zs_ok;
}

z_status zb_ds_table_mk::open(bool writable)
{
	
	return _ds->_get_view(_mk_view,get_ds_id(),get_desc());
}
size_t zb_ds_table_mk::get_record_count()
{
	return _mk_view.GetSize();
	
}
z_status zb_ds_table_mk::get_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	if( index>=(size_t)_mk_view.GetSize())
		return Z_ERROR(zs_out_of_range);
	if(cursor==0)
		return Z_ERROR(zs_bad_parameter);

	zb_rec_ptr_mk* r=dynamic_cast<zb_rec_ptr_mk*>(*cursor);
	if(r==0)
		r=new zb_rec_ptr_mk(false);
	r->set(this,index);
	*cursor=r;
	//r->get_row_ref()=_mk_view[index];
	return zs_ok;
}
z_status zb_ds_table_mk::test_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	if( index>=(size_t)_mk_view.GetSize())
		return Z_ERROR(zs_out_of_range);
	if(cursor==0)
		return Z_ERROR(zs_bad_parameter);

	c4_StringProp p("field1str");

	get_record_by_index(index,cursor);
	gz_out << p.Get(_mk_view[index])<<"\n";
	c4_RowRef& rr=_mk_view[index];
	gz_out << p.Get(rr)<<"\n";

	//Apparently dynamic_cast is slow. change to static for release build
	zb_rec_ptr_mk* r=dynamic_cast<zb_rec_ptr_mk*>(*cursor);
	if(r==0)
		r=new zb_rec_ptr_mk(false);
	r->set(this,index);
	rr=r->get_row_ref();
	
	gz_out << p.Get(rr)<<"\n";
	return 0;

}