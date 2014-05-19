// test_console.cpp : Defines the entry point for the console application.
//


#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_terminal.h"






int main(int argc, char* argv[])
{
	z_terminal t;
	t.terminal_open();

	t.curGotoXY(3,3);
	printf("hello?");
	t.Close();
	return 0;
}


