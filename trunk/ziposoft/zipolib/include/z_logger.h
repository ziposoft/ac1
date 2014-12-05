#ifndef z_logger_h
#define z_logger_h
#include "zipolib/include/zipolib_cpp.h"



enum z_logger_level
{
	z_logger_lvl_error,
	z_logger_lvl_warning,
	z_logger_lvl_info,
	z_logger_lvl_debug,
	z_logger_lvl_trace,
};

class z_logger_msg
{
public:
	z_string _msg;
	ctext _source_file;
	ctext _source_function;
	int _source_line;
	z_logger_level _lvl;
	z_status _status;


	z_logger_msg(z_logger_level lvl,ctext file,ctext func,int line,z_status status,const char*  msg=0);

	void dump(z_file *fp,z_logger_level lvl,bool debug);

};


class z_logger
{
	z_stl_obj_vector<z_logger_msg> _log;
public:
	z_logger()
	{
		_log_level=z_logger_lvl_warning;
	}
	z_status report_not_implemented(ctext file,ctext func,int line); 
	z_status add_msg(z_logger_level lvl,ctext file,ctext func,int line,z_status status,const char*  lpszFormat,   ... );
	void out_dbg(z_logger_level lvl,z_file* f,ctext file,ctext func,int line,z_status status,ctext msg);
	void out(z_logger_level lvl,z_file* f,z_status status,ctext msg);
	void dump(z_logger_level lvl,bool debug);
	int get_log_count();
	void set_capture_level(z_logger_level lvl)
	{
		_log_level=lvl;
	}
	int _log_level;

};

#define	ZT(...)   { get_logger().add_msg (z_logger_lvl_trace,__FILE__,__FUNCTION__,__LINE__,0, __VA_ARGS__);}


void z_logger_dump();

z_logger& get_logger();
#define ZT_ENABLE() 

#define	ZTF



#endif

