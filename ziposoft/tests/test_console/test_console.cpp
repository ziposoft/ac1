// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
#include "zipolib/include/z_parse.h"



class testAs 
{
public:
	testAs() { i=0; }
	int func() { printf("hooray!!\n");return 0;};
	int func2() { printf("time for a %d min nap!!\n",i);return 0;};
	int i;
};
 class testAd
 {
 public:
	 testAd()
	 {
		_i=123;
		_str="fred";

	 }
	 virtual ~testAd(){}
	 int  _i;
	 z_string _str;
	 int func()
	 {
		 printf("hooorraaayy!!! %d  %s\n",_i,_str.c_str());
		return 0;
	 }

 };




z_console g_con;
int main(int argc, char* argv[])
{
	z_status status=zs_no_match;
	/*
	testA A;

	zfs_get_static_factory("testA")->execute_act(&A,"func");
	zfs_get_static_factory("testA")->set_var_as_string(&A,"i","67");
	zfs_get_static_factory("testA")->execute_act(&A,"func2");
   */
	gz_out << "load save args...\n";
	z_debug_load_save_args(&argc,&argv);
	gz_out << "load save args done\n";

	int i;
	//ZT_ENABLE();
	zp_cmdline 	cmdline ;
	z_parser parser;
	for(i=1;i<argc;i++)
	{
		gz_out << "parsing [%s]:\n";
		/*
		status=parser.parse_obj(&cmdline,argv[i]);
		if(status==	zs_ok)
		{
			zf_dump_obj( &cmdline);
			


		}
		else
			parser.report_error();
			*/


	}
 
	g_con.run();
	return 0;
}





 #define ZO_OBJ_LIST \
	ZCLS(testAs,void,"cmdline","{_val}ident:'=':{i123}int",ACT(func) ACT(func2) VAR(i)) 

		  
#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod), ZP_MOD(parse));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")