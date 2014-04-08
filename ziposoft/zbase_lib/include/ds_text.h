#ifndef DSTEXT_H
#define DSTEXT_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/datasource.h"



class zb_ds_text: public zb_source
{
	z_directory _dir;
	z_string _name;
public:
	zb_ds_text(ctext name);
	virtual ~zb_ds_text();
	virtual z_status open();
	virtual z_status close();
	//virtual zb_record* get_solo_record(zb_table_base* tbl)=0;
};


#endif