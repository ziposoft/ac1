
#include "zipolib/include/z_console.h"

#include "zbase_lib/include/zipobase_lib.h"


class root
{
public:
	root()
	{
		_p_logger=&gz_logger;


	}
	z_console console;
	z_logger* _p_logger;

	zb_ds_text zbs;
	z_status create();
};
/*
ZFACT(source)
{
	ZACT_XP(addtable,"addtable","desc",1,
		ZPARAM_X(_newtblname,"table_name","Name of new table"));
	ZACT_XP(open,"opendb","desc",1,
		ZPARAM_X(dbname,"db_name","Name of DB to open"));
	ZACT(close);



};
*/
ZFACT(root)
{
	ZOBJ(console);
	ZOBJ_X(zbs,"db",ZFF_PROP,"database");
	ZPOBJ(_p_logger);
 	ZACT_XP(create,"create",0,"create",0,0);

	/*
	ZPROP(x);
	ZPROP(i);
	ZACT(add);
	*/

};

/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger));
/*  ZP_MOD(zipobase) ,*/




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
			fld=p_ds->ds_field_string_new("field1str");
			if(!fld)
				break;
			tbl->field_add(fld);
		}
		fld2=tbl->get_desc().get_ds_field("field2str");
 		if(!fld2)
		{
			fld2=p_ds->ds_field_string_new("field2str");
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
					gz_out << "get_record_by_index failed:"<<i<<"\n";
					break;
				}
				if(!ptr)
				{
					gz_out << "could not get record"<<i<<"\n";
					break;
				}

				fld->get_string(ptr,data);
				gz_out <<  "record"<<i<<":"<< data <<"\n";
			}
		}
		pRec=p_ds->record_solo_new();
		
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


int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);
	return 0;
}

