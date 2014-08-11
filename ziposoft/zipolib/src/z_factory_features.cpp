#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory_controller.h"
#include "zipolib/include/z_factory_var_funcs.h"
#include "zipolib/include/z_parse_text.h"

/*________________________________________________________________________

zf_feature
________________________________________________________________________*/
zf_feature::zf_feature() 
{
	df=0;
	_offset=0;

}
zf_feature::zf_feature(ctext name,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc) 
{
	_name=name;
	df=funcs;
	_offset=offset;
	_description=desc;

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


	void * subobj=df->get_sub_obj(membervar,key);
	if(!subobj)
	{
		return zs_no_match; //not an object
	}

	z_factory* fact=df->get_fact_from_obj(subobj);
	if(!fact)
		return Z_ERROR_MSG(zs_item_not_found,"Object type not found");			
	out._obj=subobj;
	out._fact=fact;

	return zs_ok;
 }
 /*________________________________________________________________________

zf_list
________________________________________________________________________*/
zf_list::zf_list(ctext name,const zf_funcs_obj_list_base* funcs,z_memptr offset,ctext desc)
	: zf_feature(name,funcs,offset,desc) 
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

	f<<fact->get_name()<<" " << _name<<'['<<(int)size<<']';

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
		fullname=_name;
		fullname<<'['<<key<<']';
		list<<fullname;
	}
	return zs_ok;
}
 z_status zf_list::evaluate(z_factory_controller& controller, zf_obj& o)
 {

	return zs_end_of_list;	 //Evaluation is done!
 }

/*________________________________________________________________________

zf_action
________________________________________________________________________*/
zf_action::zf_action(ctext name,z_memptr offset,ctext desc) : zf_feature(name,0,offset,desc) 
{

}
void zf_action::display(z_file& f,void* obj)
{
	f.indent();

	f << _name<<'(';
	size_t i;
	
	for(i=0;i<_params.size();i++)
	{
		if(i)
			f <<',';
		f <<_params[i]->_name;

	}

	f <<")\n";
}
int zf_action::execute(z_file* f,zf_obj& obj)
{
	z_string s;
	if(f)
	{
		*f << _name<<'(';
		size_t i;
	
		for(i=0;i<_params.size();i++)
		{

			if(i)
				*f <<',';
			*f << _params[i]->_name;
			*f << "=";
			_params[i]->get_string_val(s,obj._obj);
			*f<<s;

		}

		*f <<")\n";
	}

	int ret=obj._fact->execute_act_ptr	(obj._obj,_offset);
	return ret;
}
 z_status zf_action::load(zp_text_parser &parser, zf_obj& o) 
 {
	z_status status;
 	if(parser.test_char('(')==zs_ok)
	{

		size_t param_index=0;
		while( 1)
		{
			z_string s;
			if(param_index>=_params.size())
			{
				return Z_ERROR_MSG(zs_error,"Too many parameters\n");//???
			}				
			zf_feature* param=_params[param_index];
			status=param->load(parser,o);
			if(status)
				break;
			status=parser.test_char(',');
			if(status)
				break;

			param_index++;
		}
		if(parser.test_char(')'))
			return Z_ERROR_MSG(zs_error,"Expected ')'\n");//???
	}											   

	return zs_ok;//???
 }

 z_status zf_action::evaluate1(zp_text_parser &parser, zf_obj& o,int index)
 {
	z_status status=load(parser,o);
	if(status==zs_ok)				   
		return 	execute(&gz_out,o);
	return status;
 }
  z_status zf_action::evaluate(z_factory_controller& controller, zf_obj& o)
 {
	z_status status=load(controller.get_parser(),o);
	if(status==zs_ok)				   
		return 	execute(&gz_out,o);
	return zs_end_of_list;	 //Evaluation is done!
 }
/*________________________________________________________________________

zf_child_obj
________________________________________________________________________*/
zf_child_obj::zf_child_obj(ctext name,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc)
	: zf_feature(name,funcs,offset,desc) 
{

}
void zf_child_obj::display(z_file& f,void* obj)
{
	char* pvar=(char*)obj+_offset;
	f.indent();
	z_factory* fact=df->get_fact_from_obj(pvar);

	f<<fact->get_name()<<" " << _name;

	f <<'\n';
}
 z_status zf_child_obj::evaluate(z_factory_controller& controller, zf_obj& o)
 {

	return zs_end_of_list;	 //Evaluation is done!
 }
/*________________________________________________________________________

zf_prop
________________________________________________________________________*/
zf_prop::zf_prop(ctext name,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc)
	: zf_feature(name,funcs,offset,desc) 
{

}
 z_status zf_prop::get_string_val(z_string& out, void* object,int index)
 {
 	void* ftr_ptr=(char*)object+_offset;

  	df->get(out,ftr_ptr,index);
	return zs_ok;
 }
 z_status zf_prop::load(zp_text_parser &parser, zf_obj& o) 
 {
	void* ftr_ptr=(char*)o._obj+_offset;
	return df->load(parser,ftr_ptr);

 }
 z_status zf_prop::evaluate1(zp_text_parser &parser, zf_obj& o,int index)
 {
	z_status status;
	if(parser.test_char('=')==zs_ok)
	{
		status=load( parser,o);
		return status;
	}
	z_string str;
	get_string_val(str,o._obj,index);
	gz_out << _name<<"="<<str<<"\n";
	return zs_ok;//???

	
 } 
  z_status zf_prop::evaluate(z_factory_controller& controller, zf_obj& o)
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
	f << _name;
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

	_dynamic=new z_factory_dyn();
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
			add_act(ent->name,(z_memptr)ent->offset,"?");
			break;
		case zf_ft_obj:
		case zf_ft_param:
		case zf_ft_var:
		case zf_ft_obj_list:
			add_prop(ent->name,funcs,(z_memptr)ent->offset,"?");
		default:
			Z_ERROR_MSG(zs_error,"Unknown feature type: %d",ent->type);
			break;
		}
	}
	return *_dynamic;
}
 zf_feature* z_factory::add_feature(zf_feature* f)
 {

	init_dynamic().features.add(f);
	return f;


 }

zf_action* z_factory::add_act(ctext name,z_memptr act_addr,ctext desc)
{
	zf_action* action=z_new	zf_action(name,*(z_memptr*)&act_addr);
	init_dynamic().features.add(action);
	return action;
}

zf_action* z_factory::add_act_params(ctext name,z_memptr act_addr,ctext desc,int num_params,...)
{
	int i;
	zf_action* action=add_act(name,act_addr,desc);
	va_list ArgList;
	va_start (ArgList, num_params);
	for (i=0;i<num_params;i++)
	{
		zf_feature* p=va_arg(ArgList,zf_feature*);
		action->_params.add( p);
	}
	return action;
}

zf_feature* z_factory::add_prop(ctext name,const zf_var_funcs_base* f,z_memptr offset,ctext desc)
{
	zf_feature* feat=z_new	zf_prop(name,f,offset,desc);
	init_dynamic().features.add(feat);
	return feat;
}
zf_child_obj* z_factory::add_obj(ctext name,const zf_var_funcs_base* f,z_memptr offset,ctext desc)
{
	zf_child_obj* feat=z_new	zf_child_obj(name,f,offset,desc);
	init_dynamic().features.add(feat);
	return feat;
}


zf_list* z_factory::add_list(ctext name,const zf_funcs_obj_list_base* f,z_memptr offset,ctext desc)
{
	zf_list* feat=z_new	zf_list(name,f,offset,desc);
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
	return _list.get_by_name(name);
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


