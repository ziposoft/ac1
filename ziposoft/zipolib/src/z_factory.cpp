#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory_static.h"

#define RECAST(_TYPE_,_NAME_) _TYPE_& _NAME_= *reinterpret_cast<_TYPE_*>(v);
#define VF template <> void zf_var_funcs

void zf_var_funcs_base::dump(z_file& file, void* v) const
{
	z_string s;
	get(s,v);
	file<<s;
}

/*________________________________________________________________________

 zf_var_funcs<TYPE> defaults
________________________________________________________________________*/

template <class V> void zf_var_funcs<V>::dump(z_file& file, void* v) const {	zf_var_funcs_base::dump( file,  v) ; }
template <class V> void zf_var_funcs<V>::add(void* list,void* obj) const {}
template <class V> void* zf_var_funcs<V>::get_item(void* list,size_t index) const {	return 0;}
template <class V> size_t zf_var_funcs<V>::get_size(void* list) const{	return 0;}
template <class V> void* zf_var_funcs<V>::create_obj(void* list,const z_factory* fact) const{	return 0;}
template <class V> void zf_var_funcs<V>::get(z_string& s, void* v)	const{}
template <class V> void zf_var_funcs<V>::set(ctext s, void* v)	const{}
template <class V> void zf_var_funcs<V>::clear( void* v)	const{}


/*________________________________________________________________________

 zf_var_funcs<bool> 
________________________________________________________________________*/
VF<bool>::clear(void* v)            const {RECAST(bool,b); b=false;    }
VF<bool>::get(z_string& s, void* v) const {RECAST(bool,b); s=(b?"true":"false");  }
VF<bool>::set(ctext s, void* v)     const {RECAST(bool,b); b=(strcmp(s,"true")==0);    }
/*________________________________________________________________________

 zf_var_funcs<int> 
________________________________________________________________________*/
VF<int>::clear(void* v) const			{RECAST(int,i); i=0;    }
VF<int>::get(z_string& s, void* v) const	{RECAST(int,i); s=i;   }
VF<int>::set(ctext s, void* v) const		{RECAST(int,i); i=atoi(s);    }
/*________________________________________________________________________

zf_var_funcs<z_string> 
________________________________________________________________________*/
VF<z_string>::get(z_string& s, void* v) const{RECAST(z_string,str); s=str;    }
VF<z_string>::set(ctext s, void* v) const{RECAST(z_string,str); str=s;   }
VF<z_string>::clear(void* v) const{	RECAST(z_string,str); str="";}
/*________________________________________________________________________

zf_var_funcs<z_strlist> 
________________________________________________________________________*/
VF<z_strlist>::get(z_string& s, void* v)	const{	RECAST(z_strlist,list);	list.get_as_string(s);    }
VF<z_strlist>::clear(void* v)				const{	RECAST(z_strlist,list);	list.clear();}
VF<z_strlist>::set(ctext s, void* v)		const{	RECAST(z_strlist,list);	list <<  s; }
/*________________________________________________________________________

zf_var_funcs<zp_obj_vector> 
________________________________________________________________________*/
template <> void* zf_var_funcs<zp_obj_vector>::create_obj(void* v,const z_factory* fact) const
{
	RECAST(zp_obj_vector,list);
	zp_obj obj;
	obj._obj=fact->create_obj();
	obj._fact=fact;
	list.push_back(obj);

	return obj._obj;
}
template <> size_t zf_var_funcs<zp_obj_vector>::get_size(void* v) const
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
		list[i]._fact->dump_obj(file,list[i]._obj);
	}
}
/*________________________________________________________________________

zp_var_list_funcs<zp_obj_vector> 
________________________________________________________________________*/

void zp_var_list_funcs_base::dump(z_file& f, void* list) const 
{
	size_t count=get_size(list);
	if(!count)
	{
		f << "{}";
		return;
	}
	size_t i;
	f << "{\n";
	f.indent_inc();
	for(i=0;i<count;i++)
	{
		void* p=get_item(list,i);
		get_fact()->dump_obj(f,p);
	}
	f.indent_dec();
	f.indent();
	f << "}";
}

/*________________________________________________________________________

zf_var_funcs<z_obj_vector> 
________________________________________________________________________*/

#if 0
template <> template <class TYPE> void* zf_var_funcs<z_obj_vector<TYPE>>::create_obj(void* v,const z_factory* new_child_type) const
{
	RECAST(zp_obj_vector,list);
	zp_obj obj;
	obj._obj=new_child_type->create_obj();
	obj._fact=new_child_type;
	list.push_back(obj);

	return obj._obj;
}

#endif




template class zf_var_funcs<z_string>;
template class zf_var_funcs<int>;
template class zf_var_funcs<bool>;
template class zf_var_funcs<z_strlist>;
template class zf_var_funcs<zp_obj_vector>;

/*________________________________________________________________________

z_factory_static
________________________________________________________________________*/
const zf_static_var_entry* z_factory_static::get_var_entry (ctext name) const
{
	size_t i;
	const zf_static_var_entry* list=get_var_list();
	for(i=0;i<get_var_list_size();i++)
		if(strcmp(name,	list[i].name)==0)
			return &list[i];
	return 0;
}

z_status z_factory_static::create_child(void* obj,ctext var_name,const z_factory_static* new_child_type,void** ppChild) const
{
	const zf_static_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found;
	char* pvar=(char*)obj+ent->offset;
	const zf_var_funcs_base* funcs=ent->fp_var_func_get();

	void* newobj=funcs->create_obj(pvar,new_child_type);
	*ppChild=newobj;
	return z_status_success;

}
z_status z_factory_static::get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter) const
{
	const zf_static_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found;
	char* pvar=(char*)obj+ent->offset;
	const zf_var_funcs_base* funcs=ent->fp_var_func_get();

	*ppChild=funcs->get_ptr(pvar,iter);
	return z_status_success;

}

z_status z_factory_static::set_var_as_string(void* obj,ctext var_name,ctext value)	const
{
	const zf_static_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found; 
	const zf_var_funcs_base* funcs=ent->fp_var_func_get();
	char* pvar=(char*)obj+ent->offset;
	funcs->set(value,pvar);
	return z_status_success;
}

void z_factory_static::clear_all_vars (void* obj) const
{
	size_t i;
	const zf_static_var_entry* list=get_var_list();
	for(i=0;i<get_var_list_size();i++)
	{
		char* pvar=(char*)obj+list[i].offset;
		list[i].fp_var_func_get()->clear(pvar);
	}
}

z_status z_factory_static::get_var_as_string(void* obj,ctext var_name,z_string& value) const
{
	const zf_static_var_entry* ent= get_var_entry(var_name);
	if(!ent)
		return z_status_item_not_found; 
	const zf_var_funcs_base* funcs=ent->fp_var_func_get();
	char* pvar=(char*)obj+ent->offset;
	funcs->get(value,pvar);
	return z_status_success;
}

void z_factory_static::dump_obj(z_file& f,void* obj) const
{
	size_t i;
	z_string value;
	const zf_static_var_entry* list=get_var_list();
	f.indent();
	f << get_name()<<"{\n";
	f.indent_inc();
	for(i=0;i<get_var_list_size();i++)
	{
		char* pvar=(char*)obj+list[i].offset;
		f.indent();
		f << list[i].name<< "=";
		const zf_var_funcs_base* func=list[i].fp_var_func_get();
		func->dump(f,pvar);
		f <<'\n';
	}
	f.indent_dec();
	f.indent();
	f<< "}\n";

}
void z_factory_static::dump_static(z_file& f) const
{
	size_t i;
	z_string value;
	const zf_static_var_entry* list=get_var_list();
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


/*________________________________________________________________________

GLOBAL z_factory_static functions
________________________________________________________________________*/
const z_factory_static*  zfs_get_factory_by_name(ctext name,size_t len)
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
			const z_factory_static* fact=p_obj_entry.fact;
			if(strncmp(fact->get_name(),name,len)==0)
				return fact;
		}
	}
	return 0;
}

void*  zfs_create_obj_by_type(ctext name)
{
	const z_factory_static*  f=zfs_get_factory_by_name( name);
	if(f)
		return f->create_obj();
	return 0;


}
const z_factory_static*  zfs_get_factory(ctext name)
{
	int i_module;
	for(i_module=0;i_module<zp_module_master_list_size;i_module++)
	{
		const zp_module_entry* p_module=zp_module_master_list[i_module];
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const zp_module_fact_entry& p_obj_entry=p_module->facts[i_obj];
			const z_factory_static* fact=p_obj_entry.fact;

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
			const z_factory_static* fact=p_obj_entry.fact;
			gz_out.indent_reset();
			fact->dump_static(gz_out);

		}
	}

}
