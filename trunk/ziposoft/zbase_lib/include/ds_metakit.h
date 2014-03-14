#ifndef DSTEXT_H
#define DSTEXT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"



class zb_ds_text: public zb_source
{
public:
	zb_ds_text();
	virtual ~zb_ds_text();
	virtual zb_status open(ctext name);
	virtual zb_status close();
	virtual bool is_open();
};


#endif