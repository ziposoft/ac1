#ifndef z_console_h
#define z_console_h
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_parse.h"
#include "zipolib/include/z_terminal.h"

class z_console : public z_terminal
{


	U32 index;
	U32 cur_start;
	U32 len;
	bool insertmode;
	enum_key _key;
	enum_key _prev_key;
	bool TabMode;
	z_string buff;
	bool _running;
	int _history_index;
	z_string _partial;
	U32  _tab_count;
	U32  _tab_mode_line_index;
    virtual void put_prompt();
	U32 get_index();
	U32 get_line_length();
	void AppendChar(char ch);
	void RedrawLine(int blanks=0);
	void InsertChar(char ch);
	void OverwriteChar(char ch);
	void hChar(char ch);
	void reset_line();
	void output(ctext text);
	void clear_line();
	void trim_line_to(int trim_point);
    void inc_history(int i);
	void get_auto_complete_list(z_string& partial);

protected:
	z_strlist _history;
	z_strlist _path;
	zp_cmdline 	_cmdline ;
	z_parser _parser;
	z_status parse_line(ctext text);
	virtual z_status OnExecuteLine(ctext text);
    virtual void OnEnter();
    virtual void OnTab();
    virtual void OnUp();
    virtual void OnDown();
    virtual void OnDoubleBack();
public:


	z_console();
	void run();

};
#if 0
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
	void runz(void* obj);
	void show_history();
	void inc_history(int i);
    virtual void OnUp();
    virtual void OnDown();
	virtual void display_text(const z_string& s);
    virtual void put_prompt();
    virtual void OnEnter();
    virtual void OnTab();
    virtual void get_auto_complete_list(zp_feature& parse_feature);

	z_status process_args(int argc, char** pargv);


	void dump_all();
	void dump_feature_outline(void* obj);
	void dump_obj2(void* obj);

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
class zo_root : public z_obj_vect<void>
{
public:
	ZO_OBJ_H;
	zo_root();
	zo_console	_console;
	z_trace		*_p_trace;
	z_trace		*_p_child;

	virtual ctext get_map_key();



};
#endif
#if 0
class zo_console : public z_console,public zo_man_cmd
{
public:
	ZO_OBJ_H;
	zo_console(void* obj=0) : zo_man_cmd(obj)
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
	zo_str_vect _history;
	int _history_index;
	z_string _partial;
	U32  _tab_count;
	U32  _tab_mode_line_index;

	virtual void refresh();
	
	void OnDoubleBack();
	void runz(void* obj);
	void show_history();
	void inc_history(int i);
    virtual void OnUp();
    virtual void OnDown();
	virtual void display_text(const z_string& s);
    virtual void put_prompt();
    virtual void OnEnter();
    virtual void OnTab();
    virtual void get_auto_complete_list(zp_feature& parse_feature);

	z_status process_args(int argc, char** pargv);


	void dump_all();
	void dump_feature_outline(void* obj);
	void dump_obj2(void* obj);

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
class zo_root : public z_obj_vect<void>
{
public:
	ZO_OBJ_H;
	zo_root();
	zo_console	_console;
	z_trace		*_p_trace;
	z_trace		*_p_child;

	virtual ctext get_map_key();



};

#endif
#endif








