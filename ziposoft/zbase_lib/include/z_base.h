#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"


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