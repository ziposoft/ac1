#ifndef z_error_h
#define z_error_h
#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_cpp/include/z_file.h"
#include "zipolib_cpp/include/z_logger.h"
#include "zipolib_cpp/include/z_trace.h"


enum {
	z_status_cpp_lib_start=0x1000,
	z_status_could_not_open_dir,








};

class z_error : public z_file
{
public:
    z_error()
    {
    }
    virtual ~z_error()
    {
    }
};

extern z_error gz_error;
#define	Z_ERROR(status, ...)   gz_logger.add_msg (__FILE__,__FUNCTION__,__LINE__,status,__VA_ARGS__);
#define	Z_ERROR_RETURN(status, ...)  gz_logger.add_msg( __FILE__,__FUNCTION__,__LINE__,status,__VA_ARGS__);
#define Z_ERROR_THROW(_error_no_,_error_msg_) {zb_error* e=new  zb_error(_error_no_);e->_error_msg.Format _error_msg_; throw e;}

#endif

