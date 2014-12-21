
#include "zipolib/include/z_console.h"

#include "zbase_lib/include/zipobase.h"


class root
{
public:
	root()
	{
		_p_logger=&z_logger_get();
		_param_db_type=0;


	}
	z_string _param_db_name;
	int  _param_db_type;
	z_console console;
	z_logger* _p_logger;
	z_obj_map<zb_source> ds;
	z_status opends();
};
/*
ZFACT(source)
{
	ZACT_XP(addtable,"addtable","desc",ZFF_ACT_DEF,
		ZPARAM_X(_newtblname,"table_name","Name of new table"));
	ZACT_XP(open,"opendb","desc",1,
		ZPARAM_X(dbname,"db_name","Name of DB to open"));
	ZACT(close);



};
*/
ZFACT(root)
{
	ZOBJ(console);
	//ZOBJ_X(zbs,"db",ZFF_PROP,"database");
	ZPOBJ(_p_logger,"log",ZFF_PROP,"Logger");
 	ZACT_XP(opends,"open",ZFF_ACT_DEF,"open",2,
		ZPARAM_X(_param_db_name,"name",ZFF_PARAM,"Name of new database"),
		ZPARAM_X(_param_db_type,"type",ZFF_PARAM,"Type of new database")		);
	ZPROP(ds);
	/*
	ZPROP(x);
	ZACT(add);
	*/

};

/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger));
/*  ZP_MOD(zipobase) ,*/


z_status root::opends()
{
	zb_source* d=ds.get(	 _param_db_name);
	if(d)
		return d->create_or_open();

	z_string path;
	z_filesys_getcwd(path);
	path<<'/'<<	 _param_db_name;
	z_status s=zb_datasource_create((type_ds_type)_param_db_type,path,d);
	if(s)
		return s;
		ds<<d;
	return s;
}
#if 0
z_status root::create()
{
	z_status status;
	zb_ds_rec_ptr* pRec=0;
	zb_ds_field* fld=0;
	zb_ds_field* fld2=0;
	zb_ds_rec_ptr* ptr=0;
	z_string data;
	zb_source* p_ds=& zbs;
	zb_ds_table* tbl;
	int i;
	do
	{

		status=p_ds->open("datasource",true,true);
		if(status)
			break;

		status=p_ds->ds_table_get("table1",tbl);

		if( status)
		{
			status=  p_ds->ds_table_new("table1",tbl);
		}

		tbl->open(true);
		if(status)
			break;
		fld=tbl->get_desc().get_ds_field("field1str");
 		if(!fld)
		{
			fld=tbl->ds_field_string_new("field1str");
			if(!fld)
				break;
			tbl->field_add(fld);
		}
		fld2=tbl->get_desc().get_ds_field("field2str");
 		if(!fld2)
		{
			fld2=tbl->ds_field_string_new("field2str");
			if(!fld2)
				break;
			tbl->field_add(fld2);
		}


		
		size_t count=tbl->get_record_count();

		printf("count=%d\n",count);

		for (i=0;i<3;i++)
		{
			if(count>i)
			{
				status=tbl->get_record_by_index(i,&ptr);
				if(status)
				{
					zout << "get_record_by_index failed:"<<i<<"\n";
					break;
				}
				if(!ptr)
				{
					zout << "could not get record"<<i<<"\n";
					break;
				}

				fld->get_string(ptr,data);
				zout <<  "record"<<i<<":"<< data <<"\n";
			}
		}
		pRec=tbl->record_solo_new();
		
		if(!pRec)
			break;
		data="record number";
		data <<count;
		status=fld->set_string(pRec,data);
		if(status)
			break;
		status=fld2->set_string(pRec,"teabag");
		if(status)
			break;
		status=tbl->record_add(pRec);
		if(status)
			break;
		status=p_ds->commit();
		
	}while(0);
	
	p_ds->close();
	if(pRec)
		delete pRec;
	if(fld)
		delete fld;
	//if(tbl)
		//delete tbl;
	return 0;
}


#endif

int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);
	return 0;
}

