/*________________________________________________________________________

z_factory_h

________________________________________________________________________*/


#ifndef z_factory_h
#define z_factory_h

#include "zipolib/include/z_string.h"
#include "zipolib/include/z_error.h"
#include "zipolib/include/z_type_converter.h"
#include "zipolib/include/z_stl_vector.h"
#ifdef UNIX
typedef unsigned long long z_memptr;
#else
typedef unsigned long long z_memptr;

//typedef size_t z_memptr;
#endif
enum zf_feature_type
{
	zf_ft_none,
	zf_ft_var,
	zf_ft_obj,
	//zf_ft_pobj,
	zf_ft_act,


};
class z_factory;
class zp_value;

class zf_var_funcs_base
{
public:
	virtual void dump(z_file& s, void* v) const;
	virtual void get(z_string& s, void* v,int index=-1) const {};
	virtual void set(ctext s, void* v,int index=-1) const {};
	virtual void clear(void* v) const {} 
	virtual void add(void* list,void* obj) const {} 
	virtual void* get_item(void* list,size_t index) const { return 0;} 
	virtual size_t get_size(void* list) const { return 0;} 
	virtual void* get_ptr(void* var,int* iter) const { return var;}  /*could be pointer to obj, or pointer to obj pointer */
	//This is if the member var is an obj, pointer to obj, or obj list		
	virtual void* create_obj(void* var,const z_factory* fact) const { return 0;}  /*could be pointer to obj, or pointer to obj pointer */
	virtual const z_factory*  get_child_obj_fact() const { return 0;}
	virtual void set_from_value(zp_value* val, void* var,int index=-1) const {};

} ;

class zf_feature;
class zf_feature_list : public z_map_obj<zf_feature>
{


};

typedef  const zf_var_funcs_base* (*funcp_var_funcs_get)();
struct zf_var_entry
{
	const char* name;
	z_memptr offset;
	zf_feature_type type;  //currently not used for anything
	funcp_var_funcs_get fp_var_func_get;
};
struct z_factory_info
{
	ctext name;
	ctext parse;
	const size_t num_features;
	const zf_var_entry* list;
};

class z_factory_dyn 
{
public:
	z_string _name;
	z_factory* _parent;
	z_obj_vector_map<zf_feature> features;



};
class z_factory
{

protected:
	//Static stuff
	virtual const zf_var_entry* get_var_list() const{ return  get_info().list;}
	virtual const size_t get_var_list_size() const{ return  get_info().num_features;}
	virtual const zf_var_entry*  get_var_entry(ctext name) const;
	virtual const zf_var_entry*  get_var_entry(size_t index) const;

	//Static stuff
	z_factory_dyn* _dynamic;
	z_factory_dyn& get_dyn();

public:
	z_factory()
	{
		_dynamic=0;
	}
	virtual void* create_obj() const=0;
	virtual void delete_obj(void*) const=0;
	virtual ctext get_name()const  { return  get_info().name;}
	virtual const z_factory_info& get_info() const=0;
	virtual ctext get_parse_string() const { return  get_info().parse;}
	virtual ctext get_map_key()const;



	virtual z_status get_var_info_i(size_t index,ctext& name,z_memptr &offset,const zf_var_funcs_base*& funcs) const;
	virtual z_status get_var_info(ctext name,z_memptr &offset,const zf_var_funcs_base*& funcs) const;
	virtual int execute_act_ptr(void* obj,z_memptr  act_addr) const=0;



	virtual z_status create_child(void* obj,ctext var_name,const z_factory* new_child_type,void** ppChild) const;

	virtual z_status get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter=0) const;
	virtual z_status set_var_as_string(void* obj,ctext var_name,ctext value) const;
	virtual z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const;
	virtual void clear_all_vars(void* obj) const;
	virtual void dump_obj(z_file& f,void* obj) const;
	virtual void dump_static(z_file& f) const;

	virtual z_status get_list_features(z_strlist& list)const;

	virtual z_status execute_act(void* obj,ctext act_name,int* ret=0) const;

	virtual int add_act(ctext name,z_memptr act_addr,ctext desc); 
	virtual int add_prop(ctext name,zf_feature_type type,const zf_var_funcs_base* f,z_memptr act_addr,ctext desc); 
	z_status get_feature(ctext name,zf_feature& f) const;

};
class zf_obj
{
public:
    zf_obj()
	{
		_fact=0;	
		_obj=0;	
	}
	const z_factory* _fact;
	void* _obj;
};
z_obj_vector_map<z_factory>& get_factories_dynamic();
template <class C >  class z_factory_T :public  z_factory
{
public:
	typedef int (C::*fn_act)();
	typedef C THECLASS;
	z_factory_T()	 : z_factory()
	{			//STATIC
	}
	z_factory_T(ctext name)	 : z_factory()
	{
		//DYNAMIC
		get_dyn();
		add_features();
		get_factories_dynamic().add(this);
	}

	static z_factory_T<C> &self;
	virtual void* create_obj() const {return z_new C(); }
	virtual void delete_obj(void* v) const
	{
		delete reinterpret_cast<C*>(v);
	}
	virtual int execute_act_ptr(void* vobj,z_memptr act_addr) const
	{
		typedef int (C::*funcptr)();
		C*  cobj=reinterpret_cast<C*>(vobj);
		z_memptr* pp=&act_addr;
		funcptr fp=*( funcptr*) (pp);
		return (cobj->*fp)();
	}
	virtual int add_act_T(ctext name,fn_act act_addr,ctext desc) 
	{
		add_act(name,*(z_memptr*)&act_addr,desc);
		return 0;
	}

	virtual void add_features();
	virtual const z_factory_info& get_info() const;

};

const z_factory*  zf_get_factory(ctext name);
template <class CLASS> const z_factory*  zf_get_factory_T()
{
	return &z_factory_T<CLASS>::self;

}


#define zp_offsetof_class(_class_,_member_)   (size_t)&reinterpret_cast<const volatile char&>((((_class_*)0)->_member_))

class zf_feature
{

public:


	zf_feature();
	zf_feature(ctext name,zf_feature_type t,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc="");
	ctext get_map_key() { return _name; }

	z_string _name;
	z_string _description;
	const zf_var_funcs_base* df;
	z_memptr _offset;
	zf_feature_type _type;
	void dump(z_file& f,void* obj);

	void* get_var_ptr(void* obj,int* iter=0) ;

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


#define ZFACT(_CLASS_)  z_factory_T<_CLASS_> ZFACT##_CLASS_(#_CLASS_); \
	const z_factory_info 	ZFACT##_CLASS_##INFO={ #_CLASS_,0,0,0 };   \
	template <>  const z_factory_info& z_factory_T<_CLASS_>::get_info() const{ return ZFACT##_CLASS_##INFO; }	  \
	template <> z_factory_T<_CLASS_>& z_factory_T<_CLASS_>::self=ZFACT##_CLASS_;\
	template <> void z_factory_T<_CLASS_>	::add_features()

#define ZOBJ(_VAR_)						add_prop(#_VAR_,zf_ft_obj,zp_child_obj_funcs_get( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_),"");
#define ZPROP(_VAR_)					add_prop(#_VAR_,zf_ft_var,zp_var_funcs_get( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_),"");
#define ZPROP_X(_VAR_,_NAME_,_DESC_)	add_prop(_NAME_,zf_ft_var,zp_var_funcs_get( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_),_DESC_);
#define ZACT(_ACT_) add_act_T(#_ACT_,&THECLASS::_ACT_ ,"");
#define ZACT_X(_ACT_,_NAME_,_DESC_) add_act_T(_NAME_,&THECLASS::_ACT_ ,_DESC_);

#endif

