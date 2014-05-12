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
void z_logger_msg::dump(z_file *fp)
{
	fp->putf("%s[%d] %s() : %s\n",
		_source_file.c_str(),
		_source_line,
		_source_function.c_str(),
		_msg.c_str());


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

void z_logger::dump( )
{
	size_t i;
	for(i=0;i<_log.size();i++)
	{
		_log[i]->dump(&gz_out);

	}


}
void z_logger_dump()
{
	gz_out << "Dumping logger:\n";
	gz_logger.dump();
}

z_logger gz_logger;
