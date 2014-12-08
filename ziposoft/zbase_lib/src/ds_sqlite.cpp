#include "zb_pch.h"
#include "zbase_lib/include/zb.h"

#ifdef ZB_INCLUDE_DS_SQLITE

#include "zbase_lib/include/ds_sqlite.h"

zb_ds_sl3::zb_ds_sl3() : zb_source()
{
	_last_error=SQLITE_OK;
	_handle=0;
}


bool zb_ds_sl3::is_open()
{
	if(!_handle)
		return false;
	return true;


}

z_status zb_ds_sl3::_read_stucture()
{
	//   get_data_test( "SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");

	return zs_ok;
}
/*
z_status zb_ds_sl3::_get_sql_recset_sql(zb_ds_table_sl3*&  recset,ctext sqltext)
{
	ZTF;
	recset=z_new zb_ds_table_sl3(this);
	recset->exec_sql(sqltext);

	return zs_ok;
}

*/
z_status zb_ds_sl3::_get_sql_recset(zb_ds_table_sl3*&  recset,ctext tbl_name,ctext where_clause)
{

	return zs_ok;
}
zb_st_master* zb_ds_sl3::get_tbl_master()
{
	// zb_ds_table_sl3* recset=z_new zb_ds_table_sl3(this);
	zb_st_master* tbl= z_new zb_st_master(this);


	// tbl->_ds_recset
	return 0;
}
zb_ds_table* zb_ds_sl3::get_tbl(ctext ds_table_name)
{
	ZTF;
	zb_ds_table_sl3* recset=z_new zb_ds_table_sl3(this,ds_table_name);

	z_string sqltext="SELECT * FROM ";
	sqltext<<ds_table_name;
	recset->exec_sql(sqltext);


	zb_ds_table* tbl=0;//z_new zb_table(this);


	//	tbl->create_desc_from_source();


	return tbl;

}
z_status get_zb_status_sqlite(int sql_status)
{
	return (z_status)sql_status;

}


z_status zb_ds_sl3::open(ctext  path)
{
	ZTF;
	_path=path;

	int retval;
	retval = sqlite3_open(_name,&_handle);
	ZT("opening=%s",_name.c_str());

	return get_zb_status_sqlite(retval);
}
z_status zb_ds_sl3::close()
{
	ZTF;
	int retval;
	retval =sqlite3_close(_handle);
	return get_zb_status_sqlite(retval);
}

sqlite3* zb_ds_sl3::get_handle()
{
	return _handle;
}
ctext zb_ds_sl3::get_last_error_msg()
{
	return sqlite3_errmsg(_handle);

}
z_status zb_ds_sl3::dump_master()
{
	int retval;
	retval = sqlite3_open(_path,&_handle);
	return get_zb_status_sqlite(retval);
}
z_status zb_ds_sl3::get_tables()
{

	ZTF;
	z_status status;
    // select those rows from the table

	zb_ds_table_sl3 recset(this,"sqlite_master");

	status=recset.exec_sql("SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");
	ZT("status=%s",zs_get_status_text(status));
	// Read the number of rows fetched
	int cols = recset.get_num_cols();
	while(1)
	{
		// fetch a row's status
		status = recset.ptr_increment();
    
		if(status == zs_ok)
		{
			// SQLITE_ROW means fetched a row
			const char *name = recset.ptr_get_column_text(1);
			zb_ds_table* tbl= z_new zb_ds_table(this,name);
			zb_desc desc; //TODO!!

			get_table_desc(name,desc);
			
			_ds_tables<< tbl;
		}
		else if(status == zs_end_of_list)
		{
			// All rows finished
			break;
		}
		else
		{
			// Some error encountered
			printf("status=%s",zs_get_status_text(status));

			printf("Some error encountered %d\n",status);
			break;
			
		}
	}

	return zs_ok;
}

z_status zb_ds_sl3::get_table_desc(ctext ds_table_name,zb_desc& desc)
{
	ZTF;
	z_status status;
    // select those rows from the table

	zb_ds_table_sl3 recset(this,"ds_table_name");
	z_string s="PRAGMA table_info('";
	s<<ds_table_name<<"');";

	//status=recset.exec_sql("SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");
	status=recset.exec_sql(s);
	ZT("status=%s",zs_get_status_text(status));

	desc.clear();

	// Read the number of rows fetched
	int cols = recset.get_num_cols();
	//TODO sanity check for cols
	while(1)
	{
		// fetch a row's status
		status = recset.ptr_increment();
    
		if(status == zs_ok)
		{
			// SQLITE_ROW means fetched a row
        
			// sqlite3_column_text returns a const void* , typecast it to const char*
			const char *val = recset.ptr_get_column_text(1); //name of field
			zb_field_string* field=z_new zb_field_string(this,0,val);
			desc.add(val,field); 

			//printf("%s = %s\t",recset.get_column_name(col),val);
			//printf("\n");
		}
		else if(status == zs_end_of_list)
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
			return zs_data_error;
		}
	}
	return zs_ok;

}
/*
z_status zb_ds_sl3::get_table_info()
{
	ZTF;
	z_status status;
    // select those rows from the table

	zb_ds_table_sl3 recset(this);
	z_string s="PRAGMA table_info('";
	s<<_param_table_name<<"');";

	//status=recset.exec_sql("SELECT * FROM sqlite_master WHERE type='table' ORDER BY name;");
	status=recset.exec_sql(s);
	ZT("status=%s",zb_status_text[status]);


	// Read the number of rows fetched
	int cols = recset.get_num_cols();
	while(1)
	{
		// fetch a row's status
		status = recset.ptr_increment();
    
		if(status == zs_ok)
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
		else if(status == zs_end_of_list)
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

	return zs_ok;
}*/
zb_ds_table_sl3::zb_ds_table_sl3(zb_ds_sl3* ds,ctext unique_id) :zb_ds_table(ds,unique_id)
{
	_stmt=0;
	_file_sqlite=ds;
}
z_status zb_ds_table_sl3::ds_create_desc_from_source(zb_desc* desc)
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
				fld=z_new zb_field_int32(-1,col_name);

		}
		//if(strcmp(col_type,"text")==0)
		if(!fld)
			fld=z_new zb_field_string(this->_file_sqlite,-1,col_name);

		fld->set_index(i);

		*desc<<fld;

		//ZT("%s,%s,%d\n",get_column_name(i),,int_type);

	}
   return zs_ok;
}


zb_key_size zb_ds_table_sl3::get_num_records()
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
z_status zb_ds_table_sl3::ptr_increment()
{
	int retval=sqlite3_step(_stmt);
	if(retval==SQLITE_ROW) 
		return zs_ok;
	if(retval==SQLITE_DONE) 
		return zs_end_of_list;
	return zs_data_error;
}
int zb_ds_table_sl3::get_num_cols()
{
	if(_stmt==0)
		return -1;
	int cols = sqlite3_column_count(_stmt);
	return cols;
}
z_status zb_ds_table_sl3::exec_sql(ctext sqltext)
{
	ZTF;
	int retval;
	if(!_file_sqlite) 
		return zs_not_open;
	if(!_file_sqlite->is_open()) 
		return zs_not_open;
	retval = sqlite3_prepare_v2(_file_sqlite->get_handle(),sqltext,-1,&_stmt,0);
	if(0==retval)
		return zs_ok;
	_file_sqlite->_last_error=retval;
	return zs_data_error;
}
ctext zb_ds_table_sl3::get_column_name(int i)
{
	return sqlite3_column_name(_stmt,i);
}
ctext zb_ds_table_sl3::ptr_get_column_text(int i)
{
	return (ctext)sqlite3_column_text(_stmt,i);
}
ctext zb_ds_table_sl3::ptr_get_column_type(int i)
{
	return (ctext)sqlite3_column_decltype(_stmt,i);
	//int int_type=sqlite3_column_type(_stmt,i);
}

z_status zb_ds_table_sl3::ds_get_val_string(z_string& val,zb_field* field)
{
	val=(ctext)sqlite3_column_text(_stmt,field->get_index());
	return zs_ok;
}
void zb_ds_table_sl3::dump(z_file* fp)
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

act_rtn zb_ds_sl3::get_data_test()
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
        
			if(retval == zs_ok)
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

