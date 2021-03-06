#define Z_PARSE_NS(_CLASS_)   _zp_parse_ns##_CLASS_

#undef POBJ
#undef OBJ
#undef LIST
#undef ZCLS
#undef VAR
#undef OBJV
#undef NOFTR

#define NOFTR
#define POBJ(_VAR)  const zp_member_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_child_pobj_funcs_get(nullobj->_VAR); }
#define LIST(_VAR)  const zp_member_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_var_list_funcs(nullobj->_VAR); }
#define OBJ(_VAR)  const zp_member_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_child_obj_funcs_get(nullobj->_VAR); }
#define VAR(_VAR)  const zp_member_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_var_funcs_get(nullobj->_VAR); }
#define ZCLS(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) namespace Z_PARSE_NS(_CLASS_) { _CLASS_* nullobj=0;	_EXTRA_ }

ZO_OBJ_LIST;


#undef ZCLS
#undef LIST
#undef VAR
#undef OBJ
#undef POBJ
#undef NOFTR

#define NOFTR {"",0,0},
#define VAR(_VAR)  { #_VAR, zp_offsetof(_VAR),__zp_var_funcs_get_##_VAR },

#define POBJ(_VAR) VAR(_VAR)
#define OBJ(_VAR) VAR(_VAR)
#define ZCLS(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) \
	namespace Z_PARSE_NS(_CLASS_) {const zp_var_entry varlist[]={ _EXTRA_ }; const size_t varlist_length=sizeof( varlist)/sizeof(	zp_var_entry);  }	 \
	template <> const size_t zp_factory_T<_CLASS_>::get_var_list_size() const{ return Z_PARSE_NS(_CLASS_)::varlist_length; };\
	template <> const zp_var_entry* zp_factory_T<_CLASS_>::get_var_list() const{ return Z_PARSE_NS(_CLASS_)::varlist; }\
	const zp_factory_T<_CLASS_> static_instance_factory##_CLASS_;  \
	template <> const zp_factory_T<_CLASS_>&  zp_factory_T<_CLASS_>::obj=static_instance_factory##_CLASS_;  \
	template <> ctext zp_factory_T<_CLASS_>::get_parse_string() const{ return _PARSE_; }	  \
	template <> ctext zp_factory_T<_CLASS_>::get_name()const { return #_CLASS_; }		\
	template <> void* zp_factory_T<_CLASS_>::create_obj()const { return z_new _CLASS_(); }	

ZO_OBJ_LIST;

#undef OBJ
#undef ZCLS
#undef LIST
#undef VAR
#undef POBJ
#define ZCLS(_CLASS_,_BASE_,_NAME_,_DESC_,...) { #_CLASS_,&static_instance_factory##_CLASS_ }, 
#define ZP_MODULE_DEFINE(_NAME_) zp_module_fact_entry zp_module_##_NAME_##_fact_list[]={\
ZO_OBJ_LIST };\
const zp_module_entry ZP_MODULE(_NAME_)= \
{ #_NAME_,zp_module_##_NAME_##_fact_list,sizeof(zp_module_##_NAME_##_fact_list)/sizeof(zp_module_fact_entry)};

/* dont undefine ZCLS, it is needed for the ZP_MODULE_DEFINE */ 