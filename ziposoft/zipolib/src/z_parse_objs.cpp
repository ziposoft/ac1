
#include "zipolib_cpp_pch.h"
#include "z_parse.h"




//________________________________________________________________
//
//zp_pair
//________________________________________________________________

#define ZO_OBJ_LIST \
	ZCLS(zp_path,void,"","{_root_slash}?'/':+({_path_list}ident:'/')",	VAR(_root_slash)   VAR(_path_list) )\
	ZCLS(zp_cmdline,void,"",\
	"{_path}?zp_path:?({_object}ident:'.'):{_feature}zp_feature:"\
	"?( ({_assignment}'=':{_assign_val}zp_value)|{_params}zp_params)",\
	POBJ(_feature)  VAR(_object) POBJ(_assign_val)  VAR(_assignment) POBJ(_params) POBJ(_path) )\
	ZCLS(zp_str_list,void,"strlist","'{':*(({_list}ident|{_list}string|{_list}string_sq):?','):'}'",VAR(_list) )\
	ZCLS(zp_params,void,"params","('(':*({_param_list}zp_value:?','):')')",VAR(_param_list) )\
	ZCLS(zp_pair,void,"pair","{_name}ident:'=':{_val}?ident:#','",VAR(_name) VAR(_val) )\
	ZCLS(zp_value,void,"value","{_string}string|{_string}string_sq|{_string_list}zp_str_list|{_string}ident",\
		 POBJ(_string_list) VAR(_string)   )\
	ZCLS(zp_subscript,void,"","'[':({_id}ident):']'",VAR(_id) )	\
	ZCLS(zp_feature,void,"","{_name}ident:?{_sub}zp_subscript",VAR(_name) POBJ(_sub)   )\
	ZCLS(zp_cfg_file,void,"","%whsp:{_obj}*zp_cfg_obj:?whsp",VAR(_obj))\
	ZCLS(zp_cfg_obj,void,"","%whsp:{_obj_type}ident:'{':{_features}*zp_cfg_feature:'}'",VAR(_obj_type) VAR(_features))\
	ZCLS(zp_cfg_feature,void,"","%whsp:{_name}ident:'=':({_strlist}zp_str_list|{_child}zp_cfg_obj|{_val}ident)",\
		POBJ(_strlist) POBJ(_child) VAR(_val) VAR(_name))






#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(parse);



