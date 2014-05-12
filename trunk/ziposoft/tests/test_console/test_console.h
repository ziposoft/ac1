// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib/include/zipolib.h"
#include "zipolib_parse/include/z_console.h"


class z_console_ntf : public z_console
{
public:
	z_console_ntf()
	{
		_dummy=0;
		debug=true;
	}
	int _dummy;
	bool debug;
	z_string _executable_name;
	z_string path;
	//virtual void 
	//z_status execute_line2();


	U32 _feature_index;
	U32 _feature_count;
	zo_feature_list _auto_tab;
	z_strlist _history;
	int _history_index;
	z_string _partial;
	U32  _tab_count;
	U32  _tab_mode_line_index;

	virtual void refresh();
	
	void OnDoubleBack();
	void runz(zp_obj_base* obj);
	void show_history();
	virtual void display_text(const z_string& s);
    virtual void put_prompt();
    virtual void OnEnter();
    virtual void OnTab();
    virtual void get_auto_complete_list(zp_feature& parse_feature);

	z_status process_args(int argc, char** pargv);


	void dump_all();
	void dump_feature_outline(zp_obj_base* obj);
	void dump_obj2(zp_obj_base* obj);

	//command line functions


	//command line props
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

