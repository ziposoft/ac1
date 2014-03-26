#ifndef DS_METAKIT_H
#define DS_METAKIT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"
#include "metakit/include/mk4.h"
#include "metakit/include/mk4str.h"


class zb_ds_metakit: public zb_source
{
public:
	zb_ds_metakit();
	virtual ~zb_ds_metakit();
	virtual z_status open(ctext name);
	virtual z_status close();
	virtual bool is_open();
};


#endif
