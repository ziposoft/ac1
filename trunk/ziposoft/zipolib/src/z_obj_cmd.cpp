#include "zipolib_cpp_pch.h"
#if 0
#include "z_obj_cmd.h"


zo_man_cmd::zo_man_cmd(void* root)
{
	_root_obj=root;
	_obj_current=_root_obj;
	_obj_current_tmp=_root_obj;


}
z_status zo_man_cmd::parse_line(ctext text)
{
	z_status status=_parser.parse_obj(&_cmd_line_obj,text);
	if(status)
	{
		_parser.report_error();

		return status;
	}
	return status;
}
z_status zo_man_cmd::execute_feature(void* obj)
{
	z_status status=zs_ok;
	ctext feature=_cmd_line_obj._feature._name;
	zf_var_entry* f=obj->get_feature(feature);
	if(!f)
	{
		
		return zs_no_entry_for_item;
	}
	if(_cmd_line_obj._feature._subscript)
	{
		//This is a list item
		z_string fullname;
		void* list=0;
		feature_objlist_get(obj,feature,&list);
		void* subobj=list->get_obj(_cmd_line_obj._feature._subscript_id);
		
		subobj->set_name(_cmd_line_obj._feature.get_full_name(fullname));
		subobj->set_parent_obj(_obj_current);
		_obj_current=subobj;
		return zs_ok;

	}

	//Assign parameters
	size_t num_params=_cmd_line_obj._params._param_list.size();
	if(num_params)
	{
		const zo_action_params* ap=(const zo_action_params*)(f->_ftr_specific);
		if(!ap)
		{
			return Z_ERROR_MSG(zs_error,"feature has no param list");
		}
		if(num_params>ap->size)
		{
			return Z_ERROR_MSG(zs_error,"Too many parameters for \"%s\"",feature);

		}
		U32 i;
		for (i=0;i<ap->size;i++)
		{
			if(i>num_params)
				break;
			ctext param_name=ap->list[i];
			if(!param_name)
			{
				return Z_ERROR_MSG(zs_error,"Action  \"%s\" has no parameters",feature);
			}
			zf_var_entry* param_feature=obj->get_feature(param_name);
			if(!param_feature)
			{
				return Z_ERROR_MSG(zs_error,"Bad parameter \"%s\"",param_name);
			}
			status=feature_set_from_value(obj,_cmd_line_obj._params._param_list[i],param_feature);
		}
	}

	if(_cmd_line_obj._assignment)
	{
		status=feature_set_from_value(obj,&_cmd_line_obj._assign_val,f);

	}
	else
	{
		status= feature_execute(obj,f);
	}
	return status;


}
z_status zo_man_cmd::navigate_feature(ctext name)
{
	void* child=get_child_obj(name,_obj_current_tmp);
	if(!child)
	{
		return Z_ERROR_MSG(zs_child_not_found,"Could not find child object \"%s\"",name);
	}
	child->set_parent_obj(_obj_current_tmp);
	_obj_current_tmp=child;


	return zs_ok;

}
z_status zo_man_cmd::execute_line(ctext text)
{
	z_status status=parse_line(text);
	if(status)
		return status;

//find path
	if(_cmd_line_obj._root_slash)
	{
		_obj_current_tmp=_root_obj;
	}
	else
	{
		_obj_current_tmp=_obj_current;
	}
	if(_obj_current_tmp)
	{
		if(_cmd_line_obj._path_list.size())
		{
			size_t i;
			for (i=0;i<_cmd_line_obj._path_list.size();i++)
			{
				ctext name=_cmd_line_obj._path_list[i];
				status=navigate_feature(name);
				if(status)
					return status;

			}
		}
		if(_cmd_line_obj._object)
		{
			ctext name=_cmd_line_obj._object;
			void* child=get_child_obj(name,_obj_current_tmp);
			if(!child)
			{
				return Z_ERROR_MSG(zs_child_not_found,"Could not find child object \"%s\"",name);
			}
			_obj_current_tmp=child;
		}
		status=execute_feature(_obj_current_tmp);
	}
	else
		status=zs_no_entry_for_item;


	if(status==zs_no_entry_for_item)
		status=execute_feature(this);
	if(status==zs_no_entry_for_item)
	{
		Z_ERROR_MSG(zs_no_entry_for_item,"Unknown feature:\"%s\"",_cmd_line_obj._feature._name.c_str());
	}
	return status;
}
z_status zo_man_cmd::callback_feature_execute_obj(void* pObj,zf_var_entry* fe)
{
	//This is absolutely horrible code.
	
	pObj->set_name(zo_ftr_get_name(fe));
	pObj->set_parent_obj(_obj_current);
	_obj_current=pObj;
	
	return zs_ok;
}

z_status zo_man_cmd::dump_features_by_type(z_file* fp,void* obj,U32 feature_type)
{
	_dump_fp=fp;
	zo_feature_list list;
	zf_var_entry* f;
	int i=0;
	obj->get_feature_map(this,list,feature_type,false);
	list.reset_iter();
	ctext key;
	while(f=list.get_next(key))
	{
		if((f->_type&ZO_MT_LIST)||(f->_type==ZO_MT_CHILD))
		{
			*fp<<"  "<< key;
			//feature_display(obj,f);
		}
		if(f->_type==ZO_MT_PROP)
		{
			*fp<<"  "<< key;
			*fp<<"=";
			feature_display(obj,f);
		}		
		if(f->_type&ZO_MT_ACT)
		{
			*fp<<"  "<< key;
			*fp<<"(";
			const zo_action_params* ap=(const zo_action_params*)(f->_ftr_specific);
			U32 i;
			bool need_comma=false;
			for (i=0;i<ap->size;i++)
			{
				ctext p=ap->list[i];
				if(p)
				{
					if(need_comma)
						*fp<<",";
					*fp<<p;
				}
				need_comma=true;
			}
			*fp<<")";
		}
		*fp<<"\n";
	}
	return zs_ok;
}


z_status zo_man_cmd::dump_features(z_file* fp,void* obj)
{
	if(!obj)
	{
		return Z_ERROR_MSG(zs_bad_argument_2,"No object");
	}
	*fp<< "\nFeatures of \""<<obj->get_map_key()<<"\"\n";
	*fp<< "Properties:\n";
	dump_features_by_type(fp,obj,ZO_MT_PROP);
	*fp<< "\nActions:\n";
	dump_features_by_type(fp,obj,ZO_MT_ACT);
	*fp<< "\nChild Objects:\n";
	dump_features_by_type(fp,obj,ZO_MT_CHILD);
	dump_features_by_type(fp,obj,ZO_MT_LIST);
	return zs_ok;
}
void zo_man_cmd::dump_features(void* obj)
{
	z_factory* fact=obj->get_fact();
	if(!fact)
		return;
	int i_var;
	for(i_var=0;i_var<fact->var_list_size;i_var++)
	{
		zf_var_entry& fe=fact->var_list[i_var];
		gz_out<< "\t\t"<< fe._internal_name;
		if(obj)
			gz_out<<"="<<feature_get_as_string(obj,&fe);

		gz_out<<"\n";
	}
}



void zo_man_cmd::dump()
{
	/*
	int i_module;
	for(i_module=0;i_module<z_module_master_list_size;i_module++)
	{
		const z_module_entry* p_module=z_module_master_list[i_module];
		gz_out<< "MODULE: "<< p_module->module_name<<"\n";
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const z_module_obj_entry& p_obj_entry=p_module->facts[i_obj];
			z_factory* fact=p_obj_entry.fact;
			gz_out<< "\t"<< p_obj_entry.name;
			if(fact->base_fact)
				gz_out<< "::"<< z_obj_fact_get_name(fact->base_fact);
			gz_out<<"\n";
		}
	}*/
}


#define ZO_OBJ_LIST \
	OBJ(zo_man_cmd,none,"generic","generic",0,NO_FTR)

#define Z_MODULE _Z_MODULE(man)
#include "zipolib_parse/include/z_obj.macro"


#endif