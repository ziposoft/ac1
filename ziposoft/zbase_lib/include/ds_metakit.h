#ifndef DS_METAKIT_H
#define DS_METAKIT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"
#include "metakit/include/mk4.h"
#include "metakit/include/mk4str.h"


class zb_ds_metakit: public zb_source
{

    c4_Storage      *_pStore;
	z_string		_filename;
public:
	zb_ds_metakit(ctext name);
	virtual ~zb_ds_metakit();
	virtual z_status open(bool writable);
	virtual z_status close();
	virtual z_status commit();
	virtual bool is_open();
};


#endif
