#ifndef z_obj_h
#define z_obj_h

#include "zipolib_parse2/include/z_parse_pch.h"
#include "zipolib_parse2/include/z_parse_def.h"


#define ZP_MODULE(_NAME_) zp_module_##_NAME_ 
#define ZP_MODULE_DECLARE(_NAME_) extern const zp_module_entry ZP_MODULE(_NAME_);
 class zp_var_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const=0;
	virtual void set(ctext s, void* v) const=0;
 } ;
 template <class VAR >  class zp_var_funcs  : public zp_var_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 };
  template <class VAR >  class zp_var_funcs_hex  : public zp_var_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 };
 void zp_var_funcs<int>::get(z_string& s, void* v) const{    }
 void zp_var_funcs<int>::set(ctext s, void* v) const{int* i= reinterpret_cast<int*>(v); *i=atoi(s);    }
 void zp_var_funcs<z_string>::get(z_string& s, void* v) const{    }
 void zp_var_funcs<z_string>::set(ctext s, void* v) const{z_string* vs= reinterpret_cast<z_string*>(v); *vs=s;   }
 

typedef  const zp_var_funcs_base* (*funcp_var_funcs_get)();



 template <class VAR >  const zp_var_funcs_base* zp_var_funcs_get(VAR& item)
 {
	static const zp_var_funcs<VAR> f;
	return &f;
 };


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
	virtual size_t get_var_list_size()=0;
	virtual zp_var_entry* get_var_list()=0;
	zp_var_entry* get_var_entry(ctext name)
	{
		size_t i;
		zp_var_entry* list=get_var_list();
		for(i=0;i<get_var_list_size();i++)
			if(strcmp(name,	list[i].name)==0)
				return &list[i];
		return 0;
	}
	z_status set_var(void* obj,ctext var_name,ctext value)
	{
		zp_var_entry* ent= get_var_entry(var_name);
		if(!ent)
			return -1; 
		const zp_var_funcs_base* funcs=ent->fp_var_func_get();
		char* pvar=(char*)obj+ent->offset;
		funcs->set(value,pvar);
		return z_status_success;
	}
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
 template <class C >  class zp_factory_T :public  zp_factory
 {
 public:
	virtual void* create_obj() const;
	size_t get_var_list_size();
	zp_var_entry* get_var_list();	
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


#endif
