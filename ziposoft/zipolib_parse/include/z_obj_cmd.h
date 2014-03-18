#ifndef z_obj_cmd_h
#define z_obj_cmd_h

#include "zipolib_parse/include/z_parse.h"


class zo_man_cmd : public zo_manipulator,public z_obj
{
public:
	ZO_OBJ_H;

	zo_man_cmd(z_obj* root=0);


	z_obj* _root_obj;
	z_obj* _obj_current;
	z_obj* _obj_current_tmp;

	zp_obj_parser _parser;
	zp_cmdline _cmd_line_obj;
	void dump();
	//void dump_obj(z_obj* obj);;
	void dump_features(z_obj* obj);
	void set_root(z_obj* root_obj) { _obj_current= _root_obj=root_obj; }
	z_status parse_line(ctext text);
	z_status execute_line(ctext text);
	z_status execute_feature(z_obj* obj);
	z_status navigate_feature(ctext text);

	z_status dump_features_by_type(z_file* fp,z_obj* obj,U32 feature_type);
	z_status dump_features(z_file* fp,z_obj* obj);



	//Actions
	z_status act1();


	//properties
	z_string startup; //operation to perform on startup

	virtual z_status callback_feature_execute_obj(z_obj* p_obj,zo_ftr_entry* fe);

};

#endif
