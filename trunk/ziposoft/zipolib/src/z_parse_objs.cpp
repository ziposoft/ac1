
#include "zipolib_cpp_pch.h"
#include "z_parse.h"

z_status zp_cfg_obj::load_obj(void* obj, z_factory* fact)
{
	size_t i;
	zf_feature *f;
	for(i=0;i<_features.size();i++)
	{
		f=fact->get_feature(_features[i]->_name);
		if(f)
		{
			void* ftr_ptr=(char*)obj+f->_offset;
			f->df->set_from_value(&_features[i]->_val,ftr_ptr);
		}
	}
	return zs_ok;
}

z_status zp_cfg_obj::createobj(zf_obj& o)
{
	o._fact=zf_get_factory(_obj_type);
	if(!o._fact)
	{
		return Z_ERROR_MSG(zs_item_not_found,"Object type \"%s\" not found",_obj_type.c_str());
	}
  	o._obj=o._fact->create_obj();
	if(!o._obj)
	{
		return Z_ERROR_MSG(zs_cannot_create_virtual_obj,"Object \"%s\" cannot be created",_obj_type.c_str());
	}
	size_t i;
	zf_feature* f;
	for(i=0;i<_features.size();i++)
	{
		if( (f=o._fact->get_feature(_features[i]->_name))  )
		{
			void* ftr_ptr=(char*)o._obj+f->_offset;
			f->df->set_from_value(&_features[i]->_val,ftr_ptr);
		}
	}
	return zs_ok;
}
zp_feature* zp_cmdline::get_feature()
{
	if(_path.size()==0)
		return 0;
	return _path[_path.size()-1];

}

bool zp_cmdline::has_path()
{
	return (  _root_slash || _path.size()>1);
}

//________________________________________________________________
//
//zp_pair
//________________________________________________________________

#define ZO_OBJ_LIST \
	ZCLS(zp_cmdline,none,"",\
	"{_root_slash}?'/':*({_path}zp_feature:?('/'|'.')):?( ({_assignment}'=':{_assign_val}zp_value)|{_params}zp_params)",\
	 POBJ(_assign_val)  VAR(_assignment) POBJ(_params) VAR(_root_slash)  VAR(_path) )\
	ZCLS(zp_str_list,none,"strlist","'{':*(({_list}string|{_list}string_sq):?','):'}'",VAR(_list) )\
	ZCLS(zp_params,none,"params","('(':*({_param_list}zp_value:?','):')')",VAR(_param_list) )\
	ZCLS(zp_pair,none,"pair","{_name}ident:'=':{_val}?ident:#','",VAR(_name) VAR(_val) )\
	ZCLS(zp_value,none,	"value","{_string}string|{_string}string_sq|{_string_list}zp_str_list|{_obj}zp_cfg_obj|{_obj_list}zp_cfg_obj_list|{_string}ident",\
		 POBJ(_string_list)  POBJ(_obj_list) POBJ(_obj) VAR(_string)   )\
	ZCLS(zp_subscript,none,"","'[':({_id}ident):']'",VAR(_id) )	\
	ZCLS(zp_feature,none,"","{_name}ident:?{_sub}zp_subscript",VAR(_name) POBJ(_sub)   )\
	ZCLS(zp_cfg_obj_list,none,"","%whsp:'{':{_list}*zp_cfg_obj:'}':?whsp",VAR(_list))\
	ZCLS(zp_cfg_file,none,"","%whsp:{_obj}*zp_cfg_obj:?whsp",VAR(_obj))\
	ZCLS(zp_cfg_obj,none,"","%whsp:{_obj_type}ident:'{':{_features}*zp_cfg_feature:'}'",VAR(_obj_type) VAR(_features))\
	ZCLS(zp_cfg_feature,none,"","%whsp:{_name}ident:'=':{_val}zp_value", OBJ(_val)  VAR(_name))


//	ZCLS(zp_path,none,"","{_root_slash}?'/':*({_path_list}ident:'/')",	VAR(_root_slash)   VAR(_path_list) )

//	ZCLS(zp_value,none,	"value","{_string}string|{_string}string_sq|{_string_list}zp_str_list|{_obj}zp_cfg_obj|{_obj_list}zp_cfg_obj_list|{_string}ident",\



#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(parse);



