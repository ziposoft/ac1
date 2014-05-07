

#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_parse2/include/z_parse.h"



class testclass 
{
public:
	testclass()
	{
	   i=0;
	   j=0;
	   k=0;
	}
	int i;
	z_string _val;
	int j;
	int k;
};
class testclass2 
{
public:
	testclass2()
	{
	   i=0;
	   k=0;
	}
	int i;
	z_string str;
	int k;
};


 #define ZO_OBJ_LIST \
 OBJ(testclass,void,"cmdline","{_val}ident:'=':{i}int",VAR(i) VAR(k)  VAR(j) VAR(_val)) \
	OBJ(testclass2,void,"cmdline","parse",VAR(i) VAR(k)   VAR(str))


#include "zipolib_parse2/include/z_obj_macro.h"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod));


//reinterpret_cast<const volatile char&>((((s *)0)->m))
int main()
{
	zp_parser p;
	testclass t;


	p.parse_obj(&t,"heeelllo=4");
//	const zp_var_funcs_base* ff=_zp_parse_testclass::__zp_var_funcs_get_i();
	const zp_module_entry* me=&ZP_MODULE(testmod);
	zp_factory_T<testclass>	f;
	void* pt=(void*)&t;
	size_t o=f.set_var(pt,"j","27");
	int x=t.j;

	return 0;
}
