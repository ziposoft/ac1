#include "zipobase_lib.h"



ZFACT(zb_ds_table)
{
	ZPROP(_id);
	ZACT_XP(close,"close",ZFF_ACT_DEF,"close",0,0);
	ZACT_XP(act_open,"open",ZFF_ACT_DEF,"open",0,0);
	ZACT_XP(commit,"commit",ZFF_ACT_DEF,"commit",0,0);
	ZACT_XP(act_dump_records,"dump",ZFF_ACT_DEF,"dump",0,0);
	ZACT_XP(act_add_field,"addfield",ZFF_ACT_DEF,"Add field",2,
		ZPARAM(_param_new_field_type),ZPARAM(_param_new_field_name));
	ZACT_XP(act_record_del,"del",ZFF_ACT_DEF,"Delete Record",1,ZPARAM(_param_record_index));
	ZACT_XP(act_record_add,"add",ZFF_ACT_DEF,"Add Record",1,ZPARAM(_param_new_data));
};

ZFACT(zb_source)
{
	
	ZPROP_X(_param_db_name,"name",ZFF_PROP,"DB Name");
	ZPROP_X(_param_table_new_name,"new_table_name",ZFF_PROP,"Table Name");
	ZPROP_X(_ds_tables,"tbl",ZFF_PROP_NOLOAD,"Tables");
	ZACT_XP(act_table_new,"new_table",ZFF_ACT_DEF,"Create a new table",1,ZPARAM(_param_table_new_name));
	ZACT_XP(act_open,"open",ZFF_ACT_DEF,"Open the DB",1,ZPARAM(_param_db_name));
	ZACT_XP(commit,"commit",ZFF_ACT_DEF,"Commit the DB",0,0);
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
