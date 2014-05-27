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
	void* _obj_root;
	void* _obj_current;
	const z_factory* _fact_self;
	const z_factory* _fact_root;
	const z_factory* _fact_current;
	void run(const z_factory* f,void * obj);

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


