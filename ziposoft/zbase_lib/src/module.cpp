#include "zipobase_lib.h"


ZFACT(zb_ds_table)
{
	ZPROP(_id);
};


ZFACT(zb_source)
{
	
	ZPROP(_name);
	ZPROP(_ds_tables);
	ZACT_XP(table_new,"table_new","Create a new table",1,ZPARAM(table_new_name));
};

ZFACT_V( zb_ds_text, zb_source)
{};


/*
ZFACT_V(zb_source,z_intf_random_access)
{
	ZPROP(filename);
	

};
*/


/* STATIC FACTORY 	  ???

#define ZO_OBJ_LIST \
	ZCLS(zb_source,none,"","",  \
		VAR(_name) VAR(_ds_tables))    \
		VAR(table_new_name)	 \
		ACT_XP(table_new,"table_new","Create a new table",)	 \
	ZCLS(zb_ds_table,none,"","",  VAR(_id)    ) 


#include "zipolib/include/z_obj.inc"

ZP_MODULE_DEFINE(zipobase);


*/
