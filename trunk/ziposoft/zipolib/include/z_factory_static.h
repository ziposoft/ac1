#ifndef z_obj_h
#define z_obj_h


#include "zipolib/include/z_factory.h"

#define ZP_MODULE(_NAME_) zp_module_##_NAME_ 
#define ZP_MODULE_DECLARE(_NAME_) extern const zp_module_entry ZP_MODULE(_NAME_);
#define ZP_MOD(_NAME_) &ZP_MODULE(_NAME_)


class z_factory_static;



#ifdef  _WIN64
#define zp_offsetof(m)   (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>(((nullobj)->m)) )
#else
#define zp_offsetof(m)   (size_t)&reinterpret_cast<const volatile char&>(((nullobj)->m))
#endif

 typedef  const zf_var_funcs_base* (*funcp_var_funcs_get)();
struct zf_var_entry
{
	const char* name;
	size_t offset;
	zf_feature_type type;  //currently not used for anything
	funcp_var_funcs_get fp_var_func_get;
};
class z_factory_static	: public z_factory
{

protected:
	virtual const zf_var_entry* get_var_list() const=0;
	virtual const zf_var_entry*  get_var_entry(ctext name) const;
	virtual const zf_var_entry*  get_var_entry(size_t index) const;


public:
	virtual const size_t get_var_list_size() const=0;


	virtual z_status get_var_info_i(size_t index,ctext& name,size_t &offset,const zf_var_funcs_base*& funcs) const;
	virtual z_status get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const;



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
	const  static z_factory_static_T<C> &self;
	virtual void* create_obj() const {return z_new C(); }
	virtual void delete_obj(void* v) const {delete reinterpret_cast<C*>(v);	}
 	virtual int execute_act_ptr(void* vobj,size_t act_addr) const
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
 #include "zipolib/include/z_factory_var_funcs.h"



/*________________________________________________________________________

GLOBAL z_factory_static functions
________________________________________________________________________*/
extern "C" {
extern const zp_module_entry *zp_module_master_list[];
extern const  int zp_module_master_list_size;
extern const zp_module_entry *zp_module_master_list_default[];
extern const  int zp_module_master_list_size_default;
}

 #define ZP_MODULE_INCLUDE(...) extern "C" { const zp_module_entry *zp_module_master_list[] = { __VA_ARGS__ }; \
 const int zp_module_master_list_size=sizeof(zp_module_master_list)/sizeof(void*);}
#pragma comment(linker, "/alternatename:_zp_module_master_list=_zp_module_master_list_default")
#pragma comment(linker, "/alternatename:_zp_module_master_list_size=_zp_module_master_list_size_default")

const z_factory_static*  zf_get_static_factory_by_name(ctext name);
void*  zfs_create_obj_by_type(ctext name);
void  zo_factory_list_dump();

template <class CLASS> void zf_dump_obj(CLASS* p_obj)
{
	const z_factory_static* factory=&z_factory_static_T<CLASS>::self;
	factory->dump_obj(gz_out,p_obj);

}
 template <class CLASS> void zf_dump_static(CLASS* p_obj)
{
	const z_factory_static* factory=&z_factory_static_T<CLASS>::self;
	factory->dump_static(gz_out);

}

#endif
