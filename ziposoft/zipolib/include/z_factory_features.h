/*________________________________________________________________________

 z_factory_features_h

 dont use stand alone
 THIS IS INCLUDED IN z_factory.h 

________________________________________________________________________*/


#ifndef z_factory_dyn_h
#define z_factory_dyn_h


typedef  U64 zf_feature_flags; 
#define ZFF_LOAD		0x00000001
#define ZFF_SAVE		0x00000002
#define ZFF_LIST		0x00000004
#define ZFF_SET			0x00000008
#define ZFF_LIST3		0x00000010
#define ZFF_LIST4		0x00000020


#define ZFF_HEX			0x01000000

#define ZFF_PROP (ZFF_LOAD|ZFF_SAVE|ZFF_LIST|ZFF_SET)
#define ZFF_PARAM (ZFF_LOAD|ZFF_SAVE|ZFF_SET)
#define ZFF_STAT (ZFF_SAVE|ZFF_LIST)
#define ZFF_DYN   (ZFF_LIST|ZFF_SET)



#define __ZPROP_X(_VFUNCS_,_VAR_,_NAME_,_FLAGS_,_DESC_)	\
	add_feature(_VFUNCS_ (( (THECLASS*)0)->_VAR_),_NAME_,zp_offsetof_class(THECLASS,_VAR_),_DESC_,_FLAGS_ /*flags*/)
#define ZPROP_X(_VAR_,_NAME_,_FLAGS_,_DESC_)	__ZPROP_X(zp_var_funcs_get,_VAR_,_NAME_,_FLAGS_,_DESC_)

#define ZPROP_HEX(_VAR_,_NAME_,_FLAGS_,_DESC_)	__ZPROP_X(zp_var_funcs_hex_get,_VAR_,_NAME_,_FLAGS_,_DESC_)


#define  ZPROP(_VAR_) ZPROP_X( _VAR_,#_VAR_,0,"")
//#define ZACT(_ACT_) add_act_T(#_ACT_,*(z_memptr*)&(&THECLASS::_ACT_) ,"");
#define ZPARAM_X(_VAR_,_NAME_,_FLAGS_,_DESC_) ZPROP_X(_VAR_,_NAME_,_FLAGS_,_DESC_)
#define ZPARAM(_VAR_) ZPROP(_VAR_)

#define ZACT_X(_ACT_,_NAME_,_FLAGS_,_DESC_) {fn_act _func_##_ACT_=&THECLASS::_ACT_;add_act(_NAME_,*(z_memptr*)(&_func_##_ACT_) ,_DESC_);}
#define ZACT(_ACT_) ZACT_X(_ACT_,#_ACT_,0,"")
#define ZACT_XP(_ACT_,_NAME_,_FLAGS_,_DESC_,_N_,...) {fn_act _func_##_ACT_=&THECLASS::_ACT_;add_act_params(_NAME_,*(z_memptr*)(&_func_##_ACT_) ,_DESC_,_N_,__VA_ARGS__);}
#define ZPOBJ(_VAR_) __ZPROP_X(zp_child_pobj_funcs_get,_VAR_,#_VAR_,0,"")
#define ZOBJ(_VAR_) __ZPROP_X(zp_child_obj_funcs_get,_VAR_,#_VAR_,0,"")

/*


#define ZFEAT(_VAR_,__FUNCTYPE_)		add_prop(#_VAR_,__FUNCTYPE_( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_),"")
#define ZVOBJ(_VAR_) ZFEAT(_VAR_,zp_child_vobj_funcs_get)
#define ZPROP(_VAR_) ZFEAT(_VAR_,zp_var_funcs_get)

#define ZOBJ_X(_VAR_,__FUNCTYPE_)		add_obj(#_VAR_,__FUNCTYPE_( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_),"")

#define ZLIST(_VAR_)		add_list(#_VAR_,__FUNCTYPE_( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_),"")
*/
class z_console;
class z_factory_controller;
class zf_feature
{
	friend class z_factory;
	friend class z_factory_controller;
protected:
	const zf_var_funcs_base* df;
	z_memptr _offset;
public:

	zf_feature();
	zf_feature(ctext name,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc="");
	ctext get_map_key() { return _name; }

	z_string _name;
	z_string _description;
	zf_feature_flags _flags;
	virtual zf_feature_type get_type()const =0;

	//zf_feature_type _type;
	void dump(z_file& f,void* obj);
	virtual void display(z_file& f,void* obj)=0;

	void* get_memvar_ptr(void* obj,int* iter=0) ;
	virtual zf_action* get_action(){return 0;}
   	virtual z_status set_from_value(zp_value* val,void *obj) {return Z_ERROR_NOT_IMPLEMENTED; 	}
  	virtual z_status load(zp_text_parser &parser, zf_obj& o){return Z_ERROR_NOT_IMPLEMENTED;}
	/* 
	evaluate 
	evaluate a feature as part of a command line
	Returns:
		zs_ok- processed OK, continue processing
		zs_end_of_list- processed OK, command complete (check for extraneous chars after)
		zs_feature_not_found-
		zs_syntax_error-
	*/
		
	virtual z_status evaluate(z_factory_controller& controller,zf_obj& o)
	{
		return Z_ERROR_NOT_IMPLEMENTED;
	}

 	virtual z_status evaluate1(zp_text_parser &parser, zf_obj& o,int index=-1) 
	{
		return Z_ERROR_NOT_IMPLEMENTED;
	}
 	virtual z_status get_string_val(z_string& out, void* v,int index=-1){return Z_ERROR_NOT_IMPLEMENTED;}
 	virtual z_status get_zf_obj(zf_obj& out, ctext key,zf_obj& parent);

	virtual z_status on_tab(z_console* console) { return zs_ok; }
	virtual z_status add_to_list(z_strlist& list,void* obj) {  list<<_name; return zs_ok;}


};
class zf_action  : public  zf_feature
{
public:
	zf_action(ctext name,z_memptr offset,ctext desc="");
	z_obj_vector_map<zf_feature> _params;
	virtual zf_action* get_action(){return this;}
	virtual void display(z_file& f,void* obj);
	int execute(z_file* f,zf_obj& obj);
  	virtual z_status load(zp_text_parser &parser, zf_obj& o) ;
 	virtual z_status evaluate1(zp_text_parser &parser, zf_obj& o,int index=-1) ;
	virtual zf_feature_type get_type() const { return zf_ft_act; }
	virtual z_status evaluate(z_factory_controller& controller,zf_obj& o) ;
};
class zf_child_obj  : public  zf_feature
{
public:
 	zf_child_obj(ctext name,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc="");
	virtual void display(z_file& f,void* obj);
	virtual zf_feature_type get_type() const { return zf_ft_obj; }
	virtual z_status evaluate(z_factory_controller& controller,zf_obj& o) ;

};
class zf_prop  : public  zf_feature
{
public:
	zf_prop(ctext name,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc="");
	virtual void display(z_file& f,void* obj);
	virtual z_status set_from_value(zp_value* val,void *obj) ;

  	virtual z_status load(zp_text_parser &parser, zf_obj& o) ;
 	virtual z_status get_string_val(z_string& out, void* v,int index=-1);
 	virtual z_status evaluate1(zp_text_parser &parser, zf_obj& o,int index=-1) ;
	virtual zf_feature_type get_type() const { return zf_ft_var; }
	virtual z_status evaluate(z_factory_controller& controller,zf_obj& o) ;

};
class zf_funcs_obj_list_base;
class zf_list  : public  zf_feature
{
	const zf_funcs_obj_list_base* _list_funcs;
public:
	zf_list(ctext name,const zf_funcs_obj_list_base* funcs,z_memptr offset,ctext desc="");
	virtual void display(z_file& f,void* obj);
	/*
	virtual z_status set_from_value(zp_value* val,void *obj) ;

  	virtual z_status load(zp_text_parser &parser, zf_obj& o) ;
 	virtual z_status get_string_val(z_string& out, void* v,int index=-1);
 	virtual z_status evaluate1(zp_text_parser &parser, zf_obj& o,int index=-1) ;
	*/
	virtual zf_feature_type get_type() const { return zf_ft_obj_list; }
	virtual z_status on_tab(z_console* console) ;
	virtual z_status add_to_list(z_strlist& list,void* obj);
	virtual z_status evaluate(z_factory_controller& controller,zf_obj& o) ;



};
class zf_alias  : public  zf_feature
{
public:
	zf_feature* df;

};


#endif

