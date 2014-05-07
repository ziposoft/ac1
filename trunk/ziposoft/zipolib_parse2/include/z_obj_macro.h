#define Z_PARSE_NS(_CLASS_)   _zp_parse_ns##_CLASS_

#undef OBJ
#undef VAR
#undef OBJV



#define VAR(_VAR)  const zp_var_funcs_base* __zp_var_funcs_get_##_VAR (){ return zp_var_funcs_get(nullobj->_VAR); }
#define  OBJ(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) namespace Z_PARSE_NS(_CLASS_) { _CLASS_* nullobj=0;	_EXTRA_ }

ZO_OBJ_LIST;


#undef OBJ
#undef VAR
#undef OBJV



#define VAR(_VAR)  { #_VAR, zp_offsetof(_VAR),__zp_var_funcs_get_##_VAR },
#define  OBJ(_CLASS_,_BASE_,_NAME_,_PARSE_,_EXTRA_) \
	namespace Z_PARSE_NS(_CLASS_) {const zp_var_entry varlist[]={ _EXTRA_ }; const size_t varlist_length=sizeof( varlist)/sizeof(	zp_var_entry);  }	 \
	const size_t zp_factory_T<_CLASS_>::get_var_list_size() const{ return Z_PARSE_NS(_CLASS_)::varlist_length; };\
	const zp_var_entry* zp_factory_T<_CLASS_>::get_var_list() const{ return Z_PARSE_NS(_CLASS_)::varlist; }\
	const zp_factory_T<_CLASS_> zp_factory_T<_CLASS_>::static_instance;  \
	ctext zp_factory_T<_CLASS_>::get_parse_string() const{ return _PARSE_; }	  \
	ctext zp_factory_T<_CLASS_>::get_name()const { return #_CLASS_; }		\
	void* zp_factory_T<_CLASS_>::create_obj()const { return z_new _CLASS_(); }	

ZO_OBJ_LIST;

#undef OBJ
#undef VAR
#undef OBJV

#define OBJ(_CLASS_,_BASE_,_NAME_,_DESC_,...) { #_CLASS_,&zp_factory_T<_CLASS_>::static_instance }, 
#define ZP_MODULE_DEFINE(_NAME_) zp_module_fact_entry zp_module_##_NAME_##_fact_list[]={\
ZO_OBJ_LIST };\
const zp_module_entry ZP_MODULE(_NAME_)= \
{ #_NAME_,zp_module_##_NAME_##_fact_list,sizeof(zp_module_##_NAME_##_fact_list)/sizeof(zp_module_fact_entry)};
	
