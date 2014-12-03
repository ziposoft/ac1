#include "zb_pch.h"

#include "zbase_lib/include/datasource.h"


zb_ds_field* zb_ds_desc::get_ds_field(ctext name)
{
	return (*this)[name];

}


/*__________________________________________________________________________

	zb_ds_field
____________________________________________________________________________*/
z_status zb_ds_field::set_string(zb_ds_rec_ptr *rec,ctext s){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::get_string(zb_ds_rec_ptr *rec,z_string& s){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::set_int32(zb_ds_rec_ptr *rec,I32 i){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::get_int32(zb_ds_rec_ptr *rec,I32& i){ return Z_ERROR_NOT_IMPLEMENTED;}
z_status zb_ds_field::get_name(z_string& s)
{
	s=_id;
	return 0;
}




/*____________________________________________________________________________
	zb_ds_rec_ptr
____________________________________________________________________________*/


/*____________________________________________________________________________
	zb_ds_rec_ptr
____________________________________________________________________________*/
 zb_ds_rec_ptr::zb_ds_rec_ptr()
{


}
void zb_ds_rec_ptr::set(zb_ds_table* rs,size_t index)
{
	  Z_ERROR_NOT_IMPLEMENTED;

}

/*__________________________________________________________________________

	zb_ds_table
____________________________________________________________________________*/

/* Interface */
 z_status zb_ds_table::act_record_add()
 {
	zb_ds_field* fld=0;
	zb_ds_desc &desc=get_desc();
	zb_ds_rec_ptr* pRec=0;
	z_map_iter fld_iter;
	z_status status;
	int count_fields=0;
	status=open(true);
	if(! status)
		return status;


	pRec=_ds->record_solo_new();
	while (fld=desc.get_next( fld_iter))
	{
		z_string data;
		if(	_param_new_data.size()>	count_fields)
		{
			data= _param_new_data[count_fields];
		}
		status=fld->set_string(pRec,data);
		count_fields++;
	}
	status=record_add(pRec);
	return status;
 }

 z_status zb_ds_table::act_record_del()
 {
	  return delete_record_by_index(_param_record_index);
 }
 z_status zb_ds_table::act_dump_records()
{
	z_status status;
	zb_ds_rec_ptr* pRec=0;
	zb_ds_field* fld=0;
	z_string data;
	zb_ds_desc &desc=get_desc();
	z_map_iter fld_iter;
	size_t i;
	open(false);
	size_t count=get_record_count();

	printf("count=%d\n",count);

	zout <<"\t";


	fld_iter.reset();
	while (fld=desc.get_next( fld_iter))
	{
		fld->get_name(data);
		zout <<  data << "\t";
	}
	zout <<'\n';
	for (i=0;i<count;i++)
	{
		status=get_record_by_index(i,&pRec);
		if(status)
		{
			zout << "get_record_by_index failed:"<<i<<"\n";
			break;
		}
		if(!pRec)
		{
			zout << "could not get record"<<i<<"\n";
			break;
		}
		zout << (int)i<<":";
		fld_iter.reset();
		while (fld=desc.get_next( fld_iter))
		{
			fld->get_string(pRec,data);
			zout <<  data << "\t";
		}
		zout <<'\n';

	}
	return zs_ok;


}

z_status zb_ds_table::act_add_field()
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}



/* Virtual funcs */

z_status zb_ds_table::record_add(zb_ds_rec_ptr* rec)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
z_status zb_ds_table::open(bool writable)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}

size_t zb_ds_table::get_record_count()
{
	Z_ERROR_NOT_IMPLEMENTED;
	 return 0;
}
z_status zb_ds_table::get_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
z_status zb_ds_table::test_record_by_index(size_t index,zb_ds_rec_ptr** cursor)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
z_status zb_ds_table::delete_record_by_index(size_t index)
{
	 return Z_ERROR_NOT_IMPLEMENTED;
}
ctext zb_ds_table::get_map_key()
{
	return _id;
}
zb_ds_table::zb_ds_table(zb_source* ds,ctext unique_id)
{
	_ds=ds;
	_id=unique_id;
	_status=status_closed;
}
zb_ds_table::zb_ds_table()
{
	_ds=0;
	Z_ASSERT(0);
}

z_status zb_ds_table::field_add(zb_ds_field* fld)
{
	Z_ASSERT(fld);
	if(	 _ds_desc.get_ds_field(fld->_id))
		return zs_already_exists;


	_ds_desc<<fld;
	 return 0;

}


/*__________________________________________________________________________

	zb_source
____________________________________________________________________________*/

zb_source::zb_source()
{
	_name="";
	_status=status_closed;
}
bool zb_source::is_open()
{ 

	Z_TODO;
	return false;
}

z_status zb_source::commit()
{
	z_map_iter i;
	zb_ds_table* t;
	while(t=_ds_tables.get_next(i))
	{
		t->commit();

	}
	return zs_ok;

}

z_status zb_source::ds_table_new(ctext ds_table_name,zb_ds_table*& tbl)
{
	tbl=_ds_tables.get(ds_table_name);
	if(tbl)
	{
		return Z_ERROR(zs_already_exists);
	}

	z_status status=_table_new(ds_table_name,tbl);
	if(status)
		return 	status;

	_ds_tables<<tbl;
	return zs_success;
}
z_status zb_source::ds_table_get(ctext ds_table_name,zb_ds_table*& tbl)
{ 
	tbl=_ds_tables.get(ds_table_name);
	if(!tbl)
		return Z_ERROR(zs_not_found);
	
	return zs_success;
};

z_status zb_source::act_open()
{
	zb_ds_table* tbl=0;
	if(	_param_db_name=="")
	{
		Z_ERROR_MSG(zs_bad_parameter,"You must specify a DB name");
	}

	return	 open(_param_db_name,true,true);
}



z_status zb_source::act_table_new()
{
	zb_ds_table* tbl=0;
	if(	_param_table_new_name=="")
	{
		Z_ERROR_MSG(zs_bad_parameter,"You must specify a table name");
	}


	return ds_table_new(_param_table_new_name,tbl);
}


