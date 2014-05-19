/*________________________________________________________________________

 z_factory_dyn_h

________________________________________________________________________*/


#ifndef z_factory_dyn_h
#define z_factory_dyn_h

#include "zipolib/include/z_factory.h"

class z_factory_dyn : public z_factory
{
public:
	virtual void* create_obj() const=0;
	virtual void delete_obj(void*) const=0;

	virtual z_status get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter=0) const=0;
	virtual z_status set_var_as_string(void* obj,ctext var_name,ctext value) const =0;
	virtual z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const =0;
	virtual void clear_all_vars(void* obj) const=0;
	virtual void dump_obj(z_file& f,void* obj) const=0;
	virtual void dump_static(z_file& f) const=0;

	virtual ctext get_name()const =0;
	virtual z_status get_list_features(zf_feature_list& list)const { return Z_ERROR(zs_operation_not_supported);};

};

#endif

