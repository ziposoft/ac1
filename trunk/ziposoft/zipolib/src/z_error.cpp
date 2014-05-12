#include "zipolib_cpp_pch.h"
#include "z_error.h"


#define BUFF_SIZE 100


z_error_handler gz_error;




z_status z_error_handler::report(ctext file,ctext func,int line,z_status status,const char*  lpszFormat,  ... )
{
    int c;
	static char buff[BUFF_SIZE];

	va_list ArgList;
	va_start (ArgList, lpszFormat);

    c=vsnprintf (buff,BUFF_SIZE-1, lpszFormat, ArgList);

	z_debug_out(buff);
	gz_out << buff << '\n';

	return status;
}