#include "zbase_lib/include/record.h"
#include "zbase_lib/include/datasource.h"


zb_status zb_recset::create_desc_from_source()
{
	return _ds_recset->ds_create_desc_from_source(&_desc);

}
zb_field* zb_recset::get_field(ctext name)
{
	//if(!_pdesc)  		return 0;
	return _desc.get_field( name);
}


zb_status zb_recset::get_val_string(z_string& val,zb_field* field)
{
	return _ds_recset->ds_get_val_string(val,field);
}

zb_status zb_recset::ptr_increment()
{
	return _ds_recset->ptr_increment();
}

#ifdef ZB_SQLITE

zb_status zb_src_set_sl3::ds_create_desc_from_source(zb_desc* desc)
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


zb_key_size zb_src_set_sl3::get_num_records()
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
zb_status zb_src_set_sl3::ptr_increment()
{
	int retval=sqlite3_step(_stmt);
	if(retval==SQLITE_ROW) 
		return zb_ok;
	if(retval==SQLITE_DONE) 
		return zb_end_of_data;
	return zb_unknown_data_error;
}
int zb_src_set_sl3::get_num_cols()
{
	if(_stmt==0)
		return -1;
	int cols = sqlite3_column_count(_stmt);
	return cols;
}
zb_status zb_src_set_sl3::exec_sql(ctext sqltext)
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
ctext zb_src_set_sl3::get_column_name(int i)
{
	return sqlite3_column_name(_stmt,i);
}
ctext zb_src_set_sl3::ptr_get_column_text(int i)
{
	return (ctext)sqlite3_column_text(_stmt,i);
}
ctext zb_src_set_sl3::ptr_get_column_type(int i)
{
	return (ctext)sqlite3_column_decltype(_stmt,i);
	//int int_type=sqlite3_column_type(_stmt,i);
}

zb_status zb_src_set_sl3::ds_get_val_string(z_string& val,zb_field* field)
{
	val=(ctext)sqlite3_column_text(_stmt,field->get_index());
	return zb_ok;
}
void zb_src_set_sl3::dump(z_file* fp)
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


