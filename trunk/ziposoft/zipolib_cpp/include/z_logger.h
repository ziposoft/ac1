#ifndef z_logger_h
#define z_logger_h
#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_cpp/include/z_string.h"


enum z_logger_level
{
	z_logger_lvl_error,
	z_logger_lvl_warning,
	z_logger_lvl_info,
	z_logger_lvl_debug,
};

class z_logger_msg
{
public:
	z_string _msg;
	z_string _source_file;
	z_string _source_function;
	int _source_line;

	z_logger_msg(ctext file,ctext func,int line,z_status status,const char*  msg=0);



};


class z_logger
{
	z_stl_list<z_logger_msg> _log;
public:
	z_status add_msg(ctext file,ctext func,int line,z_status status,const char*  lpszFormat,   ... );


};

#define	Z_LOG_ERROR(status,...)  gz_logger.add_msg (__FILE__,__FUNCTION__,__LINE__,status, __VA_ARGS__);

void z_logger_dump();

extern z_logger gz_logger;
#endif

