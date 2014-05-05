#ifndef z_obj_man_h
#define z_obj_man_h
#include "zipolib_parse2/include/zp_obj.h"

class void_parse;
class void_parser;
class zo_manipulator
{
	int     _dump_depth;
	bool    _dump_new_line;
	void*   _p_member_var_obj;
	z_string _member_var_string_data;
	int      _member_var_int_data;   
	void* _member_obj_container;
	size_t _action_return_value;

protected:
	z_file* _dump_fp;
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
		void* obj //If obj is passed in, append values
		);
	void get_feature_list(
		z_strlist& list,
		U32 feature,
		void* obj 
		);
		*/
	zo_ftr_entry* get_next_feature(
			const z_obj_fact* fact,
			U32 feature_type,
			int &index
			);
/*
	z_status var_set_from_text(void* p_obj,ctext var_id,ctext txt);
	z_status var_set_from_string(void* p_obj,ctext var_id);
*/	
	z_status dump_obj(z_file* fp,void* obj);
	z_status dump_obj(void* obj);
	z_status load_obj(void* obj,void_parse* p);
	void* get_child_obj(ctext name,void* parent);

	void dump_indent();
	void dump_newline();

	
	//These are set by the obj->feature_manipulate()
	int feature_get_num_children(void* p_obj,ctext var_id );
	z_status feature_objlist_add(void* p_obj,ctext fet_name,void* p_obj_child);
	z_status feature_objlist_get_next(void* p_obj,ctext fet_name,void** pp_obj);
	z_status feature_objlist_get(void* p_obj,ctext fet_name,void** pp_list);
	z_status feature_set_integer(void* p_obj,ctext fet_name,int val);
	z_status feature_set_string(void* p_obj,ctext fet_name,ctext val,size_t len);
	z_status feature_get_string(void* p_obj,ctext fet_name ,ctext& val);
	z_status feature_set_from_value(void* p_obj,zp_value* p_value_obj,zo_ftr_entry* fe);
	z_status feature_reset_iter(void* p_obj,ctext name=0 /* all*/);
	z_status feature_clear(void* p_obj,ctext name=0 /* all*/);
	z_status feature_dump(void* p_obj,ctext var_id);
	void     feature_capture_action_return_value(size_t act_rtn_val);
	ctext	 feature_get_as_string(void* p_obj,zo_ftr_entry* fe);
	z_status feature_execute(void* p_obj,zo_ftr_entry* fe);
	virtual z_status callback_feature_execute_obj(void* p_obj,zo_ftr_entry* fe){ return zs_ok;}
	z_status feature_display(void* p_obj,zo_ftr_entry* fe);

	//z_status feature_check_access(zo_fet_man_context* context,zo_fet_opt options
	//CALLBACKS 
	z_status access_action_params(const zo_action_params* params);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, z_string& str);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, bool& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, int& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, void* mem_var);
	//z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, void& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, zo_str_container& mem_var);
	//z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, void& mem_var);
	z_status feature_callback(zo_fet_man_context* context,zo_fet_opt options, void** mem_var);
	z_status feature_callback_pchild(zo_fet_man_context* context,zo_fet_opt options,void* pObj);
	z_status access_obj_child(zo_fet_man_context* context, void* pchild);

};


#endif
