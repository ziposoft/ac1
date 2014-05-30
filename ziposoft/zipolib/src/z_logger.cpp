#include "zipolib_cpp_pch.h"
#include "z_logger.h"

#define BUFF_SIZE 100



z_logger_msg::z_logger_msg(z_logger_level lvl,ctext file,ctext func,int line,z_status status,const char*  msg)
{
	_msg=msg;
	_lvl=lvl;
	_status=status;
	_source_file=file;
	_source_function=func;
	_source_line=line;

}
void z_logger_msg::dump(z_file *fp)
{
	gz_logger.out(fp,_source_file, _source_function,_source_line,_msg.c_str());

}
void z_logger::out(z_file* f,ctext file,ctext func,int line,ctext msg)
{
	ctext fn=z_get_filename_from_path(file);
	f->putf("%s[%d] %s() : %s\n",
		fn,
		line,
		func,
		msg);
}

z_status z_logger::add_msg(z_logger_level lvl,ctext file,ctext func,int line,z_status status,const char*  lpszFormat,  ... )
{
    int c;
	char* buff=z_temp_buffer_get(BUFF_SIZE);

	va_list ArgList;
	va_start (ArgList, lpszFormat);

    c=vsnprintf (buff,BUFF_SIZE-1, lpszFormat, ArgList);


	z_debug_out(buff);
	out(&gz_debug,file,func,line,buff);

	_log << new z_logger_msg(lvl, file, func, line, status, buff);
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
