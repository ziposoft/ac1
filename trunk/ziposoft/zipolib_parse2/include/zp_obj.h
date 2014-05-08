#ifndef z_obj_h
#define z_obj_h

#include "zipolib_parse2/include/z_parse_pch.h"
#include "zipolib_parse2/include/z_parse_def.h"


#define ZP_MODULE(_NAME_) zp_module_##_NAME_ 
#define ZP_MODULE_DECLARE(_NAME_) extern const zp_module_entry ZP_MODULE(_NAME_);
#define ZP_MOD(_NAME_) &ZP_MODULE(_NAME_)

 #define ZP_MODULE_INCLUDE(...) const zp_module_entry *zp_module_master_list[] = { __VA_ARGS__ };const int zp_module_master_list_size=sizeof(zp_module_master_list)/sizeof(void*);
class zp_factory;
 class zp_member_funcs_base
 {
 public:
	virtual void dump(z_file& s, void* v,int& depth) const;
	virtual void get(z_string& s, void* v) const {};
	virtual void set(ctext s, void* v) const {};
	virtual void clear(void* v) const {} 
	virtual void* reset_create_obj(void* var) const { return 0;}  /*could be pointer to obj, or porinter to obj pointer */
	virtual void* get_opj_ptr(void* var) const { return 0;}  /*could be pointer to obj, or porinter to obj pointer */
			
 } ;
 typedef  const zp_member_funcs_base* (*funcp_var_funcs_get)();



struct zp_var_entry
{
	const char* name;
	size_t offset;
	funcp_var_funcs_get fp_var_func_get;
};

#ifdef  _WIN64
#define zp_offsetof(m)   (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>(((nullobj)->m)) )
#else
#define zp_offsetof(m)   (size_t)&reinterpret_cast<const volatile char&>(((nullobj)->m))
#endif


class zp_factory
{
public:
	virtual void* create_obj() const=0;
	virtual const size_t get_var_list_size() const=0;
	virtual const zp_var_entry* get_var_list() const=0;
	const zp_var_entry* get_var_entry(ctext name) const;
	z_status get_new_child_obj_ptr(void* obj,ctext var_name,void** ppChild) const;
	z_status get_child_obj_ptr(void* obj,ctext var_name,void** ppChild) const;
	z_status set_var(void* obj,ctext var_name,ctext value) const;
	z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const;
	void clear_all_vars(void* obj) const;
	void dump_obj(z_file& f,void* obj) const;
	void dump_obj_r(z_file& f,void* obj,int& depth) const;
	virtual ctext get_parse_string() const{ return ""; }
	virtual ctext get_name()const =0;


};
struct zp_module_fact_entry 
{
	ctext name;
	const zp_factory* fact;
};

struct zp_module_entry 
{
	ctext name;
	zp_module_fact_entry *facts;
	const int num_facts;
};
const zp_factory*  zo_get_factory_by_name(ctext name,size_t len=-1);
const zp_factory*  zo_get_factory(ctext name);
extern const zp_module_entry *zp_module_master_list[];
extern const int zp_module_master_list_size;
 template <class C >  class zp_factory_T :public  zp_factory
 {
 public:
	const  static zp_factory_T<C> static_instance;
	virtual void* create_obj() const;
	const size_t get_var_list_size() const;
	const zp_var_entry* get_var_list() const;	
	virtual ctext get_parse_string() const;
	virtual ctext get_name() const;
 };

class zo_str_container
{
public:
	virtual ctext get_next()=0;
	virtual void add(ctext x)=0;
	virtual void clear()=0;
	virtual size_t count()=0;
	virtual void reset_iter()=0;
	virtual void copy(zo_str_container& other);

};
class zo_str_vect : public z_strlist,public zo_str_container
{
public:
	U32 _iter;
	zo_str_vect()
	{
		_iter=0;
	}
    virtual void add(ctext x)
    {
		push_back(x);
    };
	virtual void clear() 
	{ 
		z_strlist::clear();
	}
	virtual size_t count() 
	{ 
		return size();
	}
	virtual void reset_iter() 
	{ 
		_iter=0;
	}
	virtual ctext get_next()
	{
		if(_iter>=size())
			return 0;
		ctext t= (*this)[_iter];
		_iter++;
		return t;
	}   
};
class zo_str_map : public z_stl_map<z_string,int>,public zo_str_container
{
public:
	zo_str_map()
	{
		iter=begin();

	}
	virtual zo_str_map & operator << (ctext s) { add(s); return *this; }

    virtual void add(ctext x)
    {
		Z_ASSERT(x);
        (*this)[x]=0;
    };

	z_stl_map<z_string,int>::iterator iter;
	virtual void reset_iter() 
	{ 
		iter=begin();
	}
	virtual void clear() 
	{ 
		z_stl_map<z_string,int>::clear();
	}
	virtual ctext get_next()
	{
		if(iter==end())
			return 0;
		const z_string& str=iter->first;
		iter++;
		return str.c_str();
	}   
	virtual size_t count() 
	{ 
		return size();
	}
};

 
 /*
 This inteface manipulates simple member variables 
 */
 template <class VAR >  class zp_var_funcs  : public zp_member_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 	virtual void clear(void* v) const;
};
 template <class VAR >  const zp_member_funcs_base* zp_var_funcs_get(VAR& item)
 {
	static const zp_var_funcs<VAR> f;
	return &f;
 };
 /*
 This inteface manipulates child objects 
 */
template <class CLASS >  class zp_child_obj_funcs  : public zp_member_funcs_base
{
 public:
	virtual void* reset_create_obj(void* var /* pointer to obj */) const
	{
		zp_factory_T<CLASS>::static_instance.clear_all_vars(var); 
		return var;
	}
	virtual void* get_opj_ptr(void* var ) const
	{
		return var;
	}
};
 template <class CLASS >  const zp_member_funcs_base* zp_child_obj_funcs_get(CLASS& obj)
 {
	static const zp_child_obj_funcs<CLASS> f;
	return &f;
 };
  /*
 This inteface manipulates child object pointers 
 */
template <class CLASS >  class zp_child_pobj_funcs  : public zp_member_funcs_base
{
 public:
	virtual void* get_opj_ptr(void* var ) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	}
	virtual void* reset_create_obj(void* var /* pointer to obj pointer*/) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		//if(*ppObj) 	delete *ppObj;
		*ppObj=zp_factory_T<CLASS>::static_instance.create_obj();
		return *ppObj;
	}
	virtual void dump(z_file& file, void* v,int& depth) const
	{
		void** ppObj=reinterpret_cast<void**>(v); 
		if(*ppObj == 0)
			file<< "NULL";
		else

			zp_factory_T<CLASS>::static_instance.dump_obj_r(file,*ppObj,depth);
	}

 	virtual void get(z_string& s, void* v) const
	{
		
		s="???";


	}


};
 template <class CLASS >  const zp_member_funcs_base* zp_child_pobj_funcs_get(CLASS*& obj)
 {
	static const zp_child_pobj_funcs<CLASS> f;
	return &f;
 };
  /*
 This is custom HEX interface 
 */
  template <class VAR >  class zp_var_funcs_hex  : public zp_member_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 };


#endif
