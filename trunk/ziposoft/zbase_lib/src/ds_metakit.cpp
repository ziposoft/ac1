#include "zb_pch.h"

#include "zbase_lib/include/ds_metakit.h"

//============================================================
// Metakit Data Source 


zb_ds_metakit::zb_ds_metakit(ctext name) : zb_source( name)
{
	_pStore=0;
	_filename<<name <<".mtk";

}
zb_ds_metakit:: ~zb_ds_metakit()
{
    if (_pStore)
	{   delete _pStore;  }

}


zb_ds_field* zb_ds_metakit::get_ds_field_string(ctext id)
{
	return new zb_ds_field_mk_string(id);
}

z_status zb_ds_metakit::GetViewString(zb_desc& desc,z_string &str,bool blocked)
{
	if(blocked) str+="[_B";
	str+="[";

	z_map_iter i;
	zb_field* f=0;
	bool comma=false;
	while(f=desc.get_next(i))
	{
        if(comma) str+=",";
		zb_ds_field_mk * mkf=(zb_ds_field_mk*)f->get_ds_field();

        mkf->MakeDesc(str);
		comma=true;
	}
    str+="]";
	if(blocked) str+="]";

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
//============================================================
// Metakit Fields


zb_ds_field_mk::zb_ds_field_mk(ctext id)
{
	_id=id;

}

zb_ds_field_mk_string::zb_ds_field_mk_string(ctext id) :zb_ds_field_mk(id)
{
	_pStrProp=new c4_StringProp(id);

}

//============================================================
// Metakit Recordsets


zb_status tblTopLevel::GetDataView(c4_View& view)
{
	
	//Z_ASSERT(GetFile()->is_open());
	if(!GetFile()->is_open()) ZB_TRACE_RETURN(zb_status_data_error);
	if(_status==statusGotData) return zb_status_ok;
	if(_status==statusError) ZB_TRACE_RETURN(zb_status_data_error);
    z_string str="T";
    str+=GetTableKey();
	GetDesc().GetViewString(str,_use_blocked_view);
	ZTRACE_OUT(TR_TABLES,("Table# %x, %s, viewstr=%s", GetTableKey(),GetNameOfTable(),str.c_str()));	

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
		view=GetFile()->get_mk_view(str); //TODO catch assertions HERE, for corrupt databases.

#endif
	}

	catch(...)
	{
		_status=statusError;
		ZB_THROW_ERROR(zb_status_data_error,("Error accessing %s",GetNameOfTable() ));
		//Z_ASSERT(0);
		ZB_TRACE_RETURN(zb_status_data_error);

	}
	_status=statusGotData;
	return zb_status_ok;
}