#define Z_PARSE_NS(_CLASS_)   _zp_parse_ns##_CLASS_

#undef POBJ
#undef OBJ
#undef LIST
#undef ZCLS
#undef VAR
#undef OBJV
#undef NOFTR
#undef ACT

#define NOFTR
#define POBJ(_VAR)  const zf_var_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_child_pobj_funcs_get(nullobj->_VAR); }
#define LIST(_VAR)  const zf_var_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_var_funcs_list_vect(nullobj->_VAR); }
#define OBJ(_VAR)  const zf_var_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_child_obj_funcs_get(nullobj->_VAR); }
#define VAR(_VAR)  const zf_var_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_var_funcs_get(nullobj->_VAR); }
#define ZCLS(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) namespace Z_PARSE_NS(_CLASS_) { _CLASS_* nullobj=0;	_EXTRA_ }
#define ACT(_VAR)
ZO_OBJ_LIST;


#undef ZCLS
#undef LIST
#undef VAR
#undef OBJ
#undef POBJ
#undef NOFTR
#undef ACT

#define NOFTR 
#define VAR(_VAR) 
#define POBJ(_VAR)  
#define OBJ(_VAR)  
#define ACT(_VAR)  const fptr act_##_VAR=&THECLASS::_VAR;

#define ZCLS(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) \
	namespace Z_PARSE_NS(_CLASS_) {typedef  _CLASS_ THECLASS; typedef int (THECLASS::*fptr)();_EXTRA_ };

ZO_OBJ_LIST;


#undef ZCLS
#undef LIST
#undef VAR
#undef OBJ
#undef POBJ
#undef NOFTR
#undef ACT

#define NOFTR {"",0,zf_ft_none,0},
#define VAR(_VAR)  { #_VAR, zp_offsetof(_VAR),zf_ft_var,__zp_var_funcs_get_##_VAR },
#define POBJ(_VAR)  { #_VAR, zp_offsetof(_VAR),zf_ft_obj,__zp_var_funcs_get_##_VAR },
#define OBJ(_VAR)  { #_VAR, zp_offsetof(_VAR),zf_ft_obj,__zp_var_funcs_get_##_VAR },
#define ACT(_VAR)  { #_VAR, *(z_memptr*)&act_##_VAR,zf_ft_act,zp_act_funcs_get },

#define ZCLS(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) \
	namespace Z_PARSE_NS(_CLASS_) {typedef  _CLASS_ THECLASS; const zf_var_entry varlist[]={ _EXTRA_ }; const size_t varlist_length=sizeof( varlist)/sizeof(	zf_var_entry);  }	 \
	z_factory_T<_CLASS_> _gz_factory_##_CLASS_;  \
	const z_factory_info 	ZFACT##_CLASS_##INFO={ #_CLASS_,&_gz_factory_##_BASE_,_PARSE_,Z_PARSE_NS(_CLASS_)::varlist_length,&Z_PARSE_NS(_CLASS_)::varlist[0] };   \
	template <>  const z_factory_info& z_factory_T<_CLASS_>::get_info() const{ return ZFACT##_CLASS_##INFO; }	  \
	template <> z_factory_T<_CLASS_>&  z_factory_T<_CLASS_>::self=_gz_factory_##_CLASS_;\
	template <> void z_factory_T<_CLASS_>::add_features(){}

ZO_OBJ_LIST;

#undef OBJ
#undef ZCLS
#undef ACT
#undef LIST
#undef VAR
#undef POBJ
#define ZCLS(_CLASS_,_BASE_,_NAME_,_DESC_,...) { #_CLASS_,&_gz_factory_##_CLASS_ }, 
#define ZP_MODULE_DEFINE(_NAME_) zp_module_fact_entry zp_module_##_NAME_##_fact_list[]={\
ZO_OBJ_LIST };\
const zp_module_entry ZP_MODULE(_NAME_)= \
{ #_NAME_,zp_module_##_NAME_##_fact_list,sizeof(zp_module_##_NAME_##_fact_list)/sizeof(zp_module_fact_entry)};

/* dont undefine ZCLS, it is needed for the ZP_MODULE_DEFINE */ 