// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_console.h"


class z_console_ntf : public z_console
{
public:
	z_console_ntf()
	{
		_self._fact=&z_factory_T<z_console_ntf>::self;
		_self._obj=this;
		_selected= _self;
		_root._fact=0;
		_root._obj=0;
		_config_file="console.cfg";
		_dump_cmd_line=false;
	}
 	z_status load_config_file(ctext exe_name)
	{
		_config_file= exe_name;
		_config_file<<".cfg";
		loadcfg();
	}
    virtual void OnDoubleBack();
	virtual z_status ExecuteLine(ctext text);
	virtual z_status EvaluateLine(ctext text);
    virtual void OnTab();


	zf_obj _root;
	zf_obj _self;
	zf_obj _selected;
	zf_obj _temp;
	z_string _temp_path;
	bool _has_feature;


	template <class CLASS> void setroot(CLASS * obj)
	{
		_root._obj=obj;
		_root._fact=&z_factory_T<CLASS>::self;
		_selected= _root;

	}

	z_status evaluate_feature(zf_obj& o);
 	z_status navigate_to_obj();
 	z_status select_obj(zp_feature* zpf);
 	z_status select_obj_from_path(zf_obj& start,z_string& path);
	//void get_auto_complete_list(z_string& partial);

	//command line props
	bool _dump_cmd_line;
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
	z_status testreadall();

};


