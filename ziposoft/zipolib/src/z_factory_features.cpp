#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory_controller.h"
#include "zipolib/include/z_factory_var_funcs.h"
#include "zipolib/include/z_parse_text.h"



zf_feature* zf_features::get_by_name(ctext t)   const
{
	size_t i=0;
	while(i<this->size())
	{
		zf_feature* obj= this->get(i);
		if(strcmp(obj->get_name(),t)==0)
			return obj;
		i++;
	}
	return 0;
}

/*________________________________________________________________________

zf_feature
________________________________________________________________________*/
zf_feature::zf_feature() 
{
	df=0;
	_offset=0;
	_flags=ZFF_PROP;

}
zf_feature::zf_feature(ctext id,ctext name,const zf_var_funcs_base* funcs,z_memptr offset,zf_feature_flags flags,ctext desc) 
{
	_id=id;
	_name=name;
	df=funcs;
	_offset=offset;
	_description=desc;
	_flags=flags;

}

void* zf_feature::get_memvar_ptr(void* obj,int* iter)
{
	char* pvar=(char*)obj+_offset;

	return pvar;


}
void zf_feature::dump(z_file& f,void* obj)
{
	char* pvar=(char*)obj+_offset;
	f.indent();
	f << _name;
	if(df)
	{
		f << "=";
		df->dump(f,pvar);
	}
	f <<'\n';
}

 z_status zf_feature::load(zp_text_parser &parser,const zf_obj& o,zf_feature_flags oper) 
 {
	void* ftr_ptr=0;
	if(!(oper & _flags))
	{
	  oper=ZFF_SKIP;
	}
	else
	{
		ftr_ptr=(char*)o._obj+_offset;

	}
	return df->load(parser,ftr_ptr,oper);

 }
void zf_feature::display(z_file& f,void* obj)
{
	char* pvar=(char*)obj+_offset;
	f.indent();
	f << _name;
	if(df)
	{
		f << "=";
		df->dump(f,pvar);
	}
	f <<'\n';
}
 z_status zf_feature::get_zf_obj(zf_obj& out, ctext key,zf_obj& parent)
 {
	 if(!df)
		 return zs_no_match;

	 char* membervar=(char*)parent._obj+_offset;
	return  df->get_child_zobj(membervar,key,out);

 }
 /*________________________________________________________________________

zf_list
________________________________________________________________________*/
zf_list::zf_list(ctext id,ctext name,const zf_funcs_obj_list_base* funcs,z_memptr offset,zf_feature_flags flags,ctext desc)
	: zf_feature(id,name,funcs,offset,flags,desc) 
{
	_list_funcs=funcs;

}
void zf_list::display(z_file& f,void* obj)
{
	char* plist_ptr=(char*)obj+_offset;
	z_obj_list_base* list=_list_funcs->get_list(plist_ptr);
	z_obj_list_iter i;
	size_t size=list->size();
	f.indent();
	z_factory* fact=_list_funcs->get_list_obj_fact();

	f<<fact->get_name()<<" " << get_name()<<'['<<(int)size<<']';

	f <<'\n';

}
z_status zf_list::on_tab(z_console* console) 

 { 
	 return zs_ok; 
}
z_status zf_list::add_to_list(z_strlist& list,void* obj)
{
	char* plist_ptr=(char*)obj+_offset;
	z_obj_list_base* objlist=_list_funcs->get_list(plist_ptr);
	z_obj_list_iter i;
	z_string fullname;
	z_string key;
	
	while((objlist->get_next_key(i,key))==zs_ok)
	{
		fullname= get_name();
		fullname<<'['<<key<<']';
		list<<fullname;
	}
	return zs_ok;
}
 z_status zf_list::evaluate(z_factory_controller& controller, zf_obj& o,zf_feature_flags oper)
 {

	return zs_end_of_list;	 //Evaluation is done!
 }

/*________________________________________________________________________

zf_action
________________________________________________________________________*/
zf_action::zf_action(ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc) 
	: zf_feature(id,name,0,offset,flags,desc) 
{
	//ZT("act id=%s name=%s offset=%llx flags=%llx\n",id,name,offset,flags);
}
void zf_action::display(z_file& f,void* obj)
{
	f.indent();

	f <<  get_name()<<'(';
	size_t i;
	
	for(i=0;i<_params.size();i++)
	{
		if(i)
			f <<',';
		f <<_params[i]-> get_name();

	}

	f <<")\n";
}
z_status zf_action::execute(z_file* f,const zf_obj& obj)
{
	z_string s;
	z_string dbg;

	dbg <<  get_name()<<'(';
	size_t i;
	
	for(i=0;i<_params.size();i++)
	{

		if(i)
			dbg <<',';
		dbg << _params[i]-> get_name();
		dbg << "=";
		_params[i]->get_string_val(s,obj._obj);
		dbg<<s;

	}

	dbg <<")\n";
	ZT("%s\n",dbg.c_str());
	z_status ret=obj._fact->execute_act_ptr	(obj._obj,_offset);
	return ret;
}
 z_status zf_action::load(zp_text_parser &parser,const zf_obj& o,zf_feature_flags oper) 
 {
	z_status status;
	z_string error_msg;
	size_t num_params= _params.size();
	parser.skip_ws();
	bool parens=true;
 	status=parser.test_char('(');
	
	if(status==zs_eof)
		return zs_ok;// no params
	if(status==zs_no_match)
	{
		parens=false;
		if(num_params==0)
		{
			error_msg<<"'"<< get_name()<<"' takes no parameters" ;
			status=zs_bad_parameter;
		}
		else
			status=zs_ok;
	}
	if(status==zs_ok)
	{
		size_t param_index=0;
		while( param_index<num_params)
		{
			z_string s;
			
			zf_feature* param=_params[param_index];
			status=param->load(parser,o,ZFF_PRM);
			if(status)
				break;
			status=parser.test_char(',');
			if(status)
			{
				status=zs_ok;//less params is ok
				break;
			}

			param_index++;
		}
	}
	if(!status)
	{
		parser.skip_ws();
		if(parens)
		{
			status=parser.test_char(')');
			if(status)
				error_msg="Expected ')'.";
		}
		parser.skip_ws();
	}
	if(!status)
	{
		if(parser.eob()==false)
		{
			error_msg<<"Too many parameters for '" << get_name()<<"'";
			status=zs_bad_parameter;
		}
		else
			return zs_ok;
	}
	return Z_ERROR_MSG(status,error_msg);
 }

 z_status zf_action::evaluate_textp(zp_text_parser &parser, const zf_obj& o,zf_feature_flags oper,int index)
 {
	z_status status=load(parser,o,oper);
	if(status==zs_ok)				   
		return 	execute(&zout,o);
	return status;
 }
  z_status zf_action::evaluate(z_factory_controller& controller, zf_obj& o,zf_feature_flags oper)
 {
	z_status status=load(controller.get_parser(),o,oper);
	if(status==zs_ok)				   
		return 	execute(&zout,o);
	return zs_end_of_list;	 //Evaluation is done!
 }
/*________________________________________________________________________

zf_child_obj
________________________________________________________________________*/
zf_child_obj::zf_child_obj(ctext id,ctext name,const zf_var_funcs_base* funcs,z_memptr offset,zf_feature_flags flags,ctext desc)
	: zf_feature(id,name,funcs,offset,flags,desc) 
{

}
void zf_child_obj::display(z_file& f,void* obj)
{
	char* pvar=(char*)obj+_offset;
	f.indent();
	z_factory* fact=df->get_fact_from_child_vobj(pvar);

	f<<fact->get_name()<<" " <<  get_name();

	f <<'\n';
}
 z_status zf_child_obj::evaluate(z_factory_controller& controller, zf_obj& o,zf_feature_flags oper)
 {

	return zs_end_of_list;	 //Evaluation is done!
 }
/*________________________________________________________________________

zf_prop
________________________________________________________________________*/
zf_prop::zf_prop(ctext id,ctext name,const zf_var_funcs_base* funcs,z_memptr offset,zf_feature_flags flags,ctext desc)
	: zf_feature(id,name,funcs,offset,flags,desc) 
{

}
 z_status zf_prop::get_string_val(z_string& out, void* object,int index)
 {
 	void* ftr_ptr=(char*)object+_offset;

  	df->get(out,ftr_ptr,0,index);
	return zs_ok;
 }
 /*
 z_status zf_prop::load(zp_text_parser &parser,const zf_obj& o,zf_feature_flags oper) 
 {
	 if(!(oper & _flags)) return zs_skipped;
	void* ftr_ptr=(char*)o._obj+_offset;
	return df->load(parser,ftr_ptr,oper);

 }	*/
 z_status zf_prop::evaluate_textp(zp_text_parser &parser, const zf_obj& o,zf_feature_flags oper,int index)
 {
	z_status status;
	if(parser.test_char('=')==zs_ok)
	{
		status=load( parser,o,oper);
		return status;
	}
	z_string str;
	get_string_val(str,o._obj,index);
	zout <<  get_name()<<"="<<str<<"\n";
	return zs_ok;//???

	
 } 
  z_status zf_prop::evaluate(z_factory_controller& controller, zf_obj& o,zf_feature_flags oper)
 {

	return zs_end_of_list;	 //Evaluation is done!
 }
z_status zf_prop::set_from_value(zp_value* val,void *obj) 
{
	void* ftr_ptr=(char*)obj+_offset;
	return df->set_from_value(val,ftr_ptr);

}


void zf_prop::display(z_file& f,void* obj)
{
	char* pvar=(char*)obj+_offset;
	f.indent();
	f <<  get_name();
	if(df)
	{
		f << "=";
		df->dump(f,pvar);
	}
	f <<'\n';
}
/*________________________________________________________________________

z_factory
________________________________________________________________________*/

z_factory_dyn& z_factory::init_dynamic()
{
	if(_dynamic)
		return *_dynamic;

	_dynamic=z_new z_factory_dyn();
	if(get_static_feature_count()==0)
		return *_dynamic;

	size_t index;
	for(index=0;index<get_static_feature_count();index++)
	{
		const zf_var_entry* ent=0;
		const zf_var_funcs_base* funcs=0;
		zf_feature_type type;

		ent=_get_var_entry(index);
		Z_ASSERT(ent);
		Z_ASSERT(ent->fp_var_func_get);
		funcs=ent->fp_var_func_get();
		type=funcs->get_type();

		switch(type)
		{
		case zf_ft_act:
			add_act(ent->name,ent->name,(z_memptr)ent->offset,ZFF_ACT_DEF /* flags! */,"?");
			break;
		case zf_ft_obj:
		case zf_ft_param:
		case zf_ft_var:
		case zf_ft_obj_list:
			add_prop(ent->name,ent->name,funcs,(z_memptr)ent->offset,ZFF_PROP/* flags! */,"?");
			break;
		default:
			Z_ERROR_MSG(zs_unknown_error,"Unknown feature type: %d",ent->type);
			break;
		}
	}
	return *_dynamic;
}
 zf_feature* z_factory::add_feature(const zf_var_funcs_base* vfuncs,ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc)
{
	zf_feature* feat=get_feature_by_id(id);

	if(feat)
	{
		feat->_flags |=flags;
		
	}
	else
	{
		feat=  vfuncs->create_feature(id,name,offset,flags,desc);
		Z_ASSERT(feat);
	 	init_dynamic().features.add(feat);
	}
	return feat;


 }

zf_action* z_factory::add_act(ctext id,ctext name,z_memptr act_addr,zf_feature_flags flags,ctext desc)
{
	zf_action* action=z_new	zf_action(id,name,*(z_memptr*)&act_addr,flags,desc);
	init_dynamic().features.add(action);
	return action;
}

zf_action* z_factory::add_act_params(ctext id,ctext name,z_memptr act_addr,zf_feature_flags flags,ctext desc,int num_params,...)
{
	int i;
	zf_action* action=add_act(id,name,act_addr,flags,desc);
	va_list ArgList;
	va_start (ArgList, num_params);
	for (i=0;i<num_params;i++)
	{
		zf_feature* p=va_arg(ArgList,zf_feature*);
		if(!p)
		{
			Z_ERROR_MSG(zs_bad_parameter,"Cannot add parameter #%d to action '%s'\n",i,id);
			return 0;
		}
		action->_params.add( p);
	}
	return action;
}

zf_feature* z_factory::add_prop(ctext id,ctext name,const zf_var_funcs_base* f,z_memptr offset,zf_feature_flags flags,ctext desc)
{

	zf_feature* feat=get_feature(id);
	
	if(feat)
	{
		
		
	}
	else
	{
		feat=z_new	zf_prop(id,name,f,offset,flags,desc);
	 	init_dynamic().features.add(feat);
	}
	return feat;
}
zf_child_obj* z_factory::add_obj(ctext id,ctext name,const zf_var_funcs_base* f,z_memptr offset,zf_feature_flags flags,ctext desc)
{
	zf_child_obj* feat=z_new	zf_child_obj(id,name,f,offset,flags,desc);
	init_dynamic().features.add(feat);
	return feat;
}


zf_list* z_factory::add_list(ctext id,ctext name,const zf_funcs_obj_list_base* f,z_memptr offset,zf_feature_flags flags,ctext desc)
{
	zf_list* feat=z_new	zf_list(id,name,f,offset,flags,desc);
	init_dynamic().features.add(feat);
	return feat;
}

/*________________________________________________________________________

z_dynamic_factory_list
________________________________________________________________________*/

void z_dynamic_factory_list::add(z_factory* f)
{
	_list.add(f);
}
z_factory*  z_dynamic_factory_list::get_by_name(ctext name)
{
	return _list.get_by_key(name);
}
z_factory*  z_dynamic_factory_list::get_by_type(ctext t)
{
	size_t i;
	for(i=0;i<_list.size();i++)
	{
		ctext tin=_list[i]->get_type_info_name();
		if (strcmp(tin,t)==0)
			return _list[i];
	}
	return 0;
}

z_dynamic_factory_list& get_factories_dynamic()
{
	static z_dynamic_factory_list g_factories_dynamic;
	return 	g_factories_dynamic;

}


