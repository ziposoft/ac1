// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
#include "zipolib/include/z_parse.h"

ZP_MODULE_INCLUDE(ZP_MOD(parse));

z_console g_con;
int main(int argc, char* argv[])
{
	z_status status=zs_no_match;
	int i;
	ZT_ENABLE();
	zp_cmdline 	cmdline ;
	z_parser parser;
	for(i=1;i<argc;i++)
	{
		status=parser.parse_obj(&cmdline,argv[i]);
		if(status==	zs_ok)
		{
			zf_dump_obj( &cmdline);
			


		}


	}

	//g_con.run();
	return 0;
}

