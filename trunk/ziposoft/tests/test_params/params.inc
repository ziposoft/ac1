
#undef PARAM
#undef OPER
#define PARAM(_TYPE_,_VAR_,_DEFAULT_,_DESC_) _TYPE_ _VAR_=_DEFAULT_;
#define OPER(_FUNC_,_DESC_)  int _FUNC_();  

PARAM_LIST


#undef PARAM
#undef OPER
#define PARAM(_TYPE_,_VAR_,_DEFAULT_,_DESC_)  
#define OPER(_FUNC_,_DESC_)  { _FUNC_, #_FUNC_, _DESC_ },


st_param_func list_param_funcs[]={
PARAM_LIST
};
int list_param_funcs_count=sizeof(list_param_funcs)/sizeof(st_param_func);

#undef PARAM
#undef OPER