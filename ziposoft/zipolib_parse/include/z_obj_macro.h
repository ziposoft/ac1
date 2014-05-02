/*
struct zo_ftr_entry
{

	ctext _internal_name;
	ctext _display_name;
	ctext _desc;
	U32  _id;
	U8   _security;
	U8   _detail;
	U32 _type;
};

PROP - Zipo Instrument Variable
VAR - Zipo Instrument Variable Simple

*/
/***** Action Params ******************************************************************************/
#undef OBJ   //object
#undef OBJV  //pure virtual object
#undef ACT   //ACT
#undef CHILD
#undef PCHILD
#undef LIST
#undef PROP
#undef NO_FTR
#undef STAT
#undef VAR
#undef PVAR
#undef ALIAS

#define NO_ID 0
#define NO_DESC 0
#define NO_FTR
#define PRM(...) { __VA_ARGS__ }
#define NO_PRM { 0 }

#define OBJV OBJ
#define LIST PROP

#define PROP(_VAR,_NAME,_ID,_DESC,_OPTIONS)
#define CHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS)
#define PCHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS)
#define VAR(_VAR) 
#define PVAR(_VAR)
#define ACT(_FUNC,_NAME,_ID,_DESC,_OPTIONS,_PARAMLIST) \
	ctext _FUNC##_PARAM_LIST[]=_PARAMLIST;\
	const zo_action_params _FUNC##_PARAMS={& _FUNC##_PARAM_LIST[0],sizeof(_FUNC##_PARAM_LIST)/sizeof(ctext)};
#define OBJ(_CLASS_,_BASE_,_NAME_,_DESC_,_PARSE,EXTRA) namespace _CLASS_##ns { EXTRA }; 
ZO_OBJ_LIST;


#undef OBJ
#undef OBJV  //pure virtual object
#undef ACT
#undef CHILD
#undef PCHILD
#undef NO_FTR
#undef LIST
#undef PROP
#undef VAR
#undef PVAR
/***** Feature List and Object defines ******************************************************************************/

#define NO_FTR 0

#define LIST(_VAR,_NAME,_ID,_DESC,_OPTIONS) { #_VAR,_NAME,_ID,_DESC,_OPTIONS,ZO_MT_LIST/*type*/ ,0},
#define PROP(_VAR,_NAME,_ID,_DESC,_OPTIONS) { #_VAR,_NAME,_ID,_DESC,_OPTIONS,ZO_MT_PROP/*type*/ ,0},
#define ACT(_VAR,_NAME,_ID,_DESC,_OPTIONS,_PARAMLIST) { #_VAR,_NAME,_ID,_DESC,_OPTIONS,ZO_MT_ACT/*type*/ ,(const void*)&_VAR##_PARAMS},
#define CHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS) { #_VAR,_NAME,_ID,_DESC,_OPTIONS,ZO_MT_CHILD /*zo_mv_child*/ /*type*/,0 },
#define PCHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS) { #_VAR,_NAME,_ID,_DESC,_OPTIONS,ZO_MT_CHILD /*zo_mv_child*/ /*type*/,0 },
#define VAR(_VAR) { #_VAR,0,0,0,0,ZO_MT_PROP/*type*/ ,0},
#define PVAR(_VAR) { #_VAR,0,0,0,0,ZO_MT_PROP/*type*/ ,0},
#define OBJ(_CLASS_,_BASE_,_NAME_,_DESC_,_PARSE,EXTRA) \
	zp_obj_base* new_##_CLASS_() {_CLASS_* obj=new  _CLASS_(); 	return obj; }\
const z_obj_fact* _CLASS_::get_fact() { return &_CLASS_::FACT;}; \
using namespace _CLASS_##ns ; zo_ftr_entry _CLASS_##_feature_list[]={ EXTRA }; \
	const z_obj_fact  _CLASS_::FACT=  {#_CLASS_, _NAME_,_DESC_,new_##_CLASS_,\
	&_CLASS_##_feature_list[0],sizeof(_CLASS_##_feature_list)/sizeof(zo_ftr_entry),&_BASE_::FACT,  _PARSE};


#define OBJV(_CLASS_,_BASE_,_NAME_,_DESC_,_PARSE,EXTRA) \
const z_obj_fact* _CLASS_::get_fact() { return &_CLASS_::FACT;}; \
using namespace _CLASS_##ns ; zo_ftr_entry _CLASS_##_feature_list[]={ EXTRA }; \
	const z_obj_fact  _CLASS_::FACT=  {#_CLASS_, _NAME_,_DESC_,0,\
	&_CLASS_##_feature_list[0],sizeof(_CLASS_##_feature_list)/sizeof(zo_ftr_entry),&_BASE_::FACT,  _PARSE};


ZO_OBJ_LIST;


#undef OBJ
#undef OBJV
#undef ACT
#undef CHILD
#undef PCHILD
#undef LIST
#undef PROP
#undef VAR
#undef PVAR
/**** List of object factories **********************************************************************************/
#define OBJV OBJ
#define OBJ(_CLASS_,_BASE_,_NAME_,_DESC_,...) { #_CLASS_,&_CLASS_::FACT }, 
#define _Z_MODULE(_NAME_) z_module_obj_entry z_module_##_NAME_##_obj_list[]={

Z_MODULE
ZO_OBJ_LIST
};
#undef _Z_MODULE
#define _Z_MODULE(_NAME_) const z_module_entry z_module_##_NAME_= \
{ #_NAME_,z_module_##_NAME_##_obj_list,sizeof(z_module_##_NAME_##_obj_list)/sizeof(z_module_obj_entry)};
	
Z_MODULE //Expands to the z_module_entry definition
#undef OBJ
#undef OBJV
#undef VAR
#undef PVAR
#undef PROP
#undef PRM

/*****************************************************************************************/
/* This section creates the feature_manipulate() functions for the ZO_OBJ_LIST */
#define LIST PROP
#define PROP(_VAR,_NAME,_ID,_DESC,_OPTIONS) \
	if(var_name==0) m->feature_callback(context,_OPTIONS,_VAR);if(z_str_check3(var_name,_ID,#_VAR,_NAME))  return	m->feature_callback(context,_OPTIONS, _VAR);

#define CHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS) \
	if(var_name==0) m->feature_callback(context,_OPTIONS,_VAR);if(z_str_check3(var_name,_ID,#_VAR,_NAME))  return	m->feature_callback(context,_OPTIONS, _VAR);

#define PCHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS) \
	if(var_name==0) m->feature_callback_pchild(context,_OPTIONS,_VAR);if(z_str_check3(var_name,_ID,#_VAR,_NAME))  return	m->feature_callback_pchild(context,_OPTIONS, _VAR);

#define PVAR(_VAR) \
	if(var_name==0) m->feature_callback_pchild(context,DEF_OPT,_VAR);if(z_str_same(var_name,#_VAR))    return	m->feature_callback_pchild(context,DEF_OPT, _VAR);

#define VAR(_VAR) \
	if(var_name==0) m->feature_callback(context,DEF_OPT,_VAR);if(z_str_same(var_name,#_VAR))    return	m->feature_callback(context,DEF_OPT, _VAR);

#define ACT(_FUNC,_NAME,_ID,_DESC,_OPTIONS,_PARAMLIST) \
	if(z_str_check3(var_name,_ID,#_FUNC,_NAME)) {if(zo_mvo_exec==oper) {	\
	m->feature_capture_action_return_value(_FUNC());return zs_ok;}} 

#define OBJV OBJ
#define OBJ(_CLASS_,_BASE_,_NAME_,_DESC_,_PARSE,EXTRA) \
z_status _CLASS_::feature_manipulate(zo_fet_man_context* context) { type_zo_memvar_oper   UNUSED oper=context->_oper;ctext UNUSED var_name=context->_name;zo_manipulator* UNUSED m=context->_man;\
   EXTRA ;	return _BASE_::feature_manipulate(context);} \

ZO_OBJ_LIST; //Expands to all the feature_manipulate() functions

#undef PROP
#undef OBJ
#undef OBJV
#undef VAR
#undef PVAR
#undef ACT
#undef CHILD
#undef PCHILD
#undef PRM

/******************************Parse*****************************************************/



/******************************** DONE! ***************************************************/
#undef _Z_MODULE
#undef Z_MODULE
#undef ZO_OBJ_LIST
#undef ZPS
#undef PROP
#undef OBJ
#undef VAR
#undef PRM
#undef PVAR


