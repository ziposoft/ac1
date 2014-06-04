
#include "zipolib_cpp_pch.h"
#include "z_parse.h"

z_status zp_cfg_obj::load_obj(void* obj,const z_factory* fact)
{
	size_t i;
	zf_feature *f;
	for(i=0;i<_features.size();i++)
	{
		if(fact->get_feature(_features[i]->_name,f)==zs_ok)
		{
			void* ftr_ptr=(char*)obj+f._offset;
			f.df->set_from_value(&_features[i]->_val,ftr_ptr);
		}
	}
	return zs_ok;
}

zf_obj zp_cfg_obj::createobj()
{
	zf_obj o;
	o._fact=zf_get_static_factory_by_name(_obj_type);
  	o._obj=o._fact->create_obj();
	size_t i;
	zf_feature* f;
	for(i=0;i<_features.size();i++)
	{
		if(o._fact->get_feature(_features[i]->_name,f)==zs_ok)
		{
			void* ftr_ptr=(char*)o._obj+f->_offset;
			f->df->set_from_value(&_features[i]->_val,ftr_ptr);
		}
	}
	return o;
}

bool zp_cmdline::has_path()
{
	return (_path_list.size())|| _root_slash;
}

//________________________________________________________________
//
//zp_pair
//________________________________________________________________

#define ZO_OBJ_LIST \
	ZCLS(zp_cmdline,void,"",\
	"{_root_slash}?'/':*({_path_list}ident:'/'):?({_object}ident:'.'):{_feature}?zp_feature:"\
	"?( ({_assignment}'=':{_assign_val}zp_value)|{_params}zp_params)",\
	POBJ(_feature)  VAR(_object) POBJ(_assign_val)  VAR(_assignment) POBJ(_params) VAR(_root_slash)   VAR(_path_list) )\
	ZCLS(zp_str_list,void,"strlist","'{':*(({_list}ident|{_list}string|{_list}string_sq):?','):'}'",VAR(_list) )\
	ZCLS(zp_params,void,"params","('(':*({_param_list}zp_value:?','):')')",VAR(_param_list) )\
	ZCLS(zp_pair,void,"pair","{_name}ident:'=':{_val}?ident:#','",VAR(_name) VAR(_val) )\
	ZCLS(zp_value,void,"value","{_string}string|{_string}string_sq|{_string_list}zp_str_list|{_obj}zp_cfg_obj|{_string}ident",\
		 POBJ(_string_list) POBJ(_obj) VAR(_string)   )\
	ZCLS(zp_subscript,void,"","'[':({_id}ident):']'",VAR(_id) )	\
	ZCLS(zp_feature,void,"","{_name}ident:?{_sub}zp_subscript",VAR(_name) POBJ(_sub)   )\
	ZCLS(zp_cfg_file,void,"","%whsp:{_obj}*zp_cfg_obj:?whsp",VAR(_obj))\
	ZCLS(zp_cfg_obj,void,"","%whsp:{_obj_type}ident:'{':{_features}*zp_cfg_feature:'}'",VAR(_obj_type) VAR(_features))\
	ZCLS(zp_cfg_feature,void,"","%whsp:{_name}ident:'=':{_val}zp_value", OBJ(_val)  VAR(_name))


//	ZCLS(zp_path,void,"","{_root_slash}?'/':*({_path_list}ident:'/')",	VAR(_root_slash)   VAR(_path_list) )




#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(parse);



