#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory.h"
#include "zipolib/include/z_parse_text.h"

/*________________________________________________________________________

zf_feature
________________________________________________________________________*/
zf_feature::zf_feature() 
{
	df=0;
	_offset=0;
	_type=zf_ft_none;

}
zf_feature::zf_feature(ctext name,zf_feature_type t,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc) 
{
	_name=name;
	df=funcs;
	_offset=offset;
	_type=t;
	_description=desc;

}

void* zf_feature::get_memvar_ptr(void* obj,int* iter)
{
	char* pvar=(char*)obj+_offset;
	/*
	if(df)
		return df->get_ptr(pvar,iter);
		*/
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
 z_status zf_feature::get_zf_obj(zf_obj& out, int index,zf_obj& parent)
 {
	 if(!df)
		 return zs_no_match;


	 char* membervar=(char*)parent._obj+_offset;


	void * subobj=df->get_sub_obj(membervar,index);
	if(!subobj)
	{
		return zs_no_match; //not an object
	}

	z_factory* fact=df->get_fact_from_obj(subobj);
	if(!fact)
		return Z_ERROR(zs_error);			
	out._obj=subobj;
	out._fact=fact;

	return zs_ok;
 }


/*________________________________________________________________________

zf_action
________________________________________________________________________*/
zf_action::zf_action(ctext name,z_memptr offset,ctext desc) : zf_feature(name,zf_ft_act,0,offset,desc) 
{

}
void zf_action::display(z_file& f,void* obj)
{
	f.indent();

	char* pvar=(char*)obj+_offset;
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

 z_status zf_action::evaluate(zp_text_parser &parser, zf_obj& o,int index)
 {
	z_status status=load(parser,o);
	if(status==zs_ok)				   
		return 	execute(&gz_out,o);
	return status;
 }

/*________________________________________________________________________

zf_child_obj
________________________________________________________________________*/
zf_child_obj::zf_child_obj(ctext name,zf_feature_type t,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc)
	: zf_feature(name,t,funcs,offset,desc) 
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

/*________________________________________________________________________

zf_prop
________________________________________________________________________*/
zf_prop::zf_prop(ctext name,zf_feature_type t,const zf_var_funcs_base* funcs,z_memptr offset,ctext desc)
	: zf_feature(name,t,funcs,offset,desc) 
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
 z_status zf_prop::evaluate(zp_text_parser &parser, zf_obj& o,int index)
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

		ent=_get_var_entry(index);
		if(ent->fp_var_func_get) 
			funcs=ent->fp_var_func_get();
		switch(ent->type)
		{
		case zf_ft_act:
			add_act(ent->name,(z_memptr)ent->offset,"?");
			break;
		case zf_ft_obj:
		case zf_ft_param:
		case zf_ft_var:
		case zf_ft_obj_list:
			add_prop(ent->name,ent->type,funcs,(z_memptr)ent->offset,"?");
		default:
			Z_ERROR_MSG(zs_error,"Unknown feature type: %d",ent->type);
			break;
		}
	}
	return *_dynamic;
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

zf_feature* z_factory::add_prop(ctext name,zf_feature_type type,const zf_var_funcs_base* f,z_memptr offset,ctext desc)
{
	zf_feature* feat=z_new	zf_prop(name,type,f,offset,desc);
	init_dynamic().features.add(feat);
	return feat;
}
zf_child_obj* z_factory::add_obj(ctext name,zf_feature_type type,const zf_var_funcs_base* f,z_memptr offset,ctext desc)
{
	zf_child_obj* feat=z_new	zf_child_obj(name,type,f,offset,desc);
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


