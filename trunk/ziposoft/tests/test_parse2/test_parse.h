#ifndef test_parse_h
#define test_parse_h
#include "zipolib_cpp/include/z_trace.h"
#include "zipolib_cpp/include/z_file.h"
#include "zipolib_parse2/include/z_parse.h"

//opt_list_operation
int run_help();
int run_parse_obj();
int run_static_tests();
int run_parse_xml();

//opt_list_test_type
int run_test_only();
int run_test_create();
int run_test_create_output();
int run_create_def_obj();
int run_test_output_def();
int run_test_all();
int run_test_output();


struct st_test_obj_entry
{
	ctext item_name;
	ctext input_data;
	z_status expected_result;
};


struct st_test_tmpl_entry
{
	ctext templ;
	ctext input_data;
	z_status expected_result;
	ctext expected_output; //NOT the same as input_data!
	ctext expected_default_output;
	ctext desc;
};
extern const st_test_tmpl_entry test_tmpl_list[];
extern const size_t test_tmpl_list_count;


typedef int (*opt_func)();

struct option
{
	ctext name;
	ctext desc;
	opt_func pfunc;

};

struct argument
{
	ctext name;
	ctext desc;
	z_string& ref_variable;
	size_t num_valid_options;
	option* option_list;
};


extern const size_t  opt_list_operation_count;
extern const size_t  opt_list_test_type_count;
extern const size_t  arg_list_count;
extern zp_parser p;
extern const zp_factory * g_parse_fact;


extern option opt_list_test_type[];
extern option opt_list_operation[];
extern argument arg_list[];

//Global Args
extern z_string g_arg_operation;
extern z_string g_arg_test_num;
extern z_string g_arg_test_type;
extern z_string g_arg_data_in;
extern z_string g_arg_obj_type;
extern z_string g_arg_dump;
extern z_string g_arg_file_input_data;
extern z_string g_arg_file_output_data;

option* get_option(argument& a,z_string& optname);
void show_valid_options(argument& a);
int run_help();

//GLOBALS
extern opt_func g_test_type_function_to_run;


class testA 
{
public:
	testA()
	{
	   i123=123;
	   _val="defaultstr";

	}
	int i123;
	z_string _val;
};
class testB 
{
public:
	int i222;
	testB()
	{
		_child=0;
		i222=222;
	}
	testA* _child;
};
/*
class zp_xml_atr : public zp_obj
{
	ZO_OBJ_H;
	z_string _name;
	z_string _val;

};
class zp_xml_elm : public zp_obj
{
	ZO_OBJ_H;
	z_string _name;
};	
class zp_xml_trackpoint : public zp_obj
{
	ZO_OBJ_H;
	zp_xml_trackpoint()
	{

	}
};
class zp_xml_activity : public zp_obj
{
	ZO_OBJ_H;
	zp_xml_activity()
	{

	}
};
class zp_xml_tcd : public zp_xml_elm
{
	ZO_OBJ_H;
	
};
class zp_xml_file : public zp_obj
{
	ZO_OBJ_H;
	zp_xml_file()
	{
		
	}
	zp_xml_tcd _tcd;
};
*/
#endif