#include "zbase_lib/include/zipobase.h"


ctext zb_status_text[]=
{
	"ok",//zb_ok,
	"zb_end_of_data",//zb_end_of_data,
	"zb_bad_param",//zb_bad_param,
	"zb_cant_open_file",//zb_cant_open_file,
	"zb_unknown_data_error",//zb_unknown_data_error,
	"zb_data_source_not_open",//zb_data_source_not_open,

};

#define Z_MODULE _Z_MODULE(zipobase)
#define ZO_OBJ_LIST \
	OBJV(zb_source,z_obj,0,"Data source", 0 , \
		PROP(_name,"name",0,0,0) \
		ACT(open,0,0,"open",0,PRM("name"))\
		ACT(close,0,0,"close",0,NO_PRM)\
		LIST(_tables,"tables",NO_ID,NO_DESC,DEF_OPT) \
		)\
	OBJV(zb_table_base,z_obj,0,"ZB Table", 0 , \
		PROP(_name,"name",0,0,0) \
		LIST(_desc,"desc",NO_ID,NO_DESC,DEF_OPT) \
		ACT(dump,0,NO_ID,NO_DESC,DEF_OPT,NO_PRM)\
		)\
	OBJV(zb_field,z_obj,0,"ZB field", 0 , NO_FTR\
		)\
	OBJ(zb_table,zb_table_base,0,"ZB Table", 0 , NO_FTR\
		)\
	OBJ(zb_table_static,zb_table_base,0,"ZB Table", 0 , NO_FTR\
		)\
	OBJ(zb_src_sl3,zb_source,"sqlite","SQLite3", 0 , \
		PROP(_handle,"handle",NO_ID,NO_DESC,DEF_OPT) \
		PROP(_path,"path",NO_ID,NO_DESC,DEF_OPT) \
		PROP(_param_table_name,"table_name",NO_ID,NO_DESC,DEF_OPT) \
		ACT(get_table_info,0,NO_ID,NO_DESC,DEF_OPT,PRM("table_name"))\
		ACT(get_tables,0,NO_ID,NO_DESC,DEF_OPT,NO_PRM)\
		)



#include "zipolib/include/z_obj_macro.h"