/*________________________________________________________________________

 z_factory_h

________________________________________________________________________*/


#ifndef z_factory_h
#define z_factory_h

#include "zipolib/include/zipolib_cpp_pch.h"
#include "zipolib/include/z_stl_map.h"
#include "zipolib/include/z_string.h"
#include "zipolib/include/z_error.h"
#include "zipolib/include/z_type_converter.h"


class z_factory_base
{
public:
	virtual void* create_obj() const=0;

	virtual z_status get_var_ptr(void* obj,ctext var_name,void** p_member_var) const =0;
	virtual z_status set_var_as_string(void* obj,ctext var_name,ctext value) const =0;
	virtual z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const =0;
	virtual void clear_all_vars(void* obj) const;
	virtual void dump_obj(z_file& f,void* obj) const;
	virtual void dump_obj_r(z_file& f,void* obj) const;
	virtual ctext get_name()const =0;


};


#endif

