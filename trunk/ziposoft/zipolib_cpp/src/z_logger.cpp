#include "zipolib_cpp_pch.h"
#include "z_logger.h"

#define BUFF_SIZE 100



z_logger_msg::z_logger_msg(ctext file,ctext func,int line,z_status status,const char*  msg)
{
	_msg=msg;
	_source_file=file;
	_source_function=func;
	_source_line=line;

}

z_status z_logger::add_msg(ctext file,ctext func,int line,z_status status,const char*  lpszFormat,  ... )
{
    int c;
	static char buff[BUFF_SIZE];

	va_list ArgList;
	va_start (ArgList, lpszFormat);

    c=vsnprintf (buff,BUFF_SIZE-1, lpszFormat, ArgList);

	_log << new z_logger_msg( file, func, line, status, buff);
	return status;
}


z_logger gz_logger;
