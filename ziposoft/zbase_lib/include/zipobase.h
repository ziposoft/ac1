#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"


class zb_zipobase //: public z_obj
{
public:
	zb_zipobase()
	{

	}
	virtual zb_status open(zb_source* ds);
	virtual zb_status close();
};

