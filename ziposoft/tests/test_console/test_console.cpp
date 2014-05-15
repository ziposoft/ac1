// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
Z_MODULE_INCLUDE(Z_MOD(parse));

z_console g_con;
int main(int argc, char* argv[])
{
	z_status status=zs_no_match;
	int i;
	ZT_ENABLE();
	zp_cmdline 	cmdline ;
	zp_obj_parser parser;
	for(i=1;i<argc;i++)
	{
		status=parser.parse_obj(&cmdline,argv[i]);
		if(status==	zs_ok)
		{

			


		}


	}

	//g_con.run();
	return 0;
}

