
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
	ZOBJ(zbs);
	ZPOBJ(_p_logger);

	/*
	ZPROP(x);
	ZPROP(i);
	ZACT(add);
	ZACT_XP(show,"show","desc",1,ZPARAM(s));
	*/

};

/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger));
/*  ZP_MOD(zipobase) ,*/



#if 0
ZFACT(root)
{
	ZOBJ(console);
	ZOBJ(dbtext);
	ZPOBJ(_p_logger);

	/*
	ZPROP(x);
	ZPROP(i);
	ZACT(add);
	ZACT_XP(show,"show","desc",1,ZPARAM(s));
	*/

};
int test_ds_table(zb_source* p_ds)
{
	z_status status;
	zb_ds_rec_ptr* pRec=0;
	zb_ds_field* fld=0;
	zb_ds_field* fld2=0;
	zb_ds_rec_ptr* ptr=0;
	z_string data;
	int i;
	do
	{

		tbl=  p_ds->ds_table_new("table1");
		if(!tbl)
			break;		
		fld=p_ds->ds_field_string_new("field1str");
		if(!fld)
			break;
		fld2=p_ds->ds_field_string_new("field2str");
		if(!fld2)
			break;

		tbl->get_desc() ,fld,fld2;
		status=p_ds->open(true,true);
		if(status)
			break;
		status=tbl->open(true);
		if(status)
			break;
		
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
	if(tbl)
		delete tbl;
	return 0;
}
#endif


int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.init(argv[0]);
	o.console.loadcfg();
	o.console.runapp(argc,argv);
	o.console.savecfg();


	return 0;
}

