#include "z_parse_pch.h"

#include "zipolib_parse2/include/zp_obj.h"
#include "zipolib_parse2/include/z_obj_man.h"

#define RECAST(_TYPE_,_NAME_) _TYPE_& _NAME_= *reinterpret_cast<_TYPE_*>(v);
#define VF template <> void zp_var_funcs

void zp_member_funcs_base::dump(z_file& file, void* v,int& depth) const
{
	z_string s;
	get(s,v);
	file<<s;
}

template <class V> void zp_var_funcs<V>::dump(z_file& file, void* v,int& depth) const
{
	zp_member_funcs_base::dump( file,  v, depth) ;

}
template <class V> void zp_var_funcs<V>::add(void* list,void* obj) const
{
}
template <class V> void* zp_var_funcs<V>::get_item(void* list,size_t index) const
{
	return 0;
}
template <class V> size_t zp_var_funcs<V>::get_size(void* list) const
{
	return 0;
}
template <class V> void* zp_var_funcs<V>::reset_create_obj(void* list) const
{
	return 0;
}
template <class V> void zp_var_funcs<V>::get(z_string& s, void* v)	const
{
}
template <class V> void zp_var_funcs<V>::clear( void* v)	const
{
}
VF<bool>::clear(void* v)            const {RECAST(bool,b); b=false;    }
VF<bool>::get(z_string& s, void* v) const {RECAST(bool,b); s=(b?"true":"false");  }
VF<bool>::set(ctext s, void* v)     const {RECAST(bool,b); b=(strcmp(s,"true")==0);    }

VF<int>::clear(void* v) const			{RECAST(int,i); i=0;    }
VF<int>::get(z_string& s, void* v) const	{RECAST(int,i); s=i;   }
VF<int>::set(ctext s, void* v) const		{RECAST(int,i); i=atoi(s);    }


VF<z_string>::get(z_string& s, void* v) const{z_string* vs= reinterpret_cast<z_string*>(v); s=*vs;    }
VF<z_string>::set(ctext s, void* v) const{z_string* vs= reinterpret_cast<z_string*>(v); *vs=s;   }
VF<z_string>::clear(void* v) const
{
	z_string* vs= reinterpret_cast<z_string*>(v); 
	*vs="";   
}
 
VF<z_strlist>::get(z_string& s, void* v) const
{z_strlist* vs= reinterpret_cast<z_strlist*>(v);vs->get_as_string(s);    }
VF<z_strlist>::clear(void* v) const
{
	z_strlist* vs= reinterpret_cast<z_strlist*>(v); 
	vs->clear();
}
VF<z_strlist>::set(ctext s, void* v) const
{
	RECAST(z_strlist,list);
	list <<  s;   
}
VF<zp_obj_vector>::set(ctext s, void* v) const
{
	RECAST(zp_obj_vector,list);
	list.size();
}


template <> void* zp_var_funcs<zp_obj_vector>::reset_create_obj(void* v) const
{
	RECAST(zp_obj_vector,list);
	return v;
}
template <> size_t zp_var_funcs<zp_obj_vector>::get_size(void* v) const
{
	RECAST(zp_obj_vector,list);
	return list.size();
}

VF<zp_obj_vector>::dump(z_file& file, void* v,int& depth) const
{
	RECAST(zp_obj_vector,list);
	size_t count=list.size();
	int i;
	for(i=0;i<count;i++)
	{
		list[i]._fact->dump_obj_r(file,list[i]._obj,depth);
	}
}




template class zp_var_funcs<z_string>;
template class zp_var_funcs<int>;
template class zp_var_funcs<bool>;
template class zp_var_funcs<z_strlist>;
template class zp_var_funcs<zp_obj_vector>;

const zp_factory*  zo_get_factory_by_name(ctext name,size_t len)
{
	if(len==-1)
	{
		len=strlen(name);
	}
	int i_module;
	for(i_module=0;i_module<zp_module_master_list_size;i_module++)
	{
		const zp_module_entry* p_module=zp_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const zp_module_fact_entry& p_obj_entry=p_module->facts[i_obj];
			const zp_factory* fact=p_obj_entry.fact;
			if(strncmp(fact->get_name(),name,len)==0)
				return fact;
		}
	}
	return 0;
}
const zp_factory*  zo_get_factory(ctext name)
{
	int i_module;
	for(i_module=0;i_module<zp_module_master_list_size;i_module++)
	{
		const zp_module_entry* p_module=zp_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const zp_module_fact_entry& p_obj_entry=p_module->facts[i_obj];
			const zp_factory* fact=p_obj_entry.fact;

			if(z_str_same(name,fact->get_name()))
				return fact;
		}
	}
	return 0;
}


const zp_var_entry* zp_factory::get_var_entry (ctext name) const
{
	size_t i;
	const zp_var_entry* list=get_var_list();
	for(i=0;i<get_var_list_size();i++)
		if(strcmp(name,	list[i].name)==0)
			return &list[i];
	return 0;
}

z_status zp_factory::get_new_child_obj_ptr(void* obj,ctext var_name,void** ppChild) const
{
	const zp_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found;
	char* pvar=(char*)obj+ent->offset;
	const zp_member_funcs_base* funcs=ent->fp_var_func_get();

	*ppChild=funcs->reset_create_obj(pvar);
	return z_status_success;

}
z_status zp_factory::get_memvar_ptr(void* obj,ctext var_name,void** ppChild) const
{
	const zp_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found;
	char* pvar=(char*)obj+ent->offset;
	const zp_member_funcs_base* funcs=ent->fp_var_func_get();

	*ppChild=funcs->get_ptr(pvar);
	return z_status_success;

}

z_status zp_factory::set_var(void* obj,ctext var_name,ctext value)	const
{
	const zp_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found; 
	const zp_member_funcs_base* funcs=ent->fp_var_func_get();
	char* pvar=(char*)obj+ent->offset;
	funcs->set(value,pvar);
	return z_status_success;
}

void zp_factory::clear_all_vars (void* obj) const
{
	size_t i;
	const zp_var_entry* list=get_var_list();
	for(i=0;i<get_var_list_size();i++)
	{
		char* pvar=(char*)obj+list[i].offset;
		list[i].fp_var_func_get()->clear(pvar);
	}
}

z_status zp_factory::get_var_as_string(void* obj,ctext var_name,z_string& value) const
{
	const zp_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found; 
	const zp_member_funcs_base* funcs=ent->fp_var_func_get();
	char* pvar=(char*)obj+ent->offset;
	funcs->get(value,pvar);
	return z_status_success;
}
void dump_indent(z_file& f,int depth)
{
	while(depth--)
		f<<"  ";
}
void zp_factory::dump_obj_r(z_file& f,void* obj,int& depth) const
{
	size_t i;
	z_string value;
	const zp_var_entry* list=get_var_list();
	dump_indent(f,depth);
	f << get_name()<<"{\n";
	depth++;
	for(i=0;i<get_var_list_size();i++)
	{
		char* pvar=(char*)obj+list[i].offset;
		dump_indent(f,depth);
		f << list[i].name<< "=";
		list[i].fp_var_func_get()->dump(f,pvar,depth);
		f <<'\n';
	}
	depth--;
	dump_indent(f,depth);
	f<< "}\n";

}

void zp_factory::dump_obj (z_file& f,void* obj) const
{
	int depth=0;
	dump_obj_r(f,obj,depth);

}