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


zb_ds_field* zb_ds_metakit::get_ds_field_string(ctext id)
{
	return new zb_ds_field_mk_string(id);
}



z_status zb_ds_metakit::open(bool writable) 
{
	_pStore= new c4_Storage(_filename,(writable?1:0));
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


z_status zb_ds_metakit::_get_view_for_table(c4_View& view,zb_table_base* tbl)
{
	


	//Z_ASSERT(GetFile()->is_open());
	/*
	if(!GetFile()->is_open()) Z_ERROR_RETURN(zb_status_data_error);
	if(_status==statusGotData) return zb_status_ok;
	if(_status==statusError) ZB_TRACE_RETURN(zb_status_data_error);

	*/

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
	_pStrProp=new c4_StringProp(id);

}
zb_ds_field_mk_string::~zb_ds_field_mk_string()
{
}


/*____________________________________________________________________________

	 Metakit Recordsets
____________________________________________________________________________*/


