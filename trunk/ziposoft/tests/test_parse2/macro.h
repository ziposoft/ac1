
#undef OBJ
#undef VAR
#undef OBJV



#define VAR(_VAR)  const zp_var_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_var_funcs_get(nullobj->_VAR); }
#define  OBJ(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) namespace _zp_parse_##_CLASS_ { _CLASS_* nullobj=0;	_EXTRA_ }

ZO_OBJ_LIST;


#undef OBJ
#undef VAR
#undef OBJV



#define VAR(_VAR)  { #_VAR, zp_offsetof(_VAR),__zp_var_funcs_get_##_VAR },
#define  OBJ(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) \
	namespace _zp_parse_##_CLASS_ { zp_var_list varlist[]={ _EXTRA_ }; size_t varlist_length=sizeof( varlist)/sizeof(	zp_var_list); }	 \
	size_t zp_factory_T<_CLASS_>::get_var_list_size() { return _zp_parse_##_CLASS_ ::varlist_length; };\
	zp_var_list* zp_factory_T<_CLASS_>::get_var_list() { return _zp_parse_##_CLASS_ ::varlist; }


ZO_OBJ_LIST;



#undef OBJ
#undef VAR
#undef OBJV