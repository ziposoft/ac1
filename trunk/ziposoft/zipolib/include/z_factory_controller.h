/*________________________________________________________________________

z_factory_h

________________________________________________________________________*/


#ifndef z_factory_controller_h
#define z_factory_controller_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_factory.h"

class z_factory_controller
{
protected:
	
	zf_obj _root;
	zf_obj _self;
	zf_obj _selected;
	zf_obj _temp;
	z_string _temp_path;
	zp_text_parser _tparser;
public:

	zp_text_parser& get_parser() { return  _tparser;}



};
#endif

