#include "zb_pch.h"


#include "zbase_lib/include/zipobase.h"
#include "zipolib/include/z_parse_text.h"


z_status zb_validate_identifier(ctext text)
{

	if(strlen(text))
		return zs_ok ;
	return zs_error;

	//TODO - 
	static zp_text_parser p;
	p.set_source(text);
	if(p.test_any_identifier()!=zs_matched)
		return zs_syntax_error;
	if(p.eob())
		return zs_ok;
	return zs_error;
}

z_refcount::z_refcount()
{
	_ref_count=1;
}
void z_refcount::ref_inc()
{
	_ref_count++;
}

int z_refcount::ref_dec()
{
	_ref_count--;
	return _ref_count;
}
z_refcount*  z_obj_ptr_copy(z_refcount* p) { p->ref_inc(); return p; }
void z_obj_ptr_delete(z_refcount* p) 
{ 
	if(p->ref_dec()==0) 
		delete p; 
}





#if 0
#define Z_MODULE _Z_MODULE(zipobase)
#define ZO_OBJ_LIST \
	OBJV(zb_source,zp_obj_base,0,"Data source", 0 , \
		PROP(_name,"name",0,0,0) \
		ACT(open,0,0,"open",0,PRM("name"))\
		ACT(close,0,0,"close",0,NO_PRM)\
		LIST(_tables,"tables",NO_ID,NO_DESC,DEF_OPT) \
		)\
	OBJV(zb_table_base,zp_obj_base,0,"ZB Table", 0 , \
		PROP(_name,"name",0,0,0) \
		LIST(_desc,"desc",NO_ID,NO_DESC,DEF_OPT) \
		ACT(dump,0,NO_ID,NO_DESC,DEF_OPT,NO_PRM)\
		)\
	OBJV(zb_field,zp_obj_base,0,"ZB field", 0 , NO_FTR\
		)\
	OBJ(zb_table,zb_table_base,0,"ZB Table", 0 , NO_FTR\
		)\
	OBJ(zb_table_static,zb_table_base,0,"ZB Table", 0 , NO_FTR\
		)\
	OBJ(zb_ds_sl3,zb_source,"sqlite",0,"SQLite3", 0 , \
		PROP(_handle,"handle",NO_ID,NO_DESC,DEF_OPT) \
		PROP(_path,"path",NO_ID,NO_DESC,DEF_OPT) \
		PROP(_param_table_name,"table_name",NO_ID,NO_DESC,DEF_OPT) \
		ACT(get_table_info,0,NO_ID,NO_DESC,DEF_OPT,PRM("table_name"))\
		ACT(get_tables,0,NO_ID,NO_DESC,DEF_OPT,NO_PRM)\
		)



#include "zipolib/include/z_obj.macro"
#endif
