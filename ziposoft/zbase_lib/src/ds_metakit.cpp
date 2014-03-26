#include "zb_pch.h"

#include "zbase_lib/include/ds_metakit.h"



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