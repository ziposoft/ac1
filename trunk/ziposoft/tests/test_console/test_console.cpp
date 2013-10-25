// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_parse/include/z_console.h"
Z_MODULE_INCLUDE(Z_MOD(parse));
z_console g_con;
int main(int argc, char* argv[])
{
	g_con.run();
	return 0;
}

