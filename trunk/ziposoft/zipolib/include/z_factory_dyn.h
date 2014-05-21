/*________________________________________________________________________

 z_factory_dyn_h

________________________________________________________________________*/


#ifndef z_factory_dyn_h
#define z_factory_dyn_h

#include "zipolib/include/z_factory.h"

class z_factory_dyn : public z_factory
{
protected:
	z_string _name;
	z_factory_dyn* _parent;
	z_obj_vector_map<zf_feature> _features;
public:
	virtual z_status get_var_info_i(size_t index,ctext& name,size_t &offset,const zf_var_funcs_base*& funcs) const;
	virtual z_status get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const;

	virtual ctext get_name()const;
	virtual z_status get_list_features(zf_feature_list& list)const { return Z_ERROR(zs_operation_not_supported);};
	template <class ITEM> void addProp(ctext id,ITEM* var)
	{
		z_ntf_prop* p= new z_ntf_prop_t<ITEM> (this,id,var);
		_features<<p;
	}
};

extern z_obj_vector_map<z_factory_dyn> g_factories_dynamic;

 template <class C >  class z_factory_T :public  z_factory_dyn
 {
 public:
	typedef int (C::*fn_act)();

	z_factory_T(ctext name)
	{
		_name=name;
		add_features();

	}

	static z_factory_T<C> &self;
	virtual void* create_obj() const {return z_new C(); }
	virtual void delete_obj(void* v) const
	{
		delete reinterpret_cast<C*>(v);
	}
 	virtual int execute_act_ptr(void* vobj,size_t act_addr) const
	{
		typedef int (C::*funcptr)();
		C*  cobj=reinterpret_cast<C*>(vobj);
		void* pp=&act_addr;
		funcptr fp=*( funcptr*) (pp);
		return (cobj->*fp)();
	}
 	virtual int add_act(ctext name,fn_act act_addr) 
	{
		_features.add(z_new	zf_feature(name,0,*(size_t*)(void*)&act_addr));
		return 0;
	}

	void add_features();

 };



#endif

