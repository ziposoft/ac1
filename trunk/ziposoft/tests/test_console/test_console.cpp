// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
#include "zipolib/include/z_factory_static.h"






class test2
{
public:
	test2()
	{
	  _name="defname";
	  _load="defload";
	}
  	z_string _name;
  	z_string _load;
};
class test1
{
public:
	test1()
	{

	}
	z_console console;
//	dog d;

	test2 a;
	test2 b;
};
class A
{
public:
	int _Aint;
	int othercrap()
	{

	}
	virtual z_status act1()
	{ 
		printf("act1");
		return 0;
	};
};

class D
{
public:
	int _Dint;
	virtual int Dothercrap()
	{
		return _Dint;

	}

};
class B : public A
{
public:
	int _Bint;
	virtual z_status act2()
	{ 
		printf("act2");
		return 0;
	};
	int othercrap2()
	{

	}
	z_console console;

};

class C: public B
{
public:
	int _Cint;
	virtual int Cothercrap()
	{
		return _Cint;
	}

};
ZFACT(A)
{
	ZACT(act1);
};
ZFACT_V(B,A)
{
	ZACT(act2);
};
ZFACT_V(C,B)
{
};

ZFACT(test2)
{
	ZPROP_X(_name,"name",ZFF_SAVE |ZFF_LIST|ZFF_SET ," no load ") ;
	ZPROP_X(_load,"l",ZFF_SAVE |ZFF_LIST|ZFF_SET|ZFF_LOAD ," no load ") ;

};
ZFACT(test1)
{
	ZOBJ_X(a,"a",ZFF_SAVE |ZFF_LIST|ZFF_SET ," load") ;
	ZOBJ_X(b,"b",ZFF_SAVE |ZFF_LIST|ZFF_SET|ZFF_LOAD ," load") ;
//	ZOBJ(d);
};


	
#ifdef BUILD_VX
int ztest()
{
	zout.set_handle((size_t)stdout);
	gz_in.set_handle((size_t)stdin);
	zout << "starting ztest\n";
	
	root o;
	o.console.setroot(&o);
	o.console.run();
	return 0;

}
#else
int main(int argc, char* argv[])
{

	//root o;
	C o;
	o.console.setroot(&o);

	o.console.runapp(argc,argv,false);


	return 0;
}

#endif




//ZP_MODULE_INCLUDE( ZP_MOD(parse), ZP_MOD(logger));
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")

