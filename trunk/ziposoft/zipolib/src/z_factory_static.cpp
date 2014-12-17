#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory.h"
#include "zipolib/include/z_parse.h"
#include "zipolib/include/z_parse_text.h"


z_factory* _pgz_factory_none=0;


/*________________________________________________________________________

z_factory
________________________________________________________________________*/

ctext z_factory::get_map_key() const
{
	return get_name();

}
z_factory* z_factory::get_base_factory() const
{
	return get_info().baseclass;

}

	   
z_status z_factory::get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter) const
{
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	z_status status=get_var_info(var_name,offset,funcs);
	if(status)
		return status;
	char* pvar=(char*)obj+offset;

	*ppChild=pvar;
	return zs_success;

}
z_status z_factory::get_child_obj_ptr(void* obj,ctext var_name,void** ppChild,z_obj_list_iter& iter) const
{
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	z_status status=get_var_info(var_name,offset,funcs);
	if(status)
		return status;
	char* pvar=(char*)obj+offset;

	*ppChild=funcs->get_ptr(pvar,iter);
	return zs_success;


}
z_status z_factory::set_var_as_string(void* obj,ctext var_name,ctext value)	const
{
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	z_status status=get_var_info(var_name,offset,funcs);
	if(status)
		return status;
	if(!funcs)
		return zs_operation_not_supported; //could be ACT

	char* pvar=(char*)obj+offset;
	funcs->set(value,pvar,0);
	return zs_success;
}
z_status z_factory::get_var_as_string(void* obj,ctext var_name,z_string& value) const
{
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	z_status status=get_var_info(var_name,offset,funcs);
	if(status)
		return status;
	if(!funcs)
		return zs_operation_not_supported; //could be ACT
	char* pvar=(char*)obj+offset;
	funcs->get(value,pvar,0);
	return zs_success;
}
z_status z_factory::create_child(void* obj,ctext var_name,z_factory* new_child_type,void** ppChild) const
{
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	z_status status=get_var_info(var_name,offset,funcs);
	if(status)
		return status;
	if(!funcs)
		return zs_operation_not_supported; //could be ACT
	char* pvar=(char*)obj+offset;


	void* newobj=funcs->create_obj(pvar,new_child_type);
	*ppChild=newobj;
	if(newobj)
		return zs_success;
	return Z_ERROR_MSG(zs_error,"Could not create child object %s->%s[%s]\n", get_name(),var_name,	new_child_type->get_name());

}
void z_factory::clear_all_vars (void* obj) const
{
	size_t index=0;
	const zf_var_funcs_base* funcs=0;
	ctext name=0;
	z_memptr offset=0;
	while(get_var_info_i(index,name,offset,funcs)==zs_ok)	 
	{
		char* pvar=(char*)obj+offset;
		if(funcs)
			funcs->clear(pvar);
		index++;
	}
}
z_status z_factory::execute_act(void* obj,ctext name,int* pret) const
{
	const zf_var_funcs_base* funcs=0;
	z_memptr offset=0;
	z_status status=get_var_info(name,offset,funcs);
	if(status)
		return status; 
	int ret=execute_act_ptr	(obj,offset);
	if(pret)
		*pret=ret;
	return zs_success;
}
z_status z_factory::load_obj_contents_static(zp_text_parser &parser,void* pObj) const
{
	z_string s;

	z_status status=zs_ok;
	while(status==zs_ok)
	{
		parser.skip_ws();
		status=parser.test_any_identifier();
		if(status==zs_no_match)
			return zs_ok;
		if(status)
			break;
		parser.get_match(s);
		z_memptr offset;
		const zf_var_funcs_base* funcs;
		status=get_var_info(s.c_str(),offset,funcs);
 		if(status)
			break;

		char* memvar=(char*)pObj +offset; 
		status=funcs->assign(parser,memvar,0);
 		if(status)
			break;
	}
	if(status==zs_eof)
		return zs_ok;
	return status;//Don't log error, should already be logged.
}


void z_factory::dump_obj_contents_static(z_file& f,void* obj) const
{
	size_t index=0;
	ctext name=0;
	z_memptr offset=0;
	const zf_var_funcs_base* funcs=0;
	z_string value;
	while(get_var_info_i(index,name,offset,funcs)==zs_ok)	 
	{
		if(funcs)//only vars, not acts
		{
			char* pvar=(char*)obj+offset;
			f.indent();
			f << name;
			f << "=";
			funcs->dump(f,pvar);
			f <<'\n';
		}
		index++;
	}
}
void z_factory::dump_obj_static(z_file& f,void* obj) const
{
	f.indent();
	f << get_name()<<"{\n";
	f.indent_inc();
	dump_obj_contents_static(f,obj);
	f.indent_dec();
	f.indent();
	f<< "}\n";
}
void z_factory::dump_static(z_file& f) const
{
	size_t index=0;
	ctext name=0;
	z_memptr offset=0;
	const zf_var_funcs_base* funcs=0;
	
	z_string value;
	f.indent();
	f << get_name()<<"{\n";
	f.indent_inc();

	while(get_var_info_i(index,name,offset,funcs)==zs_ok)	 
	{
		f.indent();
		f << name<<'\n';
		index++;
	}
	f.indent_dec();
	f.indent();
	f<< "}\n";
}

/*________________________________________________________________________

z_factory
________________________________________________________________________*/




const zf_var_entry* z_factory::_get_var_entry (ctext name) const
{
	size_t i;
	const zf_var_entry* list=get_var_list();
	for(i=0;i<get_static_feature_count();i++)
		if(strcmp(name,	list[i].name)==0)
		{
			return &list[i];
		}
	return 0;		

}
const zf_var_entry* z_factory::_get_var_entry (size_t i) const
{
	if(i>=get_static_feature_count())
		return 0;		
	return &get_var_list()[i];

}




z_status z_factory::get_var_info_i(size_t index,ctext& name,z_memptr &offset,
								   const zf_var_funcs_base*& funcs)  const
{
	/*
	z_factory* base=get_base_factory();
	if(base)
	{
		size_t numbase=base->get_num_features();
		if(index<numbase)
			return base->get_var_info_i(index,name,offset,funcs);
		index-=numbase;
	}
	*/
	const zf_var_entry* ent=0;
	if(_dynamic)
	{
 		zf_feature* f=_dynamic->features[index];
		if(f)
		{
			name=f->_name;
			offset=f->_offset;
			funcs=f->df;
			return zs_ok;
		}
	}
	else
	{
		ent=_get_var_entry(index);
		if(ent)
		{	
			name=ent->name;
			offset=ent->offset;
			funcs=0;
			if(ent->fp_var_func_get)
				funcs=ent->fp_var_func_get();
			return zs_ok;	
		}
	}
	return zs_item_not_found;	
}
zf_feature* z_factory::get_feature_by_id(ctext name) 
{
	init_dynamic();
	zf_feature* f=_dynamic->features.get_by_key(name);
	if(f)
		return f;
	return 0;
}
zf_feature* z_factory::get_feature(ctext name) 
{
	init_dynamic();
	zf_feature* f=_dynamic->features.get_by_name(name);
	if(f)
		return f;
	return 0;
}

z_status z_factory::get_var_info(ctext name,z_memptr &offset,const zf_var_funcs_base*& funcs) const
{
	if(_dynamic)
	{
 		zf_feature* f=_dynamic->features.get_by_name(name);
		if(f)
		{
			offset=f->_offset;
			funcs=f->df;
			return zs_ok;
		}
	}
	else //static OR dynamic. NOT BOTH
	{
		const zf_var_entry* ent=0;
		ent=_get_var_entry(name);
		if(ent)
		{
			offset=ent->offset;
			if(ent->fp_var_func_get)
				funcs=ent->fp_var_func_get();
			else
				funcs=0;
			return zs_ok;		
		}
		/*
		z_factory* base=get_base_factory();
		if(base)
			return base->get_var_info(name,offset,funcs);	
			*/
	}

	return zs_item_not_found;
		
}


//C:\Users\218015568\Documents\Visual Studio 2012\Templates\ProjectTemplates

/*________________________________________________________________________

GLOBAL z_factory functions
________________________________________________________________________*/
extern "C" {
const zp_module_entry *zp_module_master_list_default[]={0};
const  int zp_module_master_list_size_default=0;
}


z_factory*  _zf_get_static_factory(ctext text,bool getbyname)
{
	int i_module;
	for(i_module=0;i_module<zp_module_master_list_size;i_module++)
	{
		const zp_module_entry* p_module=zp_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const zp_module_fact_entry& p_obj_entry=p_module->facts[i_obj];
			z_factory* fact=p_obj_entry.fact;
			ctext name_or_type;
			if(getbyname)
				name_or_type=fact->get_name();
			else
				name_or_type=fact->get_type_info_name();
			if(strcmp(name_or_type,text)==0)
				return fact;
		}
	}
	return 0;
}
z_factory*  zf_get_factory(ctext name)
{
	
	z_factory* f=	 get_factories_dynamic().get_by_name(name);
	if(f)
		return f;

	f=_zf_get_static_factory(name,true);
	return f;
}
z_factory*  zf_get_factory_by_type(ctext type)
{
	z_factory* f=	 get_factories_dynamic().get_by_type(type);
	if(f)
		return f;
	f=_zf_get_static_factory(type,false);
	return f;
}
void*  zfs_create_obj_by_type(ctext name)
{
	z_factory*  f=zf_get_factory( name);
	if(f)
		return f->create_default_obj();
	return 0;


}

void  zo_factory_list_dump()
{
	int i_module;
	int depth=0;
	for(i_module=0;i_module<zp_module_master_list_size;i_module++)
	{
		const zp_module_entry* p_module=zp_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const zp_module_fact_entry& p_obj_entry=p_module->facts[i_obj];
			z_factory* fact=p_obj_entry.fact;
			zout.indent_reset();
			fact->dump_static(zout);

		}
	}

}
z_status zf_create_obj_from_text_stream_static(zp_text_parser &parser, z_factory* &factory,void* &objpointer) 
{
	parser.skip_ws();
	z_status status=parser.test_any_identifier();
	if(status==zs_no_match)
		return status;
	if(status)
		return Z_ERROR_MSG(status,"Error loading obj type");
	z_string s;
	parser.get_match(s);
	factory=zf_get_factory(s);
	if(!factory)
		return Z_ERROR_MSG(zs_cannot_create_virtual_obj,"Unknown obj type \"%s\"",s.c_str());

	if(objpointer==0)//we need to create it
	{
		objpointer=factory->create_default_obj();

	}
	parser.skip_ws();
	status=parser.test_char('{');
	if(status)
		return Z_ERROR_MSG(status,"Expected '{' ");


	status=factory->load_obj_contents_static(parser,objpointer);
 	if(status)
		return status;

	parser.skip_ws();
	status=parser.test_char('}');
	if(status)
		return Z_ERROR_MSG(status,"Expected '}' ");



	return zs_ok;

}
