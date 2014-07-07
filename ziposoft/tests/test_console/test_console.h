// test_console.cpp : Defines the entry point for the console application.
//

#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_console.h"



class z_random_access
{
public:
	char* _p_data;

	int offset;
	int width;
	int length;

	virtual int dump()
	{
		return 0;
	}
	virtual int write_pattern_incrementing()
	{
		return 0;
	}
	virtual int write_pattern_set()
	{

		return 0;
	}







};