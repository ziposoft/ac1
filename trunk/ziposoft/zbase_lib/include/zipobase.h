#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"

Z_MODULE_DEF(zipobase);

class zb_zipobase : public z_obj
{
public:
	ZO_OBJ_H;
	zb_zipobase()
	{

	}
	virtual z_status open(zb_source* ds);
	virtual z_status close();
};

