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
#define Z_TYPEINFO_P(_X_) (typeid(_X_).name()+1)
#define Z_TYPEINFO_O(_X_) (typeid(_X_).name())
#else
typedef unsigned long long z_memptr;
#define Z_TYPEINFO_P(_X_) (typeid(_X_).raw_name()+2)
#define Z_TYPEINFO_O(_X_) (typeid(_X_).raw_name()+2)
//typedef size_t z_memptr;
#endif


enum zf_feature_type
{
	zf_ft_none,
	zf_ft_var,
	zf_ft_param,
	zf_ft_obj,
	zf_ft_obj_list,
	//zf_ft_pobj,
	zf_ft_act,
	zf_ft_all,


};
class z_factory;
class zp_value;
class zf_list;
class zf_action;
class zf_child_obj;
class zp_text_parser;
class zf_funcs_obj_list_base;

class zf_feature;

class zf_var_funcs_base
{
public:
	virtual zf_feature_type get_type()const =0;
	virtual void dump(z_file& s, void* v) const;
	virtual void get(z_string& s, void* v,int index=-1) const {};
	virtual void set(ctext s, void* v,int index=-1) const {};
	virtual void clear(void* v) const {} 
	virtual void add(void* list,void* obj) const {} 
	virtual void* get_sub_obj(void* list,ctext key) const { return 0;} 
	virtual size_t get_size(void* list) const { return 0;} 
	virtual void* get_ptr(void* var,z_obj_list_iter& iter) const { return var;}  /*could be pointer to obj, or pointer to obj pointer */
	//This is if the member var is an obj, pointer to obj, or obj list		
	virtual void* create_obj(void* var,z_factory* fact) const { return 0;}  /*could be pointer to obj, or pointer to obj pointer */
	virtual z_factory*  get_fact_from_obj(void* obj) const { return 0;}
	virtual z_status set_from_value(zp_value* val, void* var,int index=-1) const { return Z_ERROR(zs_operation_not_supported);};

 	virtual z_status load(zp_text_parser &parser, void* v) const {return Z_ERROR(zs_operation_not_supported);}
 	virtual z_status assign(zp_text_parser &parser, void* v) const;
 	virtual z_status evaluate(zp_text_parser &parser, void* v) const  {return Z_ERROR(zs_operation_not_supported);}
	virtual zf_feature* create_feature(ctext name,z_memptr offset,ctext desc,U32 flags) const=0;
} ;

class zf_feature_list : public z_obj_map<zf_feature>
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
	z_factory* baseclass;
	ctext parse;
	const size_t num_features;
	const zf_var_entry* list;
};

class z_factory_dyn 
{
public:
	z_string _name;
	z_obj_vector_map<zf_feature> features;



};
class z_factory
{
	virtual const zf_var_entry*  _get_var_entry(ctext name) const;
	virtual const zf_var_entry*  _get_var_entry(size_t index) const;
protected:
	//Static stuff
	virtual const zf_var_entry* get_var_list() const{ return  get_info().list;}
	virtual const size_t get_static_feature_count() const{ return  get_info().num_features;}

	int get_num_features() const;
	//Static stuff
	z_factory_dyn* _dynamic;
	z_factory_dyn& init_dynamic();

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
	virtual ctext get_type_info_name()const =0;
	z_factory* get_base_factory() const;



	virtual z_status get_var_info_i(size_t index,ctext& name,z_memptr &offset,const zf_var_funcs_base*& funcs) const;
	virtual z_status get_var_info(ctext name,z_memptr &offset,const zf_var_funcs_base*& funcs) const;
	virtual int execute_act_ptr(void* obj,z_memptr  act_addr) const=0;



	virtual z_status create_child(void* obj,ctext var_name,z_factory* new_child_type,void** ppChild) const;

	virtual z_status get_child_obj_ptr(void* obj,ctext var_name,void** ppChild,z_obj_list_iter& iter) const;
	virtual z_status get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter=0) const;
	virtual z_status set_var_as_string(void* obj,ctext var_name,ctext value) const;
	virtual z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const;
	virtual void clear_all_vars(void* obj) const;
	virtual void dump_obj(z_file& f,void* obj) const;
	virtual void dump_obj_contents(z_file& f,void* obj) const;
	z_status load_obj_contents(zp_text_parser &parser,void* obj) const;
	virtual void dump_static(z_file& f) const;

	virtual z_status get_list_features(z_strlist& list,void* obj);
	virtual z_status get_map_features(zf_feature_list&  list,zf_feature_type type);

	virtual z_status execute_act(void* obj,ctext act_name,int* ret=0) const;
	virtual zf_action* add_act_params(ctext name,z_memptr act_addr,ctext desc,int num_params,...) ;

	virtual zf_feature* add_feature(zf_feature* f); 
	virtual zf_action* add_act(ctext name,z_memptr act_addr,ctext desc); 
	virtual zf_feature* add_prop(ctext name,const zf_var_funcs_base* f,z_memptr act_addr,ctext desc); 
	virtual zf_child_obj* add_obj(ctext name,const zf_var_funcs_base* f,z_memptr act_addr,ctext desc); 
	virtual zf_list* add_list(ctext name,const zf_funcs_obj_list_base* f,z_memptr act_addr,ctext desc); 
	zf_feature* get_feature(ctext name) ;

};
class zf_obj
{
public:
    zf_obj()
	{
		_fact=0;	
		_obj=0;	
	}
	z_factory* _fact;
	void* _obj;
};




class z_dynamic_factory_list
{
public:
	z_obj_vector_map<z_factory> _list;	
	z_factory* get_by_name(ctext name);
	z_factory* get_by_type(ctext type);
	void add(z_factory* f);

};

z_dynamic_factory_list& get_factories_dynamic();


template <class C >  class z_factory_T :public  z_factory
{
public:
	friend  C;
	typedef int (C::*fn_act)();
	typedef C THECLASS;
	z_factory_T()	 : z_factory()
	{			//STATIC
	}
	z_factory_T(ctext name)	 : z_factory()
	{
		//DYNAMIC
		init_dynamic();
		add_features();
		get_factories_dynamic().add(this);
	}

	virtual void* create_obj() const =0;
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

	virtual void add_features();
	virtual const z_factory_info& get_info() const;
	virtual ctext get_type_info_name()const 
	{
		C* nullobj=0;
		return Z_TYPEINFO_P(nullobj);
	}
};
template <class C >  class z_factory_T_pure_virtual :public  z_factory_T<C>
{
public:
 	virtual void* create_obj() const {return 0; }
	static z_factory_T_pure_virtual<C> &self;
	z_factory_T_pure_virtual(ctext name) : z_factory_T(name) {}

};
template <class C >  class z_factory_T_create :public  z_factory_T<C>
{
public:
	static z_factory_T_create<C> &self;
	z_factory_T_create(ctext name) : z_factory_T(name) {}
	virtual void* create_obj() const {return z_new C(); }


};
z_factory*  zf_get_factory(ctext name);
z_factory*  zf_get_factory_by_type(ctext type);
z_status zf_create_obj_from_text_stream(zp_text_parser &parser, z_factory* &factory,void* &objpointer);


template <class CLASS> z_factory*  zf_get_factory_T()
{
	return &z_factory_T<CLASS>::self;

}


#define zp_offsetof_class(_class_,_member_)   (size_t)&reinterpret_cast<const volatile char&>((((_class_*)0)->_member_))

extern z_factory* _pgz_factory_none;
#define _gz_factory_none *_pgz_factory_none

#define _ZFACT_V(_CLASS_,_BASECLASS_,_FACTTYPE_)   _FACTTYPE_<_CLASS_> _gz_factory_##_CLASS_(#_CLASS_); \
	const z_factory_info 	ZFACT##_CLASS_##INFO={ #_CLASS_,&_gz_factory_##_BASECLASS_,0,0,0 };\
	template <>  const z_factory_info& _FACTTYPE_<_CLASS_>::get_info() const{ return ZFACT##_CLASS_##INFO; }	  \
	template <> _FACTTYPE_<_CLASS_>& _FACTTYPE_<_CLASS_>::self=_gz_factory_##_CLASS_;\
	template <> void _FACTTYPE_<_CLASS_>	::add_features()


#define ZFACT_PV(_CLASS_) _ZFACT_V(_CLASS_,none,z_factory_T_pure_virtual) 
#define ZFACT_V(_CLASS_,_BASECLASS_) _ZFACT_V(_CLASS_,_BASECLASS_,z_factory_T_create) 
#define ZFACT(_CLASS_) _ZFACT_V(_CLASS_,none,z_factory_T_create) 



#include "zipolib/include/z_factory_features.h"

#endif

