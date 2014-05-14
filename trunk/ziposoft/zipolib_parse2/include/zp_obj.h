#ifndef z_obj_h
#define z_obj_h

#include "zipolib_parse2/include/z_parse_pch.h"
#include "zipolib_parse2/include/z_parse_def.h"
#include "zipolib/include/z_stl_vector.h"


#define ZP_MODULE(_NAME_) zp_module_##_NAME_ 
#define ZP_MODULE_DECLARE(_NAME_) extern const zp_module_entry ZP_MODULE(_NAME_);
#define ZP_MOD(_NAME_) &ZP_MODULE(_NAME_)

 #define ZP_MODULE_INCLUDE(...) const zp_module_entry *zp_module_master_list[] = { __VA_ARGS__ };const int zp_module_master_list_size=sizeof(zp_module_master_list)/sizeof(void*);
class zp_factory;
 class zp_member_funcs_base
 {
 public:
	virtual void dump(z_file& s, void* v) const;
	virtual void get(z_string& s, void* v) const {};
	virtual void set(ctext s, void* v) const {};
	virtual void clear(void* v) const {} 
	virtual void add(void* list,void* obj) const {} 
	virtual void* get_item(void* list,size_t index) const { return 0;} 
	virtual size_t get_size(void* list) const { return 0;} 
	virtual void* create_obj(void* var,const zp_factory* new_child_type) const { return 0;}  /*could be pointer to obj, or porinter to obj pointer */
	virtual void* get_ptr(void* var,int* iter) const { return var;}  /*could be pointer to obj, or porinter to obj pointer */
			
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
	z_status create_child(void* obj,ctext var_name,const zp_factory* new_child_type,void** ppChild) const;
	z_status get_memvar_ptr(void* obj,ctext var_name,void** ppChild,int* iter=0) const;
	z_status set_var(void* obj,ctext var_name,ctext value) const;
	z_status get_var_as_string(void* obj,ctext var_name,z_string& value) const;
	void clear_all_vars(void* obj) const;
	void dump_obj(z_file& f,void* obj) const;
	void dump_obj_r(z_file& f,void* obj) const;
	void dump_static_r(z_file& f) const;
	virtual ctext get_parse_string() const{ return ""; }
	virtual ctext get_name()const =0;


};
struct zp_obj
{
	const zp_factory* _fact;
	void* _obj;
};
class zp_obj_vector  : public std::vector<zp_obj>
{
public:


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
	const  static zp_factory_T<C> &obj;
	virtual void* create_obj() const;
	const size_t get_var_list_size() const;
	const zp_var_entry* get_var_list() const;	
	virtual ctext get_parse_string() const;
	virtual ctext get_name() const;
 };

template <class CLASS> void z_fact_dump(CLASS* p_obj)
{
	const zp_factory* factory=&zp_factory_T<CLASS>::obj;
	factory->dump_obj(gz_out,p_obj);

}

 
 /*
 This inteface manipulates simple member variables 
 */
 template <class VAR >  class zp_var_funcs  : public zp_member_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 	virtual void clear(void* v) const;
	virtual void add(void* list,void* obj) const ;
	virtual void* get_item(void* list,size_t index) const;
	virtual size_t get_size(void* list) const;
	virtual void dump(z_file& s, void* v) const;
	virtual void* create_obj(void* var,const zp_factory* new_child_type) const;
};
 template <class TYPE >  class zp_var_list_funcs  : public zp_member_funcs_base
 {
 public:
	virtual void* create_obj(void* v,const zp_factory* fact) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		TYPE* obj=reinterpret_cast<TYPE*>(fact->create_obj());
		list.add(obj);
		return obj;
	}
	virtual size_t get_size(void* v) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		return list.size();
	}
	void dump(z_file& f, void* v) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		size_t count=list.size();
		if(!count)
		{
			f << "{}";
			return;
		}
		size_t i;
		f << "{\n";
		f.indent_inc();
		for(i=0;i<count;i++)
		{
			TYPE* obj=list[i];
			zp_factory_T<TYPE>::obj.dump_obj_r(f,obj);
		}
		f.indent_dec();
		f.indent();
		f << "}";
	}
	virtual void* get_ptr(void* v,int* iter ) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		if(!iter)
		{
			Z_ERROR_MSG(zs_bad_parameter,"Objects type does not match member variable");
			return 0;
		}
		if(*iter==-1)
			*iter=0;
		else 
		{
			(*iter)++;
			if(*iter>=(int)list.size())
			{
				*iter=-1;
				return 0;
			}
		}
	
			
		TYPE* obj=list[*iter];
		return obj;
	}
};

 template <class VAR >  const zp_member_funcs_base* zp_var_funcs_get(VAR& item)
 {
	static const zp_var_funcs<VAR> f;
	return &f;
 };
 template <class VAR >  const zp_member_funcs_base* zp_var_funcs_get(z_obj_vector<VAR>& list)
 {
	static const zp_var_list_funcs<VAR> f;
	return &f;
 };
 /*
 This inteface manipulates child objects 
 */
template <class CLASS >  class zp_child_obj_funcs  : public zp_member_funcs_base
{
 public:
	virtual void* create_obj(void* var /* pointer to obj */,const zp_factory* new_child_type) const
	{
		//OBJ instance is part of parent, so it is already created.
		//Just reset it and return a pointer to it.
		const zp_factory* f=&zp_factory_T<CLASS>::obj;
		if(new_child_type!=f)
		{
			Z_ERROR_MSG(zs_wrong_object_type,"Objects type does not match member variable");
			return 0;
		}
		f->clear_all_vars(var); 
		return var;
	}
	virtual void* get_ptr(void* var,int* iter ) const
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
	virtual void* get_ptr(void* var,int* iter ) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	}
	virtual void* create_obj(void* var /* pointer to obj pointer*/,const zp_factory* new_child_type) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 

		const zp_factory* f=&zp_factory_T<CLASS>::obj;
		if(new_child_type!=f)
		{
			Z_ERROR_MSG(zs_wrong_object_type,"Objects type does not match member variable");
			return 0;
		}

		*ppObj=f->create_obj();
		return *ppObj;
	}
	virtual void dump(z_file& file, void* v) const
	{
		void** ppObj=reinterpret_cast<void**>(v); 
		if(*ppObj == 0)
			file<< "NULL";
		else

			zp_factory_T<CLASS>::obj.dump_obj_r(file,*ppObj);
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


/*________________________________________________________________________

GLOBAL zp_factory functions
________________________________________________________________________*/
const zp_factory*  zo_get_factory_by_name(ctext name,size_t len=-1);
const zp_factory*  zo_get_factory(ctext name);
void  zo_factory_list_dump();
extern const zp_module_entry *zp_module_master_list[];
extern const int zp_module_master_list_size;

#endif
