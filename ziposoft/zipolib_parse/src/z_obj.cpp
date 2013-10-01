#include "zipolib/include/z_obj.h"
#include "zipolib/include/z_file.h"
#include "zipolib/include/z_parse.h"
#include "zipolib/include/z_obj_man.h"


z_obj_man_data::z_obj_man_data()
{
	_parent_obj=0;
	_fet_ent._desc="";
	_fet_ent._display_name="";
	_fet_ent._ftr_specific=0;
	_fet_ent._internal_name="";
	_fet_ent._options=0;
	_fet_ent._short_id="";
	_fet_ent._type=0;
}
/*
	ctext name_internal;
	ctext name_display;
	ctext desc;
	zo_create_item_func create_func;
	zo_ftr_entry* var_list;
	const int var_list_size;
	const z_obj_fact* base_fact;
	ctext parse_string;
	*/

void z_obj_man_data::init_from_fact(const z_obj_fact* f,z_obj* parent)
{
	_parent_obj=parent;
	_fet_ent._desc=f->desc;
	_fet_ent._display_name=f->name_display;
	_fet_ent._ftr_specific=0;
	_fet_ent._internal_name=f->name_internal;
	_fet_ent._options=0;
	_fet_ent._short_id=f->name_internal;
	_fet_ent._type=0;

}

ctext zo_ftr_get_name(zo_ftr_entry* fe)
{
	ctext name=fe->_display_name;
	if(name)
	{
		if(name[0]==0)
		{
			Z_ASSERT(0);
			name=0;
		}
	}
	if(!name)
		name=fe->_internal_name;
	return name;
}


z_obj::z_obj()
{
	_man_data=0;
}
ctext z_obj::get_id()
{
	if(_man_data)
		return _man_data->_fet_ent._short_id;
	const z_obj_fact* fact=get_fact();
	return fact->name_internal;

	return 0;
	
}
/*
bool z_obj::compare_id(ctext id_to_compare)
{
	if(!get_id())
		return false;
	return z_str_same(id_to_compare,get_id());
}
*/
void z_obj::get_path(z_string& path)
{
	z_obj* o=get_parent_obj();
	if(o)
	{
		o->get_path(path);
		path<<'/';
	}
	path<<get_name();

}


//ZP_OBJ(zo_module,zp_obj,"module", "#'\t':{_name}ident:'<':#{_default}ident:'>':#'\n'",ZPV(_name),ZPV(_default));
zo_ftr_entry* z_obj::get_feature(ctext f)
{
	const z_obj_fact* fact=get_fact();
	if(!fact) return 0;
	return get_feature(fact,f);
}
z_status z_obj::get_feature_map(zo_manipulator* man,zo_feature_list& list,U32 feature_type,bool include_alias)
{
	man->get_feature_map_by_fact(list,get_fact(),feature_type,include_alias);
	return zs_ok;
}


zo_ftr_entry* z_obj::get_feature(const z_obj_fact* fact,ctext f)
{
	zo_ftr_entry* fe=0;
	if(!fact) return 0;
	const z_obj_fact* fact_base=fact->base_fact;
	if((fact_base)&&(fact_base->var_list_size))
	{
		fe=get_feature(fact_base,f);
		if(fe)
		{
			return fe;
		}
	}
	int i;
	for(i=0;i<fact->var_list_size;i++)
	{
		fe=&fact->var_list[i];
		ctext name=zo_ftr_get_name(fe);
		if(z_str_same(name,f))
			return fe;
		if(z_str_same(fe->_short_id,f))
			return fe;
	}
	return 0;
}


const z_obj_fact z_obj::FACT=
{
	"z_obj",
	"",
	0,//zo_create_item_func func;
	0,// zo_ftr_entry* var_list;
	0,//int var_list_size;
	0,// z_obj_fact* base_fact;
	0
};
const z_obj_fact* z_obj::get_fact() { return &z_obj::FACT; }


void z_obj::init_man_data(z_obj* parent_obj)
{
	if(!_man_data)
	{
		_man_data=new z_obj_man_data();
		_man_data->init_from_fact(get_fact(),parent_obj);
		
	}
}
ctext z_obj::get_name() 
{ 
	if(_man_data)
		if(_man_data->_name)
			return _man_data->_name;
	return z_obj_fact_get_name(get_fact());
};
ctext z_obj::get_type()
{
	return z_obj_fact_get_name(get_fact());

}

void z_obj::set_name(ctext t)
{
	get_man_data()->_name=t;

}
void z_obj::set_parent_obj(z_obj* obj)
{
	get_man_data()->_parent_obj=obj;

}
zo_ftr_entry* z_obj::get_feature_for_obj()
{
	if(_man_data)
		return &(get_man_data()->_fet_ent);
	return 0;
}
z_obj* z_obj::get_parent_obj()
{
	if(_man_data)
		return _man_data->_parent_obj;
	return 0;
}
z_status z_obj_container::feature_manipulate(zo_fet_man_context* context)
{
	type_zo_memvar_oper 	oper=context->_oper;
	ctext var_name=context->_name;
	zo_manipulator* m=context->_man;


	z_obj* obj=get_obj(var_name);
	if(obj)
		return m->feature_callback_pchild(context,0,obj);
	return zs_feature_not_found;
}

z_obj_man_data* z_obj::get_man_data()
{
	if(!_man_data)
	{
		_man_data=new z_obj_man_data();
		
	}
	return _man_data;
}

ctext z_obj_fact_get_name(const z_obj_fact* fact)
{
	ctext class_name=fact->name_display;
	if(!class_name)
		class_name=fact->name_internal;
	return class_name;
}




const z_obj_fact*  zo_get_factory_by_name_and_length(ctext name,size_t len)
{
	int i_module;
	for(i_module=0;i_module<z_module_master_list_size;i_module++)
	{
		const z_module_entry* p_module=z_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const z_module_obj_entry& p_obj_entry=p_module->facts[i_obj];
			const z_obj_fact* fact=p_obj_entry.fact;
			if(strncmp(z_obj_fact_get_name(fact),name,len)==0)
				return fact;
		}
	}
	return 0;
}
const z_obj_fact*  zo_get_factory(ctext name)
{
	int i_module;
	for(i_module=0;i_module<z_module_master_list_size;i_module++)
	{
		const z_module_entry* p_module=z_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const z_module_obj_entry& p_obj_entry=p_module->facts[i_obj];
			const z_obj_fact* fact=p_obj_entry.fact;

			if(z_str_same(name,z_obj_fact_get_name(fact)))
				return fact;
		}
	}
	return 0;
}

void zo_str_container::copy(zo_str_container& other)
{
	other.reset_iter();
	clear();
	size_t len=other.count();
	size_t i;

	for(i=0;i<len;i++)
	{
		add(other.get_next());
	}


}


zo_ftr_entry* z_obj_container::get_feature(ctext f)
{	

	z_obj* obj=get_obj(f);
	if(obj)
		return obj->get_feature_for_obj();
	return z_obj::get_feature(f);



}
z_status z_obj_container::get_feature_map(zo_manipulator* man,zo_feature_list& list,U32 feature_type,bool include_alias)
{
	if (feature_type&ZO_MT_CHILD)
	{
		reset_iter();
		z_obj* obj=0;

		while((obj=get_next_obj()))
		{
			zo_ftr_entry* fe=new zo_ftr_entry();
			fe->_internal_name=obj->get_name();
			fe->_display_name=obj->get_name();
			fe->_short_id=0;
			fe->_desc=0;
			fe->_options=0;
			fe->_type=ZO_MT_CHILD;
			list.add(fe->_display_name,fe);
		}
	}

	return z_obj::get_feature_map(man, list, feature_type, include_alias);
}