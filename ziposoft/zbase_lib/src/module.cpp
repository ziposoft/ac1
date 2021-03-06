#include "zipobase.h"

#include "ds_text.h"
#include "ds_hamster.h"

ZFACT_PV(zb_ds_table)
{
	ZPROP(_id);
	ZACT_XP(close,"close",ZFF_ACT_DEF,"close",0,0);
	ZACT_XP(act_open,"open",ZFF_ACT_DEF,"open",0,0);
	ZACT_XP(commit,"commit",ZFF_ACT_DEF,"commit",0,0);
	ZACT_XP(act_dump_records,"dump",ZFF_ACT_DEF,"dump",0,0);
	ZACT_XP(act_add_field,"addfield",ZFF_ACT_DEF,"Add field",2,
		ZPARAM_X(_param_new_field_type,"new_field_type",ZFF_PARAM,"Type of new field"),
		ZPARAM_X(_param_field_name,"new_field_name",ZFF_PARAM,"Name of new field")
		);
	ZACT_XP(act_record_del,"del",ZFF_ACT_DEF,"Delete Record",1,
		ZPARAM_X(_param_record_index,"record_index",ZFF_PARAM,"Index of record to delete")
		);
	ZACT_XP(act_record_add,"add",ZFF_ACT_DEF,"Add Record",1,
		ZPARAM_X(_param_new_data,"new_data",ZFF_PARAM,"String to ")
		);
};

ZFACT_PV(zb_source)
{
	
	ZPROP(auto_open);
	ZPROP_X(_fullpath,"path",ZFF_PROP,"Path");
	ZPROP_X(_name,"name",ZFF_PROP,"DB Name");
	ZPROP_X(_ds_tables,"tbl",ZFF_PROP_NOLOAD,"Tables");
	ZACT_XP(act_table_new,"new_table",ZFF_ACT_DEF,"Create a new table",1,
		ZPARAM_X(_param_table_new_name,"tbl_name",ZFF_PARAM,"Name of new table")
		);
	ZACT_XP(act_open,"open",ZFF_ACT_DEF,"Open the DS",0,
		//ZPARAM_X(_param_db_name,"db_name",ZFF_PARAM,"Name of new DB")
		);
	ZACT_XP(commit,"commit",ZFF_ACT_DEF,"Commit the DS",0,0);
	ZACT_XP(close,"close",ZFF_ACT_DEF,"Close the DS",0,0);
	ZACT_XP(act_delete_datasource,"delete",ZFF_ACT_DEF,"Delete the DS",0,0);
};
ZFACT_V( zb_ds_table_txt, zb_ds_table)
{

};
ZFACT_V( zb_ds_text, zb_source)
{


};
ZFACT_V( zb_ds_ham_tbl, zb_ds_table)
{

};
ZFACT_V( zb_ds_ham, zb_source)
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
