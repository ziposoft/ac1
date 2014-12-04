#include "zipolib_cpp_pch.h"
#include "z_logger.h"
#include "z_factory_static.h"

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
 /*__________________________________________________________________________

	z_logger_msg 
____________________________________________________________________________*/
z_logger_msg::z_logger_msg(z_logger_level lvl,ctext file,ctext func,int line,z_status status,const char*  msg)
{
	_msg=msg;
	_lvl=lvl;
	_status=status;
	_source_file=file;
	_source_function=func;
	_source_line=line;

}


void z_logger_msg::dump(z_file *fp,z_logger_level lvl,bool debug)
{
	if(debug)
	get_logger().out_dbg(_lvl,fp,_source_file, _source_function,_source_line,_status,_msg.c_str());
	else
	get_logger().out(_lvl,fp,_status,_msg.c_str());

}

 /*__________________________________________________________________________

	z_logger 
____________________________________________________________________________*/


void z_logger::out_dbg(z_logger_level lvl,z_file* f,ctext file,ctext func,int line,
				   z_status status,ctext msg)
{
	ctext fn=z_get_filename_from_path(file);
	f->putf("%s-%s[%d] %s()",
		log_level_type[lvl],
		fn,
		line,
		func);
	if(status)
		*f<<':'<<zs_get_status_text(status);
	if(msg)
		*f<<':'<<msg;
	*f<<'\n';
}
void z_logger::out(z_logger_level lvl,z_file* f, z_status status,ctext msg)
{
	f->putf("%s-",	log_level_type[lvl]);
	if(status)
		*f<<zs_get_status_text(status);
	if(msg)
		*f<<','<<msg;
	*f<<'\n';
}

z_status z_logger::report_not_implemented(ctext file,ctext func,int line)
{
	add_msg(z_logger_lvl_error,file,func,line,zs_not_implemented,"Function \"%s\" not implemented",func);
	return zs_not_implemented;

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

	out_dbg(lvl,&z_debug_get(),file,func,line,status,buff);
	if(lvl<=_log_level)
		_log << new z_logger_msg(lvl, file, func, line, status, buff);
	if(buff)
		z_temp_buffer_release(buff);
	return status;
}
int z_logger::get_log_count()
{
	return _log.size();

}

void z_logger::dump(z_logger_level lvl,bool debug )
{
	size_t i;
	for(i=0;i<_log.size();i++)
	{
		_log[i]->dump(&zout,lvl,debug);
		delete _log[i];

	}
	_log.clear();
}
void z_logger_dump()
{
	get_logger().dump(z_logger_lvl_warning,false);
}

z_logger& get_logger()
{
	static z_logger gz_logger;
	return gz_logger;

}


#define ZO_OBJ_LIST \
	ZCLS(z_logger,none,"","", VAR(_log_level) )




#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(logger);
