#ifndef z_obj_h
#define z_obj_h


#include "zipolib/include/z_factory.h"

#define ZP_MODULE(_NAME_) zp_module_##_NAME_ 
#define ZP_MODULE_DECLARE(_NAME_) extern const zp_module_entry ZP_MODULE(_NAME_);
#define ZP_MOD(_NAME_) &ZP_MODULE(_NAME_)

 #define ZP_MODULE_INCLUDE(...) const zp_module_entry *zp_module_master_list[] = { __VA_ARGS__ };const int zp_module_master_list_size=sizeof(zp_module_master_list)/sizeof(void*);
class z_factory_static;



#ifdef  _WIN64
#define zp_offsetof(m)   (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>(((nullobj)->m)) )
#else
#define zp_offsetof(m)   (size_t)&reinterpret_cast<const volatile char&>(((nullobj)->m))
#endif


class z_factory_static	: public z_factory
{

protected:
	virtual const size_t get_var_list_size() const=0;
	virtual const zf_var_entry* get_var_list() const=0;
	virtual const zf_var_entry*  get_var_entry(ctext name) const;

public:
	virtual z_status get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const;

	virtual z_status execute_act(void* obj,ctext act_name,int* ret=0) const;
	virtual int execute_act_ptr(void* obj,void*  act_addr) const=0;
	void clear_all_vars(void* obj) const;
	void dump_obj(z_file& f,void* obj) const;
	void dump_static(z_file& f) const;
	virtual ctext get_parse_string() const{ return ""; }
	virtual ctext get_name()const =0;


};
struct zp_obj
{
	const z_factory* _fact;
	void* _obj;
};
class zp_obj_vector  : public std::vector<zp_obj>
{
public:
};

struct zp_module_fact_entry 
{
	ctext name;
	const z_factory_static* fact;
};

struct zp_module_entry 
{
	ctext name;
	zp_module_fact_entry *facts;
	const int num_facts;
};

 template <class C >  class z_factory_static_T :public  z_factory_static
 {
 public:
	const  static z_factory_static_T<C> &obj;
	virtual void* create_obj() const;
	virtual void delete_obj(void* v) const
	{
		delete reinterpret_cast<C*>(v);
	}
 	virtual int execute_act_ptr(void* vobj,void* act_addr) const
	{
		typedef int (C::*funcptr)();
		C*  cobj=reinterpret_cast<C*>(vobj);
		funcptr fp=*( funcptr*) (act_addr);
		return (cobj->*fp)();
	}
	const size_t get_var_list_size() const;
	const zf_var_entry* get_var_list() const;	
	virtual ctext get_parse_string() const;
	virtual ctext get_name() const;
 };



 
 /*
 This interface manipulates simple member variables 
 */
 template <class VAR >  class zf_var_funcs  : public zf_var_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 	virtual void clear(void* v) const;
	virtual void add(void* list,void* obj) const ;
	virtual void* get_item(void* list,size_t index) const;
	virtual size_t get_size(void* list) const;
	virtual void dump(z_file& s, void* v) const;
	virtual void* create_obj(void* var,const z_factory* fact) const;
};	
class zp_var_list_funcs_base  : public zf_var_funcs_base
 {
 public:
 	virtual void clear(void* v) const;

	virtual const z_factory* get_fact() const=0;
	virtual z_obj_vector_base* get_list(void * v) const=0;
	void dump(z_file& f, void* v) const;
	virtual void* get_ptr(void* v,int* iter ) const;
};

 /*
 WARNING- overloaded funcs must match exactly! otherwise they will quietly not be called */
 template <class TYPE >  class zp_var_list_funcs  : public zp_var_list_funcs_base
 {
 public:
	virtual const z_factory* get_fact()	const
	{
		return &z_factory_static_T<TYPE>::obj;
	}

	virtual void* create_obj(void* v,const z_factory* fact) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		TYPE* obj=reinterpret_cast<TYPE*>(fact->create_obj());
		list.add(obj);
		return obj;
	}

	virtual z_obj_vector_base* get_list(void* v ) const
	{
		z_obj_vector<TYPE>* list= reinterpret_cast<z_obj_vector<TYPE>*>(v);
		return list;
	}
};

 template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(VAR& item)
 {
	static const zf_var_funcs<VAR> f;
	return &f;
 };
 template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(z_obj_vector<VAR>& list)
 {
	static const zp_var_list_funcs<VAR> f;
	return &f;
 };
 /*
 This inteface manipulates child objects 
 */
template <class CLASS >  class zp_child_obj_funcs  : public zf_var_funcs_base
{
 public:
	virtual void* create_obj(void* var /* pointer to obj */,const z_factory* new_child_type) const
	{
		//OBJ instance is part of parent, so it is already created.
		//Just reset it and return a pointer to it.
		const z_factory_static* f=&z_factory_static_T<CLASS>::obj;
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
 	virtual void clear(void* v) const{
		const z_factory_static* f=&z_factory_static_T<CLASS>::obj;
		f->clear_all_vars(v);
	}
};
 template <class CLASS >  const zf_var_funcs_base* zp_child_obj_funcs_get(CLASS& obj)
 {
	static const zp_child_obj_funcs<CLASS> f;
	return &f;
 };
  /*
 This interface manipulates child object pointers 
 */
template <class CLASS >  class zp_child_pobj_funcs  : public zf_var_funcs_base
{
 public:
	virtual void* get_ptr(void* var,int* iter ) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	}


	virtual void* create_obj(void* var /* pointer to obj pointer*/,const z_factory* new_child_type) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 

		const z_factory_static* f=&z_factory_static_T<CLASS>::obj;
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

			z_factory_static_T<CLASS>::obj.dump_obj(file,*ppObj);
	}

 	virtual void get(z_string& s, void* v) const
	{
		s="???";
	}
 	virtual void clear(void* v) const{
		CLASS** ppObj=reinterpret_cast<CLASS**>(v); 
		if(*ppObj )
			delete *ppObj;
		*ppObj=0;
	}


};
 template <class CLASS >  const zf_var_funcs_base* zp_child_pobj_funcs_get(CLASS*& obj)
 {
	static const zp_child_pobj_funcs<CLASS> f;
	return &f;
 };
  /*
 This is custom HEX interface 
 */
  template <class VAR >  class zp_var_funcs_hex  : public zf_var_funcs_base
 {
 public:
 	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
 };


/*________________________________________________________________________

GLOBAL z_factory_static functions
________________________________________________________________________*/
const z_factory_static*  zfs_get_factory_by_name(ctext name,size_t len=-1);
const z_factory_static*  zfs_get_factory(ctext name);
void*  zfs_create_obj_by_type(ctext name);
void  zo_factory_list_dump();
extern const zp_module_entry *zp_module_master_list[];
extern const int zp_module_master_list_size;
template <class CLASS> void zf_dump_obj(CLASS* p_obj)
{
	const z_factory_static* factory=&z_factory_static_T<CLASS>::obj;
	factory->dump_obj(gz_out,p_obj);

}
 template <class CLASS> void zf_dump_static(CLASS* p_obj)
{
	const z_factory_static* factory=&z_factory_static_T<CLASS>::obj;
	factory->dump_static(gz_out);

}

#endif
