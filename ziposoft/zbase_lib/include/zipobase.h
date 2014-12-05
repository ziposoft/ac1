#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"

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

};
class zb_zipobase //: public zp_obj_base
{
public:
	zb_zipobase()
	{

	}
	virtual z_status open(zb_source* ds);
	virtual z_status close();
	z_map<zb_table_base> _tables;
};
