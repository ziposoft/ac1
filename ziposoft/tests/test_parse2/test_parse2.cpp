

#include "zipolib_cpp/include/zipolib_cpp.h"

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
	z_string str;
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


struct zp_var_list
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
	virtual size_t get_var_list_size()=0;
	virtual zp_var_list* get_var_list()=0;
	zp_var_list* get_var_entry(ctext name)
	{
		int i;
		zp_var_list* list=get_var_list();
		for(i=0;i<get_var_list_size();i++)
			if(strcmp(name,	list[i].name)==0)
				return &list[i];
		return 0;
	}
	z_status set_var(void* obj,ctext var_name,ctext value)
	{
		zp_var_list* ent= get_var_entry(var_name);
		if(!ent)
			return -1; 
		const zp_var_funcs_base* funcs=ent->fp_var_func_get();
		char* pvar=(char*)obj+ent->offset;
		funcs->set(value,pvar);
		return z_status_success;

		

	}


};

 template <class C >  class zp_factory_T :public  zp_factory
 {
 public:
	 static C* create_new();
	size_t get_var_list_size();
	zp_var_list* get_var_list();	

 };


 #define ZO_OBJ_LIST \
	OBJ(testclass,void,"cmdline","parse",VAR(i) VAR(k)  VAR(j) VAR(str)) \
	OBJ(testclass2,void,"cmdline","parse",VAR(i) VAR(k)   VAR(str))

#include "macro.h"





//reinterpret_cast<const volatile char&>((((s *)0)->m))
int main()
{

//	const zp_var_funcs_base* ff=_zp_parse_testclass::__zp_var_funcs_get_i();

	zp_factory_T<testclass>	f;
	testclass t;
	void* pt=(void*)&t;
	size_t o=f.set_var(pt,"j","27");
	int x=t.j;

	return 0;
}
