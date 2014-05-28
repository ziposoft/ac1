// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_console.h"


class z_console_ntf : public z_console
{
public:
	z_console_ntf()
	{

	}
 	z_console_ntf(ctext exe_name)
	{
		_config_file= exe_name;
		_config_file<<".cfg";


	}
	virtual z_status OnExecuteLine(ctext text);
	zf_obj _root;
	zf_obj _self;
	zf_obj _selected;
	zf_obj _temp;

	template <class CLASS> void run_T(CLASS * obj)
	{
		_root._obj=obj;
		_root._fact=&z_factory_T<CLASS>::self;
		_selected= _root;
		_self._fact=&z_factory_T<z_console_ntf>::self;
		_self._obj=this;
		run();
	}

	z_status evaluate_feature(zf_obj& o);
 	z_status navigate_to_obj();
 	z_status select_obj(ctext name);

	//command line props
	z_string _script_file;
	z_string _config_file;
	z_string _startup_path;

	//command line functions
	z_status list_features();
	z_status dumpcfg();
	z_status loadcfg();
	z_status savecfg();
	z_status help();
	z_status shell();
	z_status exit();

};


