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

typedef  U64 zf_feature_flags; 
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
	virtual z_status dump(z_file& s, void* v) const;
	virtual z_status get(z_string& s, void* v,ctext format,int index=-1) const {return Z_ERROR_NOT_IMPLEMENTED;};
	virtual z_status set(ctext s, void* v,ctext format,int index=-1) const {return Z_ERROR_NOT_IMPLEMENTED;};
	virtual z_status clear(void* v) const {return Z_ERROR_NOT_IMPLEMENTED;} 
	virtual z_status add(void* list,void* obj) const {return Z_ERROR_NOT_IMPLEMENTED;} 
	virtual void* get_sub_obj(void* list,ctext key) const { return 0;} 
	virtual size_t get_size(void* list) const { return 0;} 
	virtual void* get_ptr(void* var,z_obj_list_iter& iter) const { return var;}  /*could be pointer to obj, or pointer to obj pointer */
	//This is if the member var is an obj, pointer to obj, or obj list		
	virtual void* create_obj(void* var,z_factory* fact) const { return 0;}  /*could be pointer to obj, or pointer to obj pointer */
	virtual z_factory*  get_fact_from_obj(void* obj) const { return 0;}
	virtual z_status set_from_value(zp_value* val, void* var,int index=-1) const { return Z_ERROR_NOT_IMPLEMENTED;};

 	virtual z_status load(zp_text_parser &parser, void* v,zf_feature_flags oper) const {return Z_ERROR_NOT_IMPLEMENTED;}
 	virtual z_status assign(zp_text_parser &parser, void* v,zf_feature_flags oper) const;
 	virtual z_status evaluate1(zp_text_parser &parser, void* v) const  {return Z_ERROR_NOT_IMPLEMENTED;}
	virtual zf_feature* create_feature(ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc) const=0;
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

class zf_features : public 	 z_obj_vector_map<zf_feature>
{
public:
 	zf_feature* get_by_name(ctext t)   const;
} ;
class z_factory_dyn 
{
public:
	z_string _name;
	zf_features features;



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
	virtual void* create_default_obj() const=0;
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
	virtual void dump_obj_static(z_file& f,void* obj) const;
	z_status load_cfg(zp_text_parser &parser,void* obj) ;
	virtual void dump_static(z_file& f) const;
	virtual void add_features_recurse();

	virtual z_status get_list_features(z_strlist& list,void* obj);
	//___________________________________________________________________________________
	// Dynamic Functions 
	//___________________________________________________________________________________
	virtual z_status get_map_features(zf_feature_list&  list,
		zf_feature_type type,zf_feature_flags oper);

	virtual z_status execute_act(void* obj,ctext act_name,int* ret=0) const;
	virtual zf_action* add_act_params(ctext id,ctext name,z_memptr act_addr,zf_feature_flags flags,ctext desc,int num_params,...) ;

	virtual zf_feature* add_feature(const zf_var_funcs_base* vfuncs,ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc); 
	virtual zf_action* add_act(ctext id,ctext name,z_memptr act_addr,zf_feature_flags flags,ctext desc); 
	virtual zf_feature* add_prop(ctext id,ctext name,const zf_var_funcs_base* f,z_memptr act_addr,zf_feature_flags flags,ctext desc); 
	virtual zf_child_obj* add_obj(ctext id,ctext name,const zf_var_funcs_base* f,z_memptr act_addr,zf_feature_flags flags,ctext desc); 
	virtual zf_list* add_list(ctext id,ctext name,const zf_funcs_obj_list_base* f,z_memptr act_addr,zf_feature_flags flags,ctext desc); 
	zf_feature* get_feature_by_id(ctext id) ;
	zf_feature* get_feature(ctext name) ;
	z_status load_obj_contents(zp_text_parser &parser,void* pObj,zf_feature_flags oper) ;

	//___________________________________________________________________________________
	// Static Functions 
	//___________________________________________________________________________________
	z_status load_obj_contents_static(zp_text_parser &parser,void* obj) const;
	virtual void dump_obj_contents_static(z_file& f,void* obj) const;


};
class zf_obj
{
public:
    zf_obj()
	{
		_fact=0;	
		_obj=0;	
	}
    zf_obj(z_factory* f,void* o)
	{
		_fact=f;	
		_obj=o;	
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
	//typedef int (C::*fn_act)();
	typedef C THECLASS;
	z_factory_T()	 : z_factory()
	{			//STATIC
	}
	z_factory_T(ctext name)	 : z_factory()
	{
		//DYNAMIC
		init_dynamic();
		add_features_recurse();
		
		get_factories_dynamic().add(this);
	}

	static z_factory_T<C> &self;
	virtual void* create_default_obj() const {return z_new C(); }
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

	template <class OTHER > static void add_features(z_factory* factobj);
	virtual void add_features_recurse();

	virtual const z_factory_info& get_info() const;
	virtual ctext get_type_info_name()const 
	{
		C* nullobj=0;
		return Z_TYPEINFO_P(nullobj);
	}


};

z_factory*  zf_get_factory(ctext name);
z_factory*  zf_get_factory_by_type(ctext type);
z_status zf_create_obj_from_text_stream_static(zp_text_parser &parser, z_factory* &factory,void* &objpointer);
z_status zf_create_obj_from_text_stream_dyn(zp_text_parser &parser, z_factory* &factory,void* &objpointer,zf_feature_flags oper);


template <class CLASS> z_factory*  zf_get_factory_T()
{
	return &z_factory_T<CLASS>::self;

}


#define zp_offsetof_class(_class_,_member_)   (size_t)&reinterpret_cast<const volatile char&>((((_class_*)0)->_member_))

extern z_factory* _pgz_factory_none;
#define _gz_factory_none *_pgz_factory_none
#define __ZFACT(_CLASS_)   z_factory_T<_CLASS_> _gz_factory_##_CLASS_(#_CLASS_); \
	template <> z_factory_T<_CLASS_>& z_factory_T<_CLASS_>::self=_gz_factory_##_CLASS_;


#define ZFACT(_CLASS_) __ZFACT(_CLASS_);\
	const z_factory_info 	ZFACT##_CLASS_##INFO={ #_CLASS_,0,0,0,0 };\
	template <>  void z_factory_T<_CLASS_>	::add_features_recurse(){ add_features<_CLASS_>(this);};\
	template <>  const z_factory_info& z_factory_T<_CLASS_>::get_info() const{ return ZFACT##_CLASS_##INFO; } \
	 template <> template <class OTHER >   void z_factory_T<_CLASS_>::add_features(z_factory* factobj)

#define ZFACT_V(_CLASS_,_BASECLASS_)  __ZFACT(_CLASS_);\
	const z_factory_info 	ZFACT##_CLASS_##INFO={ #_CLASS_,&_gz_factory_##_BASECLASS_,0,0,0 };\
	template <>  const z_factory_info& z_factory_T<_CLASS_>::get_info() const{ return ZFACT##_CLASS_##INFO; } \
	template <>  void z_factory_T<_CLASS_>	::add_features_recurse(){ z_factory_T<_BASECLASS_>::add_features<_CLASS_>(this);add_features<_CLASS_>(this);};\
	 template <> template <class OTHER > static void z_factory_T<_CLASS_>::add_features(z_factory* factobj)

#include "zipolib/include/z_factory_features.h"

#endif

