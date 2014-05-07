#include "test_parse.h"


z_string g_arg_operation="help";
z_string g_arg_test_num="all";
z_string g_arg_test_type="all";
z_string g_arg_data_in="";
z_string g_arg_obj_type="";
z_string g_arg_dump="off";
z_string g_arg_file_input_data="";
z_string g_arg_file_output_data="";



option opt_list_operation[]=
{ 
	{ "help","show help" ,run_help},
	{ "static","run tests from built in list",run_static_tests },
	{ "tmpl","use provided template",0 },
	{ "xml","parse xml file",run_parse_xml },
	{ "obj","test a specific parse object",run_parse_obj },
};
const size_t  opt_list_operation_count= (sizeof(opt_list_operation)/sizeof(option));

int run_test_only();
int run_test_create();
int run_test_create_output();
int run_create_def_obj();
int run_test_output_def();
int run_test_all();
int run_test_output();

option opt_list_test_type[]=
{ 
	{ "all","run all test types" ,run_test_all},
	{ "test","run test only" ,run_test_only},
	{ "create","run test/create",run_test_create },
	{ "defobj","create default object",run_create_def_obj },
	{ "output","run test/create/output" ,run_test_create_output},
	{ "outdef","run default output",run_test_output_def },
};
const size_t  opt_list_test_type_count= (sizeof(opt_list_test_type)/sizeof(option));



argument arg_list[]=
{
	{ "oper","operation",g_arg_operation,opt_list_operation_count,opt_list_operation},
	{ "num","test to run",g_arg_test_num,0,0},
	{ "type","type of test to perform",g_arg_test_type,opt_list_test_type_count,opt_list_test_type},
	{ "dump","dump object tree",g_arg_dump,0,0},
	{ "obj","object to test",g_arg_obj_type,0,0},
	{ "data","data to use",g_arg_data_in,0,0},
	{ "datafile","data file to use",g_arg_file_input_data,0,0},
	{ "outfile","output file to use",g_arg_file_output_data,0,0},
};
const size_t  arg_list_count= (sizeof(arg_list)/sizeof(argument));




option* get_option(argument& a,z_string& optname)
{
	size_t i;
	for(i=0;i<a.num_valid_options;i++)
	{
		if(optname==a.option_list[i].name)
			return &a.option_list[i];
	}
	return 0;
}
void show_valid_options(argument& a)
{
	size_t i;
	gz_out<<"\tValid options:\n";
	for(i=0;i<a.num_valid_options;i++)
	{
		gz_out <<'\t' << a.option_list[i].name <<" - " <<a.option_list[i].desc<<"\n";
	}
}
int run_help()
{
	int j;
	for(j=0;j<arg_list_count;j++)
	{
		argument& a= arg_list[j];
		gz_out << '\n' << a.name << " - " << a.desc<<"\n";
		show_valid_options(a);
	}
	return 0;
}
