#include "zipobase_lib.h"


ZFACT(zb_ds_table)
{
	ZPROP(_id);
	ZACT_XP(add_field,"add_field",0,"Add field",1,ZPARAM(_new_field_name));
};


ZFACT(zb_source)
{
	
	ZPROP_X(_param_db_name,"name",ZFF_PROP,"DB Name");
	ZPROP_X(_param_table_new_name,"new_table_name",ZFF_PROP,"Table Name");
	ZPROP_X(_ds_tables,"tables",ZFF_PROP_NOLOAD,"Tables");
	ZACT_XP(act_table_new,"new_table",0,"Create a new table",1,ZPARAM(_param_table_new_name));
	ZACT_XP(act_open,"open",0,"Open the DB",1,ZPARAM(_param_db_name));
	ZACT_XP(commit,"commit",0,"Commit the DB",0,0);
};
ZFACT_V( zb_ds_table_txt, zb_ds_table)
{

};
ZFACT_V( zb_ds_text, zb_source)
{


};


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
