
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


	}
	z_string _param_db_name;
	int  _param_db_type;
	z_console console;
	z_logger* _p_logger;
	z_obj_map<zb_source> ds;
	z_status opends();


	int _param_test_size;
	int _param_data_size;
	z_status testsort();
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
char get_rand_char()
{
	int r=rand()%26;
	r=r+'A';
	return (char)r;
}


z_status root::testsort()
{
	int i;
	char buf[13];
	std::map<z_string,U32> map_sort;
	std::vector<z_string> vect_data;
	for (i = 0; i < _param_test_size; i++)
	{
		int len=rand()%10+1;
		int j;
		for(j=0;j<len;j++)
			buf[j]=get_rand_char();
		buf[j]=0;

		vect_data.push_back(buf);
	}
	zout <<"created...\n";
	for (i = 0; i < _param_test_size; i++)
	{

		map_sort.emplace(vect_data[i],i);

	}
	i=0;
	for (auto& x: map_sort)
	{
		zout << " [" <<x.first << ':' << x.second << "]\n";
		if(i++>10)
			break;
	}
	return zs_ok;



}
int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);
	return 0;
}

