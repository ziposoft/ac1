/*________________________________________________________________________

 z_factory_h

________________________________________________________________________*/


#ifndef z_factory_h
#define z_factory_h

#include "zipolib/include/z_string.h"
#include "zipolib/include/z_error.h"
#include "zipolib/include/z_type_converter.h"


 class z_factory;
 class zf_var_funcs_base
 {
 public:
	virtual void dump(z_file& s, void* v) const;
	virtual void get(z_string& s, void* v) const {};
	virtual void set(ctext s, void* v) const {};
	virtual void clear(void* v) const {} 
	virtual void add(void* list,void* obj) const {} 
	virtual void* get_item(void* list,size_t index) const { return 0;} 
	virtual size_t get_size(void* list) const { return 0;} 
	virtual void* get_ptr(void* var,int* iter) const { return var;}  /*could be pointer to obj, or pointer to obj pointer */
	//This is if the member var is an obj, pointer to obj, or obj list		
	virtual void* create_obj(void* var,const z_factory* fact) const { return 0;}  /*could be pointer to obj, or pointer to obj pointer */
 } ;
 typedef  const zf_var_funcs_base* (*funcp_var_funcs_get)();




class z_factory
{
public:
	virtual void* create_obj() const=0;

	virtual z_status get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter=0) const=0;
	virtual z_status set_var_as_string(void* obj,ctext var_name,ctext value) const =0;
	virtual z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const =0;
	virtual void clear_all_vars(void* obj) const=0;
	virtual void dump_obj(z_file& f,void* obj) const=0;
	virtual void dump_static(z_file& f) const=0;

	virtual ctext get_name()const =0;
};

class zf_feature
{
	z_string _name;
public:
	zf_feature(ctext name);
	ctext get_map_key() { return _name; }
};

class zf_prop  : public  zf_feature
{
	z_string _name;
	z_string _description;
public:
	zf_var_funcs_base* df;


};

#endif
