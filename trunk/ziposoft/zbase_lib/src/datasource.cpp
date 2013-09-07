#include "zbase_lib/include/datasource.h"
#include "zbase_lib/include/record.h"



zb_source::zb_source()
{


}
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

zb_status zb_src_sl3::_get_sql_recset_sql(zb_src_set_sl3*&  recset,ctext sqltext)
{
	ZTF;
	recset=new zb_src_set_sl3(this);
	recset->exec_sql(sqltext);

	return zb_ok;
}
zb_status zb_src_sl3::_get_sql_recset(zb_src_set_sl3*&  recset,ctext tbl_name,ctext where_clause)
{

	return zb_ok;
}
zb_st_master* zb_src_sl3::get_tbl_master()
{
	// zb_src_set_sl3* recset=new zb_src_set_sl3(this);
	zb_st_master* tbl= new zb_st_master(this);


	// tbl->_ds_recset
	return 0;
}
zb_table* zb_src_sl3::get_tbl(ctext ds_table_name)
{
	ZTF;
	zb_src_set_sl3* recset=new zb_src_set_sl3(this);

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

	zb_src_set_sl3 recset(this);

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

	zb_src_set_sl3 recset(this);
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

	zb_src_set_sl3 recset(this);
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