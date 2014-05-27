#ifndef z_obj_h
#define z_obj_h


#include "zipolib/include/z_factory.h"

#define ZP_MODULE(_NAME_) zp_module_##_NAME_ 
#define ZP_MODULE_DECLARE(_NAME_) extern const zp_module_entry ZP_MODULE(_NAME_);
#define ZP_MOD(_NAME_) &ZP_MODULE(_NAME_)





#ifdef  _WIN64
#define zp_offsetof(m)   (size_t)( (ptrdiff_t)&reinterpret_cast<const volatile char&>(((nullobj)->m)) )
#else
#define zp_offsetof(m)   (size_t)&reinterpret_cast<const volatile char&>(((nullobj)->m))
#endif



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
	const z_factory* fact;
};

struct zp_module_entry 
{
	ctext name;
	zp_module_fact_entry *facts;
	const int num_facts;
};


 #include "zipolib/include/z_factory_var_funcs.h"



/*________________________________________________________________________

GLOBAL z_factory functions
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

const z_factory*  zf_get_static_factory_by_name(ctext name);
void*  zfs_create_obj_by_type(ctext name);
void  zo_factory_list_dump();

template <class CLASS> void zf_dump_obj(CLASS* p_obj)
{
	const z_factory* factory=&z_factory_T<CLASS>::self;
	factory->dump_obj(gz_out,p_obj);

}
 template <class CLASS> void zf_dump_static(CLASS* p_obj)
{
	const z_factory* factory=&z_factory_T<CLASS>::self;
	factory->dump_static(gz_out);

}

#endif
