#ifndef z_console_h
#define z_console_h
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_terminal.h"
#include "zipolib/include/z_factory_controller.h"

class z_console_base : public z_terminal
{


	U32 index;
	U32 cur_start;
	U32 len;
	bool insertmode;
	enum_key _key;
	enum_key _prev_key;
	int _history_index;
	z_string _partial;
	U32 get_index();
	void AppendChar(char ch);
	void RedrawLine(int blanks=0);
	void InsertChar(char ch);
	void OverwriteChar(char ch);
	void hChar(char ch);
	void reset_line();
	void clear_line();
    void inc_history(int i);

protected:
	z_strlist _auto_tab;
	bool _tab_mode;
	U32  _tab_count;
	U32  _tab_index;
	U32  _tab_mode_line_index;
	void trim_line_to(int trim_point);
	U32 get_line_length();
	void output(ctext text);

	z_string _buffer;
	bool _running;

	virtual z_status ExecuteLine(ctext text);
    virtual void OnEnter();
    virtual void OnTab();
    virtual void OnUp();
    virtual void OnDown();
    virtual void OnDoubleBack();
    virtual void put_prompt();
public:
	z_string  _param_path;
	z_strlist _history;

  	virtual void get_current_path(z_string &path)=0;

	z_console_base();
	z_status run();

};

class z_console : public z_console_base	,public   z_factory_controller
{
public:
	z_console();
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
	//virtual z_status EvaluatePath(ctext  text);
	//virtual z_status EvaluateLine2(ctext  text);
	//z_string _cmd_line_feature;
	//z_string _cmd_line_feature_index;
	z_status evaluate_feature(const zf_obj& o,z_string& name,z_string& idx);
	bool is_feature(const zf_obj& o,z_string& name);
	//z_status get_feature_and_index();
	//z_status select_obj();
    virtual void OnTab();


	zf_obj _temp_selected_obj;

	bool _has_feature;

	void init(ctext appname);




	z_status  runapp(int argc, char* argv[],bool loadcfg);
  	virtual void get_current_path(z_string &path);

	//command line props
	bool _dump_cmd_line;
	z_string _script_file;
	bool _param_script_step;
	z_string _config_file;
	z_string _startup_path;
	z_status get_config_file_path(z_string& path);

	//command line functions
	z_status list_features();
	z_status dumpcfg();
	z_status loadcfg();
	z_status savecfg();
	z_status act_exec();
	z_status help();
	z_status shell();
	z_status act_exit();
	z_status act_up();
	

};


#endif








