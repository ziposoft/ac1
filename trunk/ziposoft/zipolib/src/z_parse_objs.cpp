
#include "zipolib_cpp_pch.h"
#include "z_parse.h"




//________________________________________________________________
//
//zp_pair
//________________________________________________________________

#define ZO_OBJ_LIST \
	ZCLS(zp_cmdline,void,"cmdline",\
	"{_root_slash}?'/':*({_path_list}ident:'/'):?({_object}ident:'.'):{_feature}zp_feature:"\
	"?( ({_assignment}'=':{_assign_val}zp_value)|{_params}zp_params)",\
	POBJ(_feature)  \
    VAR(_root_slash)  VAR(_object) POBJ(_assign_val)  VAR(_assignment) POBJ(_params) VAR(_path_list) )\
	ZCLS(zp_str_list,void,"strlist","'{':*(({_list}ident|{_list}string|{_list}string_sq):?','):'}'",VAR(_list) )\
	ZCLS(zp_params,void,"params","('(':*({_param_list}zp_value:?','):')')",VAR(_param_list) )\
	ZCLS(zp_pair,void,"pair","{_name}ident:'=':{_val}?ident:#','",VAR(_name) VAR(_val) )\
	/*ZCLS(zp_value,void,"value","(%whsp:'{':*{_child_list}obj:'}')|{_string_list}strlist|{_string}string|{_string}string_sq|{_obj}obj|{_string}ident", */\
	ZCLS(zp_value,void,"value","{_string}string|{_string}string_sq|{_string_list}zp_str_list|{_string}ident",\
		 OBJ(_string_list) VAR(_string)   )\
	ZCLS(zp_subscript,void,"feat","'[':({_id}ident):']'",VAR(_id) )	\
	ZCLS(zp_feature,void,"feat","{_name}ident:?{_sub}zp_subscript",\
		VAR(_name) POBJ(_sub)   )


#include "zipolib/include/z_obj.macro"
ZP_MODULE_DEFINE(parse);



