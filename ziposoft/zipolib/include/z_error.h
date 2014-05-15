#ifndef z_error_h
#define z_error_h
#include "zipolib/include/zipo.h"


enum {
	zs_ok=0,
	zs_matched=0,
	z_status_success=0,
	zs_no_match,
	zs_skipped,
	zs_unparsed_data,
	zs_eof,
	zs_end_of_list,
	zs_child_not_found,
	zs_fatal_error,//fatal errors below. Status above!
	z_status_error,
	z_status_could_not_open_dir,
	z_status_already_open,
	z_status_bad_parameter,
	z_status_item_not_found,
 	zs_feature_not_found,
	zs_operation_not_supported,
	zs_cannot_create_virtual_obj,
	zs_no_entry_for_item,
	zs_syntax_error,
	zs_template_syntax_error,
	zs_tmpl_expected_closing_parenthesis,
	zs_internal_error,
	zs_bad_argument_2,
	zs_bad_parameter,
	zs_wrong_object_type,

};
ctext z_status_get_text(z_status status);

#ifdef	__cplusplus
#include "zipolib/include/z_file.h"
#include "zipolib/include/z_logger.h"
#include "zipolib/include/z_trace.h"


/*
errors can be reported many ways.

To stdout, to the zipo log, to the OS debug/trace facilities (DbgPrint), to custom callbacks, etc.


*/



class z_error_handler : public z_file
{
public:
    z_error_handler()
    {
    }
    virtual ~z_error_handler()
    {
    }
	z_status report(ctext file,ctext func,int line,z_status status,const char*  lpszFormat,   ... );
};

extern z_error_handler gz_error;
#define	Z_ERROR(status)   gz_error.report (__FILE__,__FUNCTION__,__LINE__,status,"");
#define	Z_ERROR_MSG(status,...)   gz_error.report (__FILE__,__FUNCTION__,__LINE__,status,__VA_ARGS__);

//#define Z_ERROR_THROW(_error_no_,_error_msg_) {zb_error* e=new  zb_error(_error_no_);e->_error_msg.Format _error_msg_; throw e;}

#endif
#endif

