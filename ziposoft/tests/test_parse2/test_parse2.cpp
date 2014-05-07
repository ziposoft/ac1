

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




//reinterpret_cast<const volatile char&>((((s *)0)->m))
int main()
{
	zp_parser p;
	testclass t;


	p.parse_obj(&t,"heeelllo=4");

	return 0;
}



 #define ZO_OBJ_LIST \
 OBJ(testclass,void,"cmdline","{_val}ident:'=':{i}int",VAR(i) VAR(k)  VAR(j) VAR(_val)) \
	OBJ(testclass2,void,"cmdline","parse",VAR(i) VAR(k)   VAR(str))


#include "zipolib_parse2/include/z_obj_macro.h"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod));
