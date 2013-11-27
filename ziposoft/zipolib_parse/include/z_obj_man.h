#ifndef z_obj_man_h
#define z_obj_man_h
#include "zipolib_parse/include/z_obj.h"

class zp_obj_parse;
class zp_obj_parser;
class zo_manipulator
{
	int     _dump_depth;
	bool    _dump_new_line;
	z_obj*   _p_member_var_obj;
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
		z_obj* obj //If obj is passed in, append values
		);
	void get_feature_list(
		z_strlist& list,
		U32 feature,
		z_obj* obj 
		);
		*/
	zo_ftr_entry* get_next_feature(
			const z_obj_fact* fact,
			U32 feature_type,
			int &index
			);
/*
	zp_status var_set_from_text(z_obj* p_obj,ctext var_id,ctext txt);
	zp_status var_set_from_string(z_obj* p_obj,ctext var_id);
*/	
	zp_status dump_obj(z_file* fp,z_obj* obj);
	zp_status dump_obj(z_obj* obj);
	zp_status load_obj(z_obj* obj,zp_obj_parse* p);
	z_obj* get_child_obj(ctext name,z_obj* parent);

	void dump_indent();
	void dump_newline();

	
	//These are set by the obj->feature_manipulate()
	int feature_get_num_children(z_obj* p_obj,ctext var_id );
	zp_status feature_objlist_add(z_obj* p_obj,ctext fet_name,z_obj* p_obj_child);
	zp_status feature_objlist_get_next(z_obj* p_obj,ctext fet_name,z_obj** pp_obj);
	zp_status feature_objlist_get(z_obj* p_obj,ctext fet_name,z_obj_container** pp_list);
	zp_status feature_set_integer(z_obj* p_obj,ctext fet_name,int val);
	zp_status feature_set_string(z_obj* p_obj,ctext fet_name,ctext val,size_t len);
	zp_status feature_get_string(z_obj* p_obj,ctext fet_name ,ctext& val);
	zp_status feature_set_from_value(z_obj* p_obj,zp_value* p_value_obj,zo_ftr_entry* fe);
	zp_status feature_reset_iter(z_obj* p_obj,ctext name=0 /* all*/);
	zp_status feature_clear(z_obj* p_obj,ctext name=0 /* all*/);
	zp_status feature_dump(z_obj* p_obj,ctext var_id);
	void     feature_capture_action_return_value(size_t act_rtn_val);
	ctext	 feature_get_as_string(z_obj* p_obj,zo_ftr_entry* fe);
	zp_status feature_execute(z_obj* p_obj,zo_ftr_entry* fe);
	virtual zp_status callback_feature_execute_obj(z_obj* p_obj,zo_ftr_entry* fe){ return zs_ok;}
	zp_status feature_display(z_obj* p_obj,zo_ftr_entry* fe);

	//zp_status feature_check_access(zo_fet_man_context* context,zo_fet_opt options
	//CALLBACKS 
	zp_status access_action_params(const zo_action_params* params);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_string& str);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, bool& mem_var);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, int& mem_var);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, void* mem_var);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_obj_container& mem_var);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, zo_str_container& mem_var);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_obj& mem_var);
	zp_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_obj** mem_var);
	zp_status feature_callback_pchild(zo_fet_man_context* context,zo_fet_opt options,z_obj* pObj);
	zp_status access_obj_child(zo_fet_man_context* context, z_obj* pchild);

};


#endif
