#ifndef z_obj_man_h
#define z_obj_man_h
#include "zipolib_parse/include/zp_obj.h"

class zp_obj_parse;
class zp_obj_parser;
class zo_manipulator
{
	int     _dump_depth;
	bool    _dump_new_line;
	zp_obj_base*   _p_member_var_obj;
	z_string _member_var_string_data;
	int      _member_var_int_data;   
	z_obj_container* _member_obj_container;
	size_t _action_return_value;

protected:
	z_file* _dump_fp;
	type_zo_opt_security _security_level;
	type_zo_opt_ui _ui_level;
public:

	const zo_action_params* _p_member_action_params;
	zo_manipulator();
	




	void get_feature_map_by_fact(
		zo_feature_list& list,
		const z_obj_fact* fact,
		U32 feature_type,
		bool include_alias
		);
/*
	void get_feature_list_by_fact(
		z_strlist& list,
		const z_obj_fact* fact,
		U32 feature,
		zp_obj_base* obj //If obj is passed in, append values
		);
	void get_feature_list(
		z_strlist& list,
		U32 feature,
		zp_obj_base* obj 
		);
		*/
	zo_ftr_entry* get_next_feature(
			const z_obj_fact* fact,
			U32 feature_type,
			int &index
			);
/*
	z_status var_set_from_text(zp_obj_base* p_obj,ctext var_id,ctext txt);
	z_status var_set_from_string(zp_obj_base* p_obj,ctext var_id);
*/	
	z_status dump_obj(z_file* fp,zp_obj_base* obj);
	z_status dump_obj(zp_obj_base* obj);
	z_status load_obj(zp_obj_base* obj,zp_obj_parse* p);
	zp_obj_base* get_child_obj(ctext name,zp_obj_base* parent);

	void dump_indent();
	void dump_newline();

	
	//These are set by the obj->feature_manipulate()
	int feature_get_num_children(zp_obj_base* p_obj,ctext var_id );
	z_status feature_objlist_add(zp_obj_base* p_obj,ctext fet_name,zp_obj_base* p_obj_child);
	z_status feature_objlist_get_next(zp_obj_base* p_obj,ctext fet_name,zp_obj_base** pp_obj);
	z_status feature_objlist_get(zp_obj_base* p_obj,ctext fet_name,z_obj_container** pp_list);
	z_status feature_set_integer(zp_obj_base* p_obj,ctext fet_name,int val);
	z_status feature_set_string(zp_obj_base* p_obj,ctext fet_name,ctext val,size_t len);
	z_status feature_get_string(zp_obj_base* p_obj,ctext fet_name ,ctext& val);
	z_status feature_set_from_value(zp_obj_base* p_obj,zp_value* p_value_obj,zo_ftr_entry* fe);
	z_status feature_reset_iter(zp_obj_base* p_obj,ctext name=0 /* all*/);
	z_status feature_clear(zp_obj_base* p_obj,ctext name=0 /* all*/);
	z_status feature_dump(zp_obj_base* p_obj,ctext var_id);
	void     feature_capture_action_return_value(size_t act_rtn_val);
	ctext	 feature_get_as_string(zp_obj_base* p_obj,zo_ftr_entry* fe);
	z_status feature_execute(zp_obj_base* p_obj,zo_ftr_entry* fe);
	virtual z_status callback_feature_execute_obj(zp_obj_base* p_obj,zo_ftr_entry* fe){ return zs_ok;}
	z_status feature_display(zp_obj_base* p_obj,zo_ftr_entry* fe);

	//z_status feature_check_access(zo_fet_man_context* context,zo_fet_opt options
	//CALLBACKS 
	z_status access_action_params(const zo_action_params* params);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_string& str);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, bool& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, int& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, void* mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_obj_container& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, zo_str_container& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, zp_obj_base& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, zp_obj_base** mem_var);
	z_status feature_callback_pchild(zo_fet_man_context* context,zo_fet_opt options,zp_obj_base* pObj);
	z_status access_obj_child(zo_fet_man_context* context, zp_obj_base* pchild);

};


#endif
