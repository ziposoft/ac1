#ifndef z_error_h
#define z_error_h
#include "zipolib/include/zipo.h"


#ifdef	__cplusplus
#include "zipolib/include/z_file.h"
#include "zipolib/include/z_logger.h"
#include "zipolib/include/z_trace.h"


/*
errors can be reported many ways.

To stdout, to the zipo log, to the OS debug/trace facilities (DbgPrint), to custom callbacks, etc.


*/

#define	Z_ERROR_NOT_IMPLEMENTED  get_logger().report_not_implemented (__FILE__,__FUNCTION__,__LINE__);

#define	Z_ERROR_DBG(status)   get_logger().add_msg (z_logger_lvl_debug,__FILE__,__FUNCTION__,__LINE__,status,0);

#define	Z_ERROR(status)   (z_debug_break()?get_logger().add_msg (z_logger_lvl_error,__FILE__,__FUNCTION__,__LINE__,status,0):status);
#define	Z_ERROR_MSG(status,...)    (z_debug_break()?get_logger().add_msg (z_logger_lvl_error,__FILE__,__FUNCTION__,__LINE__,status,__VA_ARGS__):status);

//#define Z_ERROR_THROW(_error_no_,_error_msg_) {zb_error* e=new  zb_error(_error_no_);e->_error_msg.Format _error_msg_; throw e;}
#endif
#endif

