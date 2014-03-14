
#ifdef ZB_SQLITE

#include "zbase_lib/include/ds_sqlite.h"





bool zb_src_sl3::is_open()
{
	if(!_handle)
		return false;
	return true;


}

zb_status zb_src_sl3::_read_stucture()
{
	//   get_data_test( "SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");

	return zb_ok;
}

zb_status zb_src_sl3::_get_sql_recset_sql(zb_ds_recordset_sl3*&  recset,ctext sqltext)
{
	ZTF;
	recset=new zb_ds_recordset_sl3(this);
	recset->exec_sql(sqltext);

	return zb_ok;
}
zb_status zb_src_sl3::_get_sql_recset(zb_ds_recordset_sl3*&  recset,ctext tbl_name,ctext where_clause)
{

	return zb_ok;
}
zb_st_master* zb_src_sl3::get_tbl_master()
{
	// zb_ds_recordset_sl3* recset=new zb_ds_recordset_sl3(this);
	zb_st_master* tbl= new zb_st_master(this);


	// tbl->_ds_recset
	return 0;
}
zb_table* zb_src_sl3::get_tbl(ctext ds_table_name)
{
	ZTF;
	zb_ds_recordset_sl3* recset=new zb_ds_recordset_sl3(this);

	z_string sqltext="SELECT * FROM ";
	sqltext<<ds_table_name;
	recset->exec_sql(sqltext);


	zb_table* tbl=0;//new zb_table(this);


	//	tbl->create_desc_from_source();


	return tbl;

}
zb_status get_zb_status_sqlite(int sql_status)
{
	return (zb_status)sql_status;

}

zb_src_sl3::zb_src_sl3(): zb_source()
{
	_last_error=SQLITE_OK;
	_handle=0;

}

zb_status zb_src_sl3::open()
{
	ZTF;
	int retval;
	retval = sqlite3_open(_name,&_handle);
	ZT(("opening=%s",_name.c_str()));

	return get_zb_status_sqlite(retval);
}
zb_status zb_src_sl3::close()
{
	ZTF;
	int retval;
	retval =sqlite3_close(_handle);
	return get_zb_status_sqlite(retval);
}

sqlite3* zb_src_sl3::get_handle()
{
	return _handle;
}
ctext zb_src_sl3::get_last_error_msg()
{
	return sqlite3_errmsg(_handle);

}
zb_status zb_src_sl3::dump_master()
{
	int retval;
	retval = sqlite3_open(_path,&_handle);
	return get_zb_status_sqlite(retval);
}
zb_status zb_src_sl3::get_tables()
{

	ZTF;
	zb_status status;
    // select those rows from the table

	zb_ds_recordset_sl3 recset(this);

	status=recset.exec_sql("SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");
	ZT(("status=%s",zb_status_text[status]));
	// Read the number of rows fetched
	int cols = recset.get_num_cols();
	while(1)
	{
		// fetch a row's status
		status = recset.ptr_increment();
    
		if(status == zb_ok)
		{
			// SQLITE_ROW means fetched a row
			const char *name = recset.ptr_get_column_text(1);
			zb_table* tbl= new zb_table(this,name);

			get_table_desc(name,tbl->get_desc());
			
			_tables<< tbl;
		}
		else if(status == zb_end_of_data)
		{
			// All rows finished
			break;
		}
		else
		{
			// Some error encountered
			printf("status=%s",zb_status_text[status]);

			printf("Some error encountered %d\n",status);
			break;
			
		}
	}

	return zb_ok;
}

zb_status zb_src_sl3::get_table_desc(ctext ds_table_name,zb_desc& desc)
{
	ZTF;
	zb_status status;
    // select those rows from the table

	zb_ds_recordset_sl3 recset(this);
	z_string s="PRAGMA table_info('";
	s<<ds_table_name<<"');";

	//status=recset.exec_sql("SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");
	status=recset.exec_sql(s);
	ZT(("status=%s",zb_status_text[status]));

	desc.clear_objs();

	// Read the number of rows fetched
	int cols = recset.get_num_cols();
	//TODO sanity check for cols
	while(1)
	{
		// fetch a row's status
		status = recset.ptr_increment();
    
		if(status == zb_ok)
		{
			// SQLITE_ROW means fetched a row
        
			// sqlite3_column_text returns a const void* , typecast it to const char*
			const char *val = recset.ptr_get_column_text(1); //name of field
			zb_field_string* field=new zb_field_string(0,val);
			desc.add(val,field); 

			//printf("%s = %s\t",recset.get_column_name(col),val);
			//printf("\n");
		}
		else if(status == zb_end_of_data)
		{
			// All rows finished
			break;
		}
		else
		{
			// Some error encountered
			//printf("status=%s",zb_status_text[status]);
			//printf("Some error encountered %d\n",status);
			//TODO!! handle error
			return zb_unknown_data_error;
		}
	}
	return zb_ok;

}
zb_status zb_src_sl3::get_table_info()
{
	ZTF;
	zb_status status;
    // select those rows from the table

	zb_ds_recordset_sl3 recset(this);
	z_string s="PRAGMA table_info('";
	s<<_param_table_name<<"');";

	//status=recset.exec_sql("SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");
	status=recset.exec_sql(s);
	ZT(("status=%s",zb_status_text[status]));


	// Read the number of rows fetched
	int cols = recset.get_num_cols();
	while(1)
	{
		// fetch a row's status
		status = recset.ptr_increment();
    
		if(status == zb_ok)
		{
			// SQLITE_ROW means fetched a row
        
			// sqlite3_column_text returns a const void* , typecast it to const char*
			for(int col=0 ; col<cols;col++)
			{
				const char *val = recset.ptr_get_column_text(col);
				printf("%s = %s\t",recset.get_column_name(col),val);
			}
			printf("\n");
		}
		else if(status == zb_end_of_data)
		{
			// All rows finished
			printf("All rows fetched\n");
			break;
		}
		else
		{
			// Some error encountered
			printf("status=%s",zb_status_text[status]);

			printf("Some error encountered %d\n",status);
			break;
			
		}
	}

	return zb_ok;
}

zb_status zb_ds_recordset_sl3::ds_create_desc_from_source(zb_desc* desc)
{
	ZTF;
	int i;

	int cols = get_num_cols();
	for(i=0;i<cols;i++)
	{
		int int_type=sqlite3_column_type(_stmt,i);
		zb_field *fld=0;
		ctext col_type=ptr_get_column_type(i);
		ctext col_name=get_column_name(i);
		if(col_type)
		{
			if(strcmp(col_type,"integer")==0)
				fld=new zb_field_int32(-1,col_name);

		}
		//if(strcmp(col_type,"text")==0)
		if(!fld)
			fld=new zb_field_string(-1,col_name);

		fld->set_index(i);

		*desc<<fld;

		//ZT(("%s,%s,%d\n",get_column_name(i),,int_type));

	}
   return zb_ok;
}


zb_key_size zb_ds_recordset_sl3::get_num_records()
{
	zb_key_size count=0;
	sqlite3_reset(_stmt);
	int retval=SQLITE_ROW;
	do
	{
		retval= sqlite3_step(_stmt);
		if(retval==SQLITE_ROW) 
			count++;

	} while(retval==SQLITE_ROW);
	return count;

}
zb_status zb_ds_recordset_sl3::ptr_increment()
{
	int retval=sqlite3_step(_stmt);
	if(retval==SQLITE_ROW) 
		return zb_ok;
	if(retval==SQLITE_DONE) 
		return zb_end_of_data;
	return zb_unknown_data_error;
}
int zb_ds_recordset_sl3::get_num_cols()
{
	if(_stmt==0)
		return -1;
	int cols = sqlite3_column_count(_stmt);
	return cols;
}
zb_status zb_ds_recordset_sl3::exec_sql(ctext sqltext)
{
	ZTF;
	int retval;
	if(!_file_sqlite) 
		return zb_data_source_not_open;
	if(!_file_sqlite->is_open()) 
		return zb_data_source_not_open;
	retval = sqlite3_prepare_v2(_file_sqlite->get_handle(),sqltext,-1,&_stmt,0);
	if(0==retval)
		return zb_ok;
	_file_sqlite->_last_error=retval;
	return zb_unknown_data_error;
}
ctext zb_ds_recordset_sl3::get_column_name(int i)
{
	return sqlite3_column_name(_stmt,i);
}
ctext zb_ds_recordset_sl3::ptr_get_column_text(int i)
{
	return (ctext)sqlite3_column_text(_stmt,i);
}
ctext zb_ds_recordset_sl3::ptr_get_column_type(int i)
{
	return (ctext)sqlite3_column_decltype(_stmt,i);
	//int int_type=sqlite3_column_type(_stmt,i);
}

zb_status zb_ds_recordset_sl3::ds_get_val_string(z_string& val,zb_field* field)
{
	val=(ctext)sqlite3_column_text(_stmt,field->get_index());
	return zb_ok;
}
void zb_ds_recordset_sl3::dump(z_file* fp)
{
    // Read the number of rows fetched
    int cols = sqlite3_column_count(_stmt);
	int   retval;
    
    while(1)
    {
        // fetch a row's status
        retval = sqlite3_step(_stmt);
        
        if(retval == SQLITE_ROW)
        {
            // SQLITE_ROW means fetched a row
            
            // sqlite3_column_text returns a const void* , typecast it to const char*
			int col;
            for(col=0 ; col<cols;col++)
            {
                const char *val = (const char*)sqlite3_column_text(_stmt,col);
                printf("%s = %s\t",sqlite3_column_name(_stmt,col),val);
            }
            printf("\n");
        }
        else if(retval == SQLITE_DONE)
        {
            // All rows finished
            printf("All rows fetched\n");
            break;
        }
        else
        {
            // Some error encountered
            printf("Some error encountered %d %s\n",retval,_file_sqlite->get_last_error_msg());
            break;
        }
    }	
}

#endif
/*

act_rtn zb_src_sl3::get_data_test()
{
	int retval;
    // select those rows from the table

	zb_table* tbl=get_tbl("sqlite_master");

	zb_desc* desc=create_desc_from_source();
    if(tbl==0)
    {
        printf("get_tbl Failed\n");
       
    }
	else
	{
		// Read the number of rows fetched
		int cols = recset.get_num_cols();
        
		while(1)
		{
			// fetch a row's status
			retval = recset.ptr_increment();
        
			if(retval == ZB_OK)
			{
				// SQLITE_ROW means fetched a row
            
				// sqlite3_column_text returns a const void* , typecast it to const char*
				for(int col=0 ; col<cols;col++)
				{
					const char *val = recset.ptr_get_column_text(col);
					printf("%s = %s\t",recset.get_column_name(col),val);
				}
				printf("\n");
			}
			else if(retval == ZB_END_OF_DATA)
			{
				// All rows finished
				printf("All rows fetched\n");
				break;
			}
			else
			{
				// Some error encountered
				printf("Some error encountered %d\n",retval);
				break;
				
			}
		}
	}

	return 0;
}

*/