#include "zipolib_cpp_pch.h"

#include "zipolib/include/zp_obj.h"

#define RECAST(_TYPE_,_NAME_) _TYPE_& _NAME_= *reinterpret_cast<_TYPE_*>(v);
#define VF template <> void zp_var_funcs

void zp_member_funcs_base::dump(z_file& file, void* v) const
{
	z_string s;
	get(s,v);
	file<<s;
}

/*________________________________________________________________________

 zp_var_funcs<TYPE> defaults
________________________________________________________________________*/

template <class V> void zp_var_funcs<V>::dump(z_file& file, void* v) const {	zp_member_funcs_base::dump( file,  v) ; }
template <class V> void zp_var_funcs<V>::add(void* list,void* obj) const {}
template <class V> void* zp_var_funcs<V>::get_item(void* list,size_t index) const {	return 0;}
template <class V> size_t zp_var_funcs<V>::get_size(void* list) const{	return 0;}
template <class V> void* zp_var_funcs<V>::create_obj(void* list,const zp_factory* new_child_type) const{	return 0;}
template <class V> void zp_var_funcs<V>::get(z_string& s, void* v)	const{}
template <class V> void zp_var_funcs<V>::set(ctext s, void* v)	const{}
template <class V> void zp_var_funcs<V>::clear( void* v)	const{}


/*________________________________________________________________________

 zp_var_funcs<bool> 
________________________________________________________________________*/
VF<bool>::clear(void* v)            const {RECAST(bool,b); b=false;    }
VF<bool>::get(z_string& s, void* v) const {RECAST(bool,b); s=(b?"true":"false");  }
VF<bool>::set(ctext s, void* v)     const {RECAST(bool,b); b=(strcmp(s,"true")==0);    }
/*________________________________________________________________________

 zp_var_funcs<int> 
________________________________________________________________________*/
VF<int>::clear(void* v) const			{RECAST(int,i); i=0;    }
VF<int>::get(z_string& s, void* v) const	{RECAST(int,i); s=i;   }
VF<int>::set(ctext s, void* v) const		{RECAST(int,i); i=atoi(s);    }
/*________________________________________________________________________

zp_var_funcs<z_string> 
________________________________________________________________________*/
VF<z_string>::get(z_string& s, void* v) const{RECAST(z_string,str); s=str;    }
VF<z_string>::set(ctext s, void* v) const{RECAST(z_string,str); str=s;   }
VF<z_string>::clear(void* v) const{	RECAST(z_string,str); str="";}
/*________________________________________________________________________

zp_var_funcs<z_strlist> 
________________________________________________________________________*/
VF<z_strlist>::get(z_string& s, void* v)	const{	RECAST(z_strlist,list);	list.get_as_string(s);    }
VF<z_strlist>::clear(void* v)				const{	RECAST(z_strlist,list);	list.clear();}
VF<z_strlist>::set(ctext s, void* v)		const{	RECAST(z_strlist,list);	list <<  s; }
/*________________________________________________________________________

zp_var_funcs<zp_obj_vector> 
________________________________________________________________________*/
template <> void* zp_var_funcs<zp_obj_vector>::create_obj(void* v,const zp_factory* new_child_type) const
{
	RECAST(zp_obj_vector,list);
	zp_obj obj;
	obj._obj=new_child_type->create_obj();
	obj._fact=new_child_type;
	list.push_back(obj);

	return obj._obj;
}
template <> size_t zp_var_funcs<zp_obj_vector>::get_size(void* v) const
{
	RECAST(zp_obj_vector,list);
	return list.size();
}

VF<zp_obj_vector>::dump(z_file& file, void* v) const
{
	RECAST(zp_obj_vector,list);
	size_t count=list.size();
	size_t i;
	for(i=0;i<count;i++)
	{
		list[i]._fact->dump_obj_r(file,list[i]._obj);
	}
}
/*________________________________________________________________________

zp_var_funcs<z_obj_vector> 
________________________________________________________________________*/

#if 0
template <> template <class TYPE> void* zp_var_funcs<z_obj_vector<TYPE>>::create_obj(void* v,const zp_factory* new_child_type) const
{
	RECAST(zp_obj_vector,list);
	zp_obj obj;
	obj._obj=new_child_type->create_obj();
	obj._fact=new_child_type;
	list.push_back(obj);

	return obj._obj;
}

#endif




template class zp_var_funcs<z_string>;
template class zp_var_funcs<int>;
template class zp_var_funcs<bool>;
template class zp_var_funcs<z_strlist>;
template class zp_var_funcs<zp_obj_vector>;


const zp_var_entry* zp_factory::get_var_entry (ctext name) const
{
	size_t i;
	const zp_var_entry* list=get_var_list();
	for(i=0;i<get_var_list_size();i++)
		if(strcmp(name,	list[i].name)==0)
			return &list[i];
	return 0;
}

z_status zp_factory::create_child(void* obj,ctext var_name,const zp_factory* new_child_type,void** ppChild) const
{
	const zp_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found;
	char* pvar=(char*)obj+ent->offset;
	const zp_member_funcs_base* funcs=ent->fp_var_func_get();

	*ppChild=funcs->create_obj(pvar,new_child_type);
	return z_status_success;

}
z_status zp_factory::get_memvar_ptr(void* obj,ctext var_name,void** ppChild,int* iter) const
{
	const zp_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found;
	char* pvar=(char*)obj+ent->offset;
	const zp_member_funcs_base* funcs=ent->fp_var_func_get();

	*ppChild=funcs->get_ptr(pvar,iter);
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

void zp_factory::dump_obj_r(z_file& f,void* obj) const
{
	size_t i;
	z_string value;
	const zp_var_entry* list=get_var_list();
	f.indent();
	f << get_name()<<"{\n";
	f.indent_inc();
	for(i=0;i<get_var_list_size();i++)
	{
		char* pvar=(char*)obj+list[i].offset;
		f.indent();
		f << list[i].name<< "=";
		list[i].fp_var_func_get()->dump(f,pvar);
		f <<'\n';
	}
	f.indent_dec();
	f.indent();
	f<< "}\n";

}
void zp_factory::dump_static_r(z_file& f) const
{
	size_t i;
	z_string value;
	const zp_var_entry* list=get_var_list();
	f.indent();
	f << get_name()<<"{\n";
	f.indent_inc();
	for(i=0;i<get_var_list_size();i++)
	{
		f.indent();
		f << list[i].name<<'\n';
	}
	f.indent_dec();
	f.indent();
	f<< "}\n";
}
void zp_factory::dump_obj (z_file& f,void* obj) const
{
	f.indent_reset();
	dump_obj_r(f,obj);

}


/*________________________________________________________________________

GLOBAL zp_factory functions
________________________________________________________________________*/
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
			const zp_factory* fact=p_obj_entry.fact;
			gz_out.indent_reset();
			fact->dump_static_r(gz_out);

		}
	}

}
