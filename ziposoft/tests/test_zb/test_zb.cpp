
#include "zipolib/include/z_console.h"

#include "zbase_lib/include/zipobase.h"


class root
{
public:
	root()
	{
		_p_logger=&z_logger_get();
		_param_db_type=0;
		_param_test_size=10000;
		_param_data_size=10;
		_param_dump_map=false;


	}
	z_string _param_db_name;
	int  _param_db_type;
	z_console console;
	z_logger* _p_logger;
	z_obj_map<zb_source> ds;
	z_status opends();
	z_status act_ds_new();


	int _param_test_size;
	int _param_data_size;
	bool _param_dump_map;


 	std::vector<z_string> _vect_data;
	z_status createvect();


	z_status testmatch();
	z_status testsort();
	z_status testsort2();
	z_status testsort3();
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
	ZACT_XP(testsort,"testsort",ZFF_ACT_DEF,"testsort",0);
	ZACT(createvect);
	ZACT(testsort2);
	ZACT(testmatch);
	ZACT(testsort3);
	ZACT(act_ds_new);
	ZACT_XP(opends,"open",ZFF_ACT_DEF,"open",2,
		ZPARAM_X(_param_db_name,"name",ZFF_PARAM,"Name of new database"),
		ZPARAM_X(_param_db_type,"type",ZFF_PARAM,"Type of new database")		);
	ZPROP(ds);
	ZPROP(_param_test_size);
	/*
	ZPROP(x);
	ZACT(add);
	*/

};

/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger), ZP_MOD(container) );
/*  ZP_MOD(zipobase) ,*/

z_status root::act_ds_new()
{
	zb_source* d=ds.get(	 _param_db_name);
	if(d)
	{
		d->close();
		ds.del(_param_db_name);

	}

	z_string path;
	z_filesys_getcwd(path);
	path<<'/'<<	 _param_db_name;
	z_status s=zb_datasource_create((type_ds_type)_param_db_type,path,d);
	if(s)
		return s;
	ds<<d;
	return s;
}
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
char get_rand_char()
{
	int r=rand()%26;
	r=r+'A';
	return (char)r;
}

z_status root::createvect()
{
	int i;
	char buf[13];
	std::map<z_string,U32> map_sort;
	zout <<"reserving...\n";

	_vect_data.reserve(_param_test_size);

	for (i = 0; i < _param_test_size; i++)
	{
		int len=rand()%10+1;
		int j;
		for(j=0;j<len;j++)
			buf[j]=get_rand_char();
		buf[j]=0;

		_vect_data.push_back(buf);
	}
	zout <<"created...\n";


	return zs_ok;

}
z_status root::testsort()
{
	int i;
	std::map<z_string,U32> map_sort;
	std::map<z_string,U32>::iterator iter;
	for (i = 0; i < _param_test_size; i++)
	{

		map_sort[_vect_data[i]]=i;

	}
	i=0;
	if(_param_dump_map)
	for (auto& x: map_sort)
	{
		zout << " [" <<x.first << ':' << x.second << "]\n";
		if(i++>10)
			break;
	}
	char lookup[2];
	lookup[1]=0;
	int dummy=0;
	char *x=&lookup[0];
		z_time time;
		z_set_start_time(&time);

	for(i=0;i<_param_test_size;i++)
	{
 		for(*x='A';*x<'Z';(*x)++)
		{
			iter=map_sort.find(lookup);
			if(iter!=map_sort.end())
			{
				dummy++;
				//printf("%s=%d\n",lookup,iter->second);
			}
		}
	}
	printf("\n dummy=%d %.3lf ms\n",dummy,z_get_elapsed_time_ms_fp( &time));
	return zs_ok;
	return zs_ok;

}


z_status root::testsort2()
{
	int i;
	z_stl_obj_map<U32> map_sort;
	z_stl_obj_map<U32>::iterator iter;
	for (i = 0; i < _param_test_size; i++)
	{

		map_sort[_vect_data[i]]=i;

	}
	i=0;
	if(_param_dump_map)
	for (auto& x: map_sort)
	{
		zout << " [" <<x.first << ':' << x.second << "]\n";
		if(i++>10)
			break;
	}
	char lookup[2];
	lookup[1]=0;
	int dummy=0;
	char *x=&lookup[0];
		z_time time;
		z_set_start_time(&time);

	for(i=0;i<_param_test_size;i++)
	{
 		for(*x='A';*x<'Z';(*x)++)
		{
			iter=map_sort.find(lookup);
			if(iter!=map_sort.end())
			{
				dummy++;
				//printf("%s=%d\n",lookup,iter->second);
			}
		}
	}
	printf("\n dummy=%d %.3lf ms\n",dummy,z_get_elapsed_time_ms_fp( &time));
	return zs_ok;

}
template <class ITEM_CLASS > class map_ctext 
: public std::map<ctext,ITEM_CLASS,ctext_less_than> 
{

};

z_status root::testsort3()
{
	int i;
	map_ctext<U32> map_sort;
	map_ctext<U32>::iterator iter;
	for (i = 0; i < _param_test_size; i++)
	{
		map_sort[_vect_data[i]]=i;
	}
	i=0;
	if(_param_dump_map)
	for (auto& x: map_sort)
	{
		zout << " [" <<x.first << ':' << x.second << "]\n";
		if(i++>10)
			break;
	}
	char lookup[2];
	lookup[1]=0;
	int dummy=0;
	char *x=&lookup[0];
	for(i=0;i<_param_test_size;i++)
	{
 		for(*x='A';*x<'Z';(*x)++)
		{
			iter=map_sort.find(lookup);
			if(iter!=map_sort.end())
			{
				dummy++;
				//printf("%s=%d\n",lookup,iter->second);
			}
		}
	}

	return zs_ok;

}
z_status root::testmatch()
{



}

int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);
	return 0;
}

