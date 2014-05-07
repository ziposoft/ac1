#include "z_parse_pch.h"
#include "z_obj_man.h"
#include "z_parse_obj.h"


zo_manipulator::zo_manipulator()
{
	_dump_depth=0;
	_dump_new_line=false;
}


  
int zo_manipulator::feature_get_num_children(void* p_obj,ctext var_id /* 0 for all */)
{
	return _member_var_int_data;
}

z_status zo_manipulator::feature_dump(void* p_obj,ctext var_id)
{
	Z_ASSERT(0);
	return -1;
}

z_status zo_manipulator::feature_set_integer(void* p_obj,ctext fet_name,int val)
{
	Z_ASSERT(0);
	return -1;
}
z_status zo_manipulator::feature_objlist_get(void* p_obj,ctext fet_name,void** pp_list)
{
	ZTF;
	Z_ASSERT(0);
	return -1;
}
z_status zo_manipulator::feature_objlist_get_next(void* p_obj,ctext fet_name,void** pp_obj)
{
	ZTF;
	Z_ASSERT(0);
	return -1;
}

z_status zo_manipulator::feature_objlist_add(void* p_obj,ctext fet_name,void* p_obj_child)
{
	ZTF;
	Z_ASSERT(0);
	return -1;
}

z_status zo_manipulator::feature_execute(void* p_obj,zp_var_entry* fe)
{
	ZTF;
	Z_ASSERT(0);
	return -1;
}
z_status zo_manipulator::feature_display(void* p_obj,zp_var_entry* fe)
{
	ZTF;
	Z_ASSERT(0);
	return -1;
}
ctext zo_manipulator::feature_get_as_string(void* p_obj,zp_var_entry* fe)
{
	ZTF;
	Z_ASSERT(0);
	return "[error]";
}
z_status zo_manipulator::feature_clear(void* p_obj,ctext name)
{
	ZTF;
	Z_ASSERT(0);
	return -1;
}
/*
z_status zo_manipulator::feature_set_from_value(void* p_obj,zp_value* p_value_obj,zp_var_entry* fe)
{
	ZTF;
	zo_fet_man_context con( this,zo_mvo_set_from_value,zo_ftr_get_name(fe),fe);
	con._p_assignment_value=p_value_obj;
	z_status s=p_obj->feature_manipulate(&con);
	return s;
}
*/
/*
z_status zo_manipulator::var_set_from_text(void* p_obj,ctext var_id,ctext txt)
{
	_member_var_string_data=txt;
	return var_set_from_string(p_obj,var_id);
}
z_status zo_manipulator::var_set_from_string(void* p_obj,ctext var_id)
{
	return p_obj->feature_manipulate(zo_mvo_set_from_string,var_id,this);
}


void zo_manipulator::get_feature_map_by_fact(
		zo_feature_list& list,
		const zp_factory* fact,
		U32 feature_type,
		bool include_alias
		)
{

	zp_var_entry* fe=0;
	Z_ASSERT(fact);
	if(!fact) 
		return ;
	const zp_factory* fact_base=fact->base_fact;
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

zp_var_entry* zo_manipulator::get_next_feature(
		const zp_factory* fact,
		U32 feature_type,
		int &index
		)
{
	int local_index=index;
	zp_var_entry* fe=0;
	if(!fact) return 0;
	const zp_factory* fact_base=fact->base_fact;
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
*/

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
 /*
z_status zo_manipulator::dump_obj(z_file* fp,void* obj)
{
	_dump_fp=fp;
	_dump_depth=0;
	dump_obj(obj);
	return zs_ok;

}


z_status zo_manipulator::dump_obj(void* obj)
{
	ZTF;
	if(!obj)
		return zs_fatal_error;

	*_dump_fp<< obj->get_type()<<"<";
	_dump_depth++;

	zp_var_entry* f;
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

z_status zo_manipulator::load_obj(void* obj,void_parse* p)
{
	ZTF;
	if(!obj)
		return zs_fatal_error;
	if(!p)
		return zs_fatal_error;


	zp_var_entry* f;
	zo_feature_list list;
	obj->get_feature_map(this,list,ZO_MT_VAR,false);
	list.reset_iter();
	ctext key;
	while(f=list.get_next(key))
	{
		void_feature* parse_f=p->_fet_list.get(key);
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
			   */


