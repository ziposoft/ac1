#include  "test_cpp.h"

typedef unsigned  long long z_memptr;

class AA
{
public:
	int i;
	AA()
	{
		i=1;
	}
	virtual  void Afunc1()
	{
		printf("Afunc1 %d \n",i);
	}

};

class BB
{
public:
	int ib;
	BB()
	{
		ib=2;
	}
	virtual  void Bfunc1()
	{
		printf("Bfunc1 %d \n",ib);
	}

};

class CC : public AA,public BB
{
public:
	CC()
	{
		i=3;
	}
	virtual  void Cfunc1()
	{
		printf("Cfunc1 %d \n",i);
	}
};

class z_factory
{
public:
	virtual z_memptr get_ptr()=0;

};

template <class C >  class z_factory_T :public  z_factory
{
public:
	friend  C;
	typedef void  (C::*fn_act)();
	typedef C THECLASS;
	z_factory_T()	 : z_factory()
	{			//STATIC
	}

	static z_factory_T<C> &self;

	virtual int execute_act_ptr(void* vobj,z_memptr act_addr) const
	{
		typedef int (C::*funcptr)();
		C*  cobj=reinterpret_cast<C*>(vobj);
		z_memptr* pp=&act_addr;
		funcptr fp=*( funcptr*) (pp);
		return (cobj->*fp)();
	}
	virtual z_memptr get_ptr();



};


template <> z_memptr z_factory_T<AA>::get_ptr()
{
	fn_act f=&AA::Afunc1;
	printf("f=%p\n",f);

	z_memptr add= *(z_memptr*)&f;
	printf("add=%lx\n",add);
	return add;
}
template <> z_memptr z_factory_T<BB>::get_ptr()
{
	fn_act f=&BB::Bfunc1;
	z_memptr add= *(z_memptr*)&f;
	return add;
}
template <> z_memptr z_factory_T<CC>::get_ptr()
{
	fn_act f=&CC::Afunc1;
	printf("f=%p\n",f);

	z_memptr add= *(z_memptr*)&f;
	printf("add=%lx\n",add);
	return add;
}
void test_func_ptrs()
{
	AA a;
	BB b;
	CC c;
	z_factory_T<AA> fa;
	z_factory_T<BB> fb;
	z_factory_T<CC> fc;
	a.i=55;
	z_memptr ap=fa.get_ptr();
	printf("ap=%lx\n",ap);

	fa.execute_act_ptr((void*)&a,ap);

	z_memptr cp=fc.get_ptr();
	printf("cp=%lx\n",cp);

	fc.execute_act_ptr((void*)&c,cp);












}