/*________________________________________________________________________

 z_factory_h

________________________________________________________________________*/


#ifndef z_factory_h
#define z_factory_h

#include "zipolib/include/z_string.h"
#include "zipolib/include/z_error.h"
#include "zipolib/include/z_type_converter.h"
#include "zipolib/include/z_stl_vector.h"

enum zf_feature_type
{
	zf_ft_none,
	zf_ft_var,
	zf_ft_obj,
	zf_ft_pobj,
	zf_ft_act,


};

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
	virtual const z_factory*  get_child_obj_fact() const { return 0;}
 } ;
 typedef  const zf_var_funcs_base* (*funcp_var_funcs_get)();
class zf_feature;
class zf_feature_list : public z_map_obj<zf_feature>
{


};
struct zf_var_entry
{
	const char* name;
	size_t offset;
	zf_feature_type type;  //currently not used for anything
	funcp_var_funcs_get fp_var_func_get;
};



class z_factory
{

public:
	virtual void* create_obj() const=0;
	virtual void delete_obj(void*) const=0;
	virtual ctext get_name()const =0;
	/*
	virtual const zf_var_entry*  get_var_entry(ctext name) const=0;
	virtual const zf_var_entry*  get_var_entry(size_t index) const=0;
	virtual const size_t get_var_list_size() const=0;
	*/
	virtual z_status get_var_info_i(size_t index,ctext& name,size_t &offset,const zf_var_funcs_base*& funcs) const=0;
	virtual z_status get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const=0;
 	virtual int execute_act_ptr(void* obj,size_t  act_addr) const=0;



	virtual z_status create_child(void* obj,ctext var_name,const z_factory* new_child_type,void** ppChild) const;

	virtual z_status get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter=0) const;
	virtual z_status set_var_as_string(void* obj,ctext var_name,ctext value) const;
	virtual z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const;
	virtual void clear_all_vars(void* obj) const;
	virtual void dump_obj(z_file& f,void* obj) const;
	virtual void dump_static(z_file& f) const;

	virtual z_status get_list_features(zf_feature_list& list)const { return Z_ERROR(zs_operation_not_supported);};
 	virtual z_status execute_act(void* obj,ctext act_name,int* ret=0) const;

};


class zf_feature
{

public:


	zf_feature(ctext name,zf_var_funcs_base* funcs,size_t offset);
	ctext get_map_key() { return _name; }

	z_string _name;
	z_string _description;
	zf_var_funcs_base* df;
	size_t _offset;

};

class zf_prop  : public  zf_feature
{
public:


};
class zf_alias  : public  zf_feature
{
public:
	zf_feature* df;

};

class zf_action  : public  zf_feature
{
public:

};
#endif

