#include "z_parse_pch.h"
#include "z_obj_man.h"
#include "z_parse_obj.h"



zo_fet_man_context::zo_fet_man_context(zo_manipulator* man,type_zo_memvar_oper oper,ctext name,zo_ftr_entry* fet_ent)
{
	_man=man;
	_oper=oper;
	_name=name;
	_p_fet_ent=fet_ent;
	_p_assignment_value=0;



}

void zo_manipulator::feature_capture_action_return_value(size_t act_rtn_val)
{


}

z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options, bool& val)
{
	ZTFP("bool& val");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		break;
	case zo_mvo_clear:
		val=false;
		break;
	case zo_mvo_set:
		val=(_member_var_int_data!=0);
		break;
	case zo_mvo_get:
		_member_var_int_data=val;
		break;
	case zo_mvo_dump:
		{
		
		*_dump_fp<< (val? "true":"false");
		}
		break;
	case zo_mvo_get_to_string:
		
		break;
	case zo_mvo_set_from_string:
		val=_member_var_string_data.GetDecVal();
		break;
	}
	return zs_ok;
}
z_status zo_manipulator::access_obj_child(zo_fet_man_context* context, zp_obj_base* pchild)
{
	ZTFP("zp_obj_base* pchild");

	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		if(pchild)
			_member_var_int_data++;
		break;
	case zo_mvo_get:
		_p_member_var_obj=pchild;
		break;
	default:
		return zs_operation_not_supported;
	}
	return zs_ok;
}
z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options, void* member_var)
{
	ZTFP("void* member_var");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		break;
	case zo_mvo_get_to_string:
		_member_var_string_data.Format("%p",member_var);
		break;
	case zo_mvo_exec:
	case zo_mvo_display:
	case zo_mvo_dump:
		{
		z_string tmp;
		tmp.Format("0x%p",member_var);
		*_dump_fp<<tmp;
		}
		break;


	case zo_mvo_set_from_string:
	case zo_mvo_set_from_value:
	default:
		return zs_operation_not_supported;

	}
	return zs_ok;
}
z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options, int& member_var)
{
	ZTFP("int& member_var");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		break;
	case zo_mvo_get_to_string:
		_member_var_string_data=member_var;
		break;
	case zo_mvo_set_from_string:
		member_var=_member_var_string_data.GetDecVal();
		break;
	case zo_mvo_set_from_value:
		member_var=context->_p_assignment_value->_string.GetDecVal();
		break;
	case zo_mvo_exec:
	case zo_mvo_display:
	case zo_mvo_dump:
		*_dump_fp<<member_var;
		break;

	default:
		return zs_operation_not_supported;

	}
	return zs_ok;
}

z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options,z_string& member_var)
{
	ZTFP("z_string& member_var");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		break;
	case zo_mvo_get:
	case zo_mvo_get_to_string:
		_member_var_string_data=member_var;
		break;
	case zo_mvo_set_from_string:
	case zo_mvo_set:
		member_var=_member_var_string_data;
		break;
	case zo_mvo_set_from_value:
		member_var=context->_p_assignment_value->_string;
		break;

	case zo_mvo_clear:
		member_var="";
		break;
		
	case zo_mvo_exec:
	case zo_mvo_display:
	case zo_mvo_dump:
		*_dump_fp<<'\"'<<member_var<<'\"';
		break;
	default:
		return zs_operation_not_supported;
	};
	return zs_ok;
}

z_status zo_manipulator::access_action_params(const zo_action_params* params)
{
	ZTFP("zo_action_params* params");
	_p_member_action_params=	params;
	

	return zs_ok;

}

z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options,zp_obj_base& Obj)
{
	ZTFP("zp_obj_base& Obj");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		_member_var_int_data++;
		break;
	case zo_mvo_reset_iter:
		break;
	case zo_mvo_clear:
		feature_clear(&Obj);
		
		break;
	case zo_mvo_set:
		//????
		break;
	case zo_mvo_get:
	case zo_mvo_get_child_obj:
		_p_member_var_obj=&Obj;
		break;
	case zo_mvo_display:
		break;

	case zo_mvo_dump:
		dump_obj(&Obj);
		break;
	case zo_mvo_set_from_value:
		return load_obj(&Obj,&context->_p_assignment_value->_obj);
	case zo_mvo_exec:
		_p_member_var_obj=&Obj;
		callback_feature_execute_obj(&Obj,context->_p_fet_ent);
		break;
	default:
		return zs_operation_not_supported;
	};
	return zs_ok;
}
z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options,zp_obj_base** p_child_obj)
{
	ZTFP("zp_obj_base** pObj");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		if(p_child_obj)
			_member_var_int_data++;
		break;		

	case zo_mvo_reset_iter:
		break;
	case zo_mvo_clear:
		if(*p_child_obj)
		feature_clear(*p_child_obj);
		break;
	case zo_mvo_set:
		//????
		break;
	case zo_mvo_get:
	case zo_mvo_get_child_obj:
		_p_member_var_obj=*p_child_obj;
		/* TODO make sure man data is set here! */
		break;
	case zo_mvo_display:
		break;
	case zo_mvo_dump:
		if(*p_child_obj)
		dump_obj(*p_child_obj);
		break;
	case zo_mvo_set_from_value:
		return load_obj(*p_child_obj,&context->_p_assignment_value->_obj);
	case zo_mvo_exec:
		_p_member_var_obj=*p_child_obj;
		callback_feature_execute_obj(*p_child_obj,context->_p_fet_ent);
		break;
	default:
		return zs_operation_not_supported;
	};
	return zs_ok;
}

z_status zo_manipulator::feature_callback_pchild(zo_fet_man_context* context,zo_fet_opt options,zp_obj_base* p_child_obj)
{
	ZTFP("zp_obj_base* pObj");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		if(p_child_obj)
			_member_var_int_data++;
		break;		

	case zo_mvo_reset_iter:
		break;
	case zo_mvo_clear:
		if(p_child_obj)
		feature_clear(p_child_obj);
		break;
	case zo_mvo_set:
		//????
		break;
	case zo_mvo_get:
	case zo_mvo_get_child_obj:
		_p_member_var_obj=p_child_obj;
		/* TODO make sure man data is set here! */
		break;
	case zo_mvo_display:
		break;
	case zo_mvo_dump:
			if(p_child_obj)
		dump_obj(p_child_obj);
		break;
	case zo_mvo_set_from_value:
		return load_obj(p_child_obj,&context->_p_assignment_value->_obj);
	case zo_mvo_exec:
		_p_member_var_obj=p_child_obj;
		callback_feature_execute_obj(p_child_obj,context->_p_fet_ent);
		break;
	default:
		return zs_operation_not_supported;
	};
	return zs_ok;
}
z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options,zo_str_container& member_var)
{
	ZTFP("zo_str_container& member_var");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		break;
	case zo_mvo_reset_iter:
		member_var.reset_iter();
		break;
    
	case zo_mvo_clear:
		member_var.clear();
		break;
	case zo_mvo_set:
		member_var.add( _member_var_string_data);
		break;
	case zo_mvo_get:
		{
		ctext s=member_var.get_next();
		if(!s)
			return zs_end_of_list;
		_member_var_string_data=s;
		break;
		}
	case zo_mvo_set_from_value:
		member_var.copy(context->_p_assignment_value->_string_list._list);
		break;

	case zo_mvo_display:
	case zo_mvo_dump:
		member_var.reset_iter();
		*_dump_fp<<"{";
		ctext t;
		if(member_var.count())
		{
			bool comma=false;
			while(t=member_var.get_next())
			{
				if(comma) 
					*_dump_fp<<',';
				*_dump_fp<<"\""<<t<<"\"";
				comma=true;

			}
		}
		*_dump_fp<<"}";
		break;
	default:
		return zs_operation_not_supported;
	};
	return zs_ok;


}

z_status zo_manipulator::feature_callback(zo_fet_man_context* context,zo_fet_opt options,z_obj_container& member_var)
{
	ZTFP("z_obj_container& member_var");
	switch(context->_oper)
	{
	case zo_mvo_get_num_obj:
		_member_var_int_data+=(int)member_var.get_count();
		break;
	case zo_mvo_clear:
	{
		member_var.clear_objs();
		break;
		return zs_ok;
	}
	case zo_mvo_reset_iter:
	{
		member_var.reset_iter();
		return zs_ok;
	}
	case zo_mvo_set:
	{
		member_var.add_obj(_p_member_var_obj);
		return zs_ok;
	}
	case zo_mvo_get:
	{
		//Z_ERROR("calling get!!!");
		_member_obj_container=&member_var;
		return zs_ok;
	}

	case zo_mvo_get_next_from_list:
	{
		_p_member_var_obj=(zp_obj*)member_var.get_next_obj();
		if(!_p_member_var_obj)
			return zs_end_of_list;
		return zs_ok;
	}
	case zo_mvo_dump:
	{
		member_var.reset_iter();
		zp_obj_base* z;
		*_dump_fp<<"{";
		_dump_depth++;
		if(member_var.get_count())
		{
			*_dump_fp<<	"\n";
			while(z=member_var.get_next_obj())
			{
				dump_indent();
				dump_obj(z);
				*_dump_fp<<	"\n";
			}
		}
		dump_indent();
		*_dump_fp<<"}";
		_dump_depth--;
		break;
	}
	case zo_mvo_display:
	{
		*_dump_fp<<	zo_ftr_get_name(context->_p_fet_ent) ;
		/*
		member_var.reset_iter();
		zp_obj_base* z;
		if(member_var.get_count())
		{

			z_string iter_string;
			while(1)
			{
				member_var.get_current_iter_as_string(iter_string);
				z=member_var.get_next_obj();
				if(!z)
					break;
				dump_indent();
				*_dump_fp<<	zo_ftr_get_name(context->_p_fet_ent) <<":"<<iter_string<<'=';
				dump_obj(z);
			}
		}
		*/
		break;
	}
	case zo_mvo_set_from_value:
		{
			member_var.clear_objs();
			context->_p_assignment_value->_child_list.reset_iter();
			zp_obj_parse* op=0;
			while( (op=(zp_obj_parse*)context->_p_assignment_value->_child_list.get_next_obj()) )
			{

				const z_obj_fact* fact=zo_get_factory(op->_name);
				if(!fact)
				{
					return Z_ERROR_MSG(zs_no_entry_for_item,"Unknown class \"%s\"",op->_name.c_str());
				}
				if(!fact->create_func)
				{
					return Z_ERROR_MSG(zs_cannot_create_virtual_obj,"Cannot create virtual obj \"%s\"",op->_name.c_str());

				}
				zp_obj_base* new_obj=(fact->create_func)();
				if(!new_obj)
					return zs_error;
				load_obj(new_obj,op);
				//add_obj(new_obj);

				member_var.add_obj(new_obj);
			}
		}
		break;
	case zo_mvo_exec:
		_p_member_var_obj=&member_var;
		callback_feature_execute_obj(&member_var,context->_p_fet_ent);
		break;
	default:
		return zs_operation_not_supported;
	};

	return zs_ok;
}
zo_manipulator::zo_manipulator()
{
	_dump_depth=0;
	_dump_new_line=false;
}


zp_obj_base* zo_manipulator::get_child_obj(ctext name,zp_obj_base* parent)
{
	ZTF;
	zo_ftr_entry* fe=parent->get_feature(name);
	if(!fe) return 0;
	_p_member_var_obj=0;
	zo_fet_man_context con( this,zo_mvo_get_child_obj,name,fe);
	z_status status=parent->feature_manipulate(&con);

	if(status)
		return 0;
	zp_obj_base* child=_p_member_var_obj;
	if(child)
		child->set_parent_obj(parent);
	return child;
}
int zo_manipulator::feature_get_num_children(zp_obj_base* p_obj,ctext var_id /* 0 for all */)
{
	_member_var_int_data=0;
	zo_fet_man_context con( this,zo_mvo_get_num_obj,var_id,0);
	z_status s=p_obj->feature_manipulate(&con);
	return _member_var_int_data;
}

z_status zo_manipulator::feature_dump(zp_obj_base* p_obj,ctext var_id)
{
	zo_fet_man_context con( this,zo_mvo_dump,var_id,0);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}

z_status zo_manipulator::feature_set_integer(zp_obj_base* p_obj,ctext fet_name,int val)
{
	ZTF;
	_member_var_int_data=val;
	zo_fet_man_context con( this,zo_mvo_set,fet_name,0);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
z_status zo_manipulator::feature_objlist_get(zp_obj_base* p_obj,ctext fet_name,z_obj_container** pp_list)
{
	ZTF;
	*pp_list=0;
	zo_fet_man_context con( this,zo_mvo_get,fet_name,0);
	z_status s=p_obj->feature_manipulate(&con);
	*pp_list=_member_obj_container;
	return s;
}
z_status zo_manipulator::feature_objlist_get_next(zp_obj_base* p_obj,ctext fet_name,zp_obj_base** pp_obj)
{
	ZTF;
	*pp_obj=0;
	zo_fet_man_context con( this,zo_mvo_get_next_from_list,fet_name,0);
	z_status s=p_obj->feature_manipulate(&con);
	*pp_obj=_p_member_var_obj;
	return s;
}
z_status zo_manipulator::feature_set_string(zp_obj_base* p_obj,ctext fet_name,ctext val,size_t len)
{
	ZTF;
	_member_var_string_data.assign(val,len);
	zo_fet_man_context con( this,zo_mvo_set,fet_name,0);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}

z_status zo_manipulator::feature_get_string(zp_obj_base* p_obj,ctext fet_name ,ctext& val)
{
	ZTF;
	_member_var_string_data="";
	zo_fet_man_context con( this,zo_mvo_get,fet_name,0);
	z_status s=p_obj->feature_manipulate(&con);
	if(!s)
		val=_member_var_string_data.c_str();
	return s;
}
z_status zo_manipulator::feature_objlist_add(zp_obj_base* p_obj,ctext fet_name,zp_obj_base* p_obj_child)
{
	ZTF;
	_p_member_var_obj=p_obj_child;
	zo_fet_man_context con( this,zo_mvo_set,fet_name,0);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}

z_status zo_manipulator::feature_execute(zp_obj_base* p_obj,zo_ftr_entry* fe)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_exec,zo_ftr_get_name(fe),fe);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
z_status zo_manipulator::feature_display(zp_obj_base* p_obj,zo_ftr_entry* fe)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_display,zo_ftr_get_name(fe),fe);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
ctext zo_manipulator::feature_get_as_string(zp_obj_base* p_obj,zo_ftr_entry* fe)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_get_to_string,zo_ftr_get_name(fe),fe);
	z_status s=p_obj->feature_manipulate(&con);
	if(s==zs_ok)
		return _member_var_string_data;
	return "[error]";
}
z_status zo_manipulator::feature_reset_iter(zp_obj_base* p_obj,ctext name)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_reset_iter,name,0);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
z_status zo_manipulator::feature_clear(zp_obj_base* p_obj,ctext name)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_clear,name,0);
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
z_status zo_manipulator::feature_set_from_value(zp_obj_base* p_obj,zp_value* p_value_obj,zo_ftr_entry* fe)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_set_from_value,zo_ftr_get_name(fe),fe);
	con._p_assignment_value=p_value_obj;
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
/*
z_status zo_manipulator::var_set_from_text(zp_obj_base* p_obj,ctext var_id,ctext txt)
{
	_member_var_string_data=txt;
	return var_set_from_string(p_obj,var_id);
}
z_status zo_manipulator::var_set_from_string(zp_obj_base* p_obj,ctext var_id)
{
	return p_obj->feature_manipulate(zo_mvo_set_from_string,var_id,this);
}
*/

void zo_manipulator::get_feature_map_by_fact(
		zo_feature_list& list,
		const z_obj_fact* fact,
		U32 feature_type,
		bool include_alias
		)
{

	zo_ftr_entry* fe=0;
	Z_ASSERT(fact);
	if(!fact) 
		return ;
	const z_obj_fact* fact_base=fact->base_fact;
	if(fact_base)
	{
		get_feature_map_by_fact(list,fact_base,feature_type,include_alias);
	}
	int i_var;
	for(i_var=0;i_var<fact->var_list_size;i_var++)
	{
		fe=&fact->var_list[i_var];
		if(fe->_type  & feature_type)
		{
			ctext name=zo_ftr_get_name(fe);
			list.add(name,fe);
			if(include_alias && fe->_short_id)
				list.add(fe->_short_id,fe);

		}
	}
}

zo_ftr_entry* zo_manipulator::get_next_feature(
		const z_obj_fact* fact,
		U32 feature_type,
		int &index
		)
{
	int local_index=index;
	zo_ftr_entry* fe=0;
	if(!fact) return 0;
	const z_obj_fact* fact_base=fact->base_fact;
	if((fact_base)&&(fact_base->var_list_size))
	{
		fe=get_next_feature(fact_base,feature_type,local_index);
		if(fe)
		{
			index=local_index;
			return fe;
		}
	}
	while(1)
	{
		if(local_index >= fact->var_list_size)
		{
			index=local_index-fact->var_list_size;
			return 0;
		}
		fe=&fact->var_list[local_index];
		local_index++;
		index++;
		if(fe->_type & feature_type)
		{
			index=local_index;
			return fe;

		}
	}
	return 0;
}


void zo_manipulator::dump_indent()
{
	
	int i=_dump_depth;
	while(i--)
		*_dump_fp<<"  ";
}

void zo_manipulator::dump_newline()
{
	*_dump_fp<<'\n';
	

}

z_status zo_manipulator::dump_obj(z_file* fp,zp_obj_base* obj)
{
	_dump_fp=fp;
	_dump_depth=0;
	dump_obj(obj);
	return zs_ok;

}


z_status zo_manipulator::dump_obj(zp_obj_base* obj)
{
	ZTF;
	if(!obj)
		return zs_fatal_error;

	*_dump_fp<< obj->get_type()<<"<";
	_dump_depth++;

	zo_ftr_entry* f;
	zo_feature_list list;
	int multi_line=feature_get_num_children(obj,0);
	bool needs_newline=true;

	obj->get_feature_map(this,list,ZO_MT_VAR,false);
	list.reset_iter();
	ctext key;

	while(f=list.get_next(key))
	{
		if(needs_newline&&multi_line)
		{
			dump_newline();
			dump_indent();
			needs_newline=false;
		}
		*_dump_fp<< key<<"=";
		feature_dump(obj,key);
		if(multi_line)
		{
			dump_newline();
			dump_indent();
		}
		else
			*_dump_fp<<' ';

	}
	*_dump_fp<<">";
	_dump_depth--;


	return zs_ok;
}

z_status zo_manipulator::load_obj(zp_obj_base* obj,zp_obj_parse* p)
{
	ZTF;
	if(!obj)
		return zs_fatal_error;
	if(!p)
		return zs_fatal_error;


	zo_ftr_entry* f;
	zo_feature_list list;
	obj->get_feature_map(this,list,ZO_MT_VAR,false);
	list.reset_iter();
	ctext key;
	while(f=list.get_next(key))
	{
		zp_obj_feature* parse_f=p->_fet_list.get(key);
		if(!parse_f)
		{
			//Z_ERROR("Could not find feature \"%s\"",key);
			//If a feature is not in the config file, it is not really an error
			continue;
		}
		feature_set_from_value(obj,&(parse_f->_val),f);
		
	}

	return zs_ok;
}



