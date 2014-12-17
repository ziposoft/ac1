
#include "zipolib/include/z_console.h"
#include "zipolib/include/z_filesystem.h"
#include "zipolib/include/z_factory_static.h"


class B
{
public:
	B()
	{

	}

};
class A
{
public:
	A()
	{

	}
	B bobpickles;
};




class root
{
public:
	root()
	{
		_p_logger=&get_logger();

	}
	z_string _param_path;
	z_console console;
	A a;
	z_logger* _p_logger;
	z_status act_dir();
	z_status act_delete_dir();
};
ZFACT(A)
{
	ZOBJ(bobpickles);

};	
ZFACT(B)
{

};	
ZFACT(root)
{
	ZOBJ(a);
	ZOBJ(console);
	//ZOBJ_X(zbs,"db",ZFF_PROP,"database");
	ZPOBJ(_p_logger,"log",ZFF_PROP,"Logger");
 	ZACT_XP(act_dir,"dir",ZFF_ACT_DEF,"Directory listing",1,
		ZPARAM_X(_param_path,"path",ZFF_PARAM,"path ")	);
 	ZACT_XP(act_delete_dir,"deldir",ZFF_ACT_DEF,"Directory listing",1,
		ZPARAM(_param_path)	);

};
z_status root::act_dir()
{
	z_directory dir;
	dir.traverse_tree();
	return zs_ok;
}
z_status root::act_delete_dir()
{
	return z_directory_delete_tree(_param_path);
}
/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);
	return 0;
}

