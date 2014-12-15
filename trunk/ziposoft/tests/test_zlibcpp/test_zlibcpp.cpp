
#include "zipolib/include/z_console.h"
#include "zipolib/include/z_filesystem.h"
#include "zipolib/include/z_factory_static.h"



class root
{
public:
	root()
	{
		_p_logger=&get_logger();

	}
	z_string _param_path;
	z_console console;
	z_logger* _p_logger;
	z_status act_dir();
};

ZFACT(root)
{
	ZOBJ(console);
	//ZOBJ_X(zbs,"db",ZFF_PROP,"database");
	ZPOBJ(_p_logger,"log",ZFF_PROP,"Logger");
 	ZACT_XP(act_dir,"dir",ZFF_ACT_DEF,"Directory listing",1,
		ZPARAM_X(_param_path,"path",ZFF_PARAM,"path ")	);


};
z_status root::act_dir()
{



	return zs_ok;

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

