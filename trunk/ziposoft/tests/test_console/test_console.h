// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_console.h"
#include "zipolib/include/z_parse_text.h"


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

/*	
	NEW METHOD
*/
	virtual z_status ExecuteLine(ctext text);
	virtual z_status EvaluateLine(ctext  text);
	z_string _cmd_line_feature;
	z_string _cmd_line_feature_index;
	bool _has_path;
	z_status evaluate_feature(zf_obj& o);
	z_status get_feature_and_index();
	z_status select_obj();
    virtual void OnTab();
	zp_text_parser _tparser;

/*	
	OLD METHOD
*/
	virtual z_status ExecuteLine_old(ctext text);
	virtual z_status EvaluateLine_old(ctext text);
	z_status evaluate_feature_old(zf_obj& o);
 	z_status select_obj_old(zp_feature* zpf);

    virtual void OnTab_old();
 	z_status select_obj_from_path(zf_obj& start,z_string& path);
	//void get_auto_complete_list(z_string& partial);



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


