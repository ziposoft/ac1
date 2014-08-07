#include "zipobase_lib.h"


#define ZO_OBJ_LIST \
	ZCLS(zb_source,none,"","",  VAR(_name)  VAR(_ds_tables)  ) \
	ZCLS(zb_ds_table,none,"","",  VAR(_id)    ) 


#include "zipolib/include/z_obj.inc"

ZP_MODULE_DEFINE(zipobase);

