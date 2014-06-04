#include "zipolib_cpp_pch.h"
#include "z_logger.h"

#define BUFF_SIZE 1024

/*
enum z_logger_level
{
	z_logger_lvl_error,
	z_logger_lvl_warning,
	z_logger_lvl_info,
	z_logger_lvl_debug,
	z_logger_lvl_trace,
};
*/

ctext log_level_type[]=
{
	"ERROR",
	"WARNING",
	"INFO",
	"DBG",
	"TRC",
};

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
	gz_logger.out(_lvl,fp,_source_file, _source_function,_source_line,_status,_msg.c_str());

}
void z_logger::out(z_logger_level lvl,z_file* f,ctext file,ctext func,int line,z_status status,ctext msg)
{
	ctext fn=z_get_filename_from_path(file);
	f->putf("%s-%s[%d] %s()",
		log_level_type[lvl],
		fn,
		line,
		func);
	if(status)
		*f<<':'<<zs_get_text(status);
	if(msg)
		*f<<':'<<msg;
	*f<<'\n';
}

z_status z_logger::add_msg(z_logger_level lvl,ctext file,ctext func,int line,z_status status,const char*  lpszFormat,  ... )
{
    int c;
	char* buff=0;
	if(lpszFormat)
	{
		buff=z_temp_buffer_get(BUFF_SIZE);
		va_list ArgList;
		va_start (ArgList, lpszFormat);

		c=vsnprintf (buff,BUFF_SIZE-1, lpszFormat, ArgList);
	}

	out(lvl,&gz_debug,file,func,line,status,buff);
	if(lvl<=_log_level)
		_log << new z_logger_msg(lvl, file, func, line, status, buff);
	if(buff)
		z_temp_buffer_release(buff);
	return status;
}

void z_logger::dump( )
{
	size_t i;
	for(i=0;i<_log.size();i++)
	{
		_log[i]->dump(&gz_out);
		delete _log[i];

	}
	_log.clear();


}
void z_logger_dump()
{
	gz_out << "Dumping logger:\n";
	gz_logger.dump();
}

z_logger gz_logger;
