#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory_dyn.h"
#include "zipolib/include/z_parse.h"

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
template <class V> void* zf_var_funcs<V>::create_obj(void* list,z_factory* fact) const{	return 0;}
template <class V> void zf_var_funcs<V>::get(z_string& s, void* v,int index)	const{}
template <class V> void zf_var_funcs<V>::set(ctext s, void* v,int index)	const{}
template <class V> void zf_var_funcs<V>::clear( void* v)	const{}
template <class V> z_status zf_var_funcs<V>::set_from_value(zp_value* val, void* var,int index)	const{  set(val->_string,var); return zs_ok;}

/*________________________________________________________________________

 zf_var_funcs<bool> 
________________________________________________________________________*/
VF<bool>::clear(void* v)            const {RECAST(bool,b); b=false;    }
VF<bool>::get(z_string& s, void* v,int index) const {RECAST(bool,b); s=(b?"true":"false");  }
VF<bool>::set(ctext s, void* v,int index)     const {RECAST(bool,b); b=(strcmp(s,"true")==0);    }
/*________________________________________________________________________

 zf_var_funcs<int> 
________________________________________________________________________*/
VF<int>::clear(void* v) const			{RECAST(int,i); i=0;    }
VF<int>::get(z_string& s, void* v,int index) const	{RECAST(int,i); s=i;   }
VF<int>::set(ctext s, void* v,int index) const		{RECAST(int,i); i=atoi(s);    }
/*________________________________________________________________________

zf_var_funcs<z_string> 
________________________________________________________________________*/
VF<z_string>::get(z_string& s, void* v,int index) const{RECAST(z_string,str); s=str;    }
VF<z_string>::set(ctext s, void* v,int index) const{RECAST(z_string,str); str=s;   }
VF<z_string>::clear(void* v) const{	RECAST(z_string,str); str="";}
VF<z_string>::dump(z_file& file, void* v) const{	RECAST(z_string,str);z_string out;z_str_escape(str,out);file <<out;}

/*________________________________________________________________________

zf_var_funcs<z_strlist> 
________________________________________________________________________*/
VF<z_strlist>::dump(z_file& file, void* v)	const{RECAST(z_strlist,list);
	size_t i;
	file<<'{';
	z_string s;
	for(i=0;i<list.size();i++)
	{
		if(i)
			file<<',';
		z_str_escape(list[i],s);
		file<<s;
	}

	file<<'}';
}


VF<z_strlist>::get(z_string& s, void* v,int index)	const
{	
	RECAST(z_strlist,list);	
	if(index==-1)
	{
		list.get_as_string(s);
		return;
	}
	if(index<(int)list.size()) 
		s=list[index];    
}
VF<z_strlist>::clear(void* v)				const{	RECAST(z_strlist,list);	list.clear();}
VF<z_strlist>::set(ctext s, void* v,int index)		const
{	
	RECAST(z_strlist,list);
	if(index==-1)
	{
		list<<s; //this is important for multi-stage items in  the parser. 
		return;
	}
	if(index<(int)list.size()) list[index]=s;  
}
template <> z_status zf_var_funcs<z_strlist>::set_from_value(zp_value* val, void* v,int index)		const{	RECAST(z_strlist,list);
	if(index==-1)
	{
		if(val->_string_list)
			list=val->_string_list->_list ;
		else
			list<<val->_string;//if we are passed a single string, then just append it.
		return zs_ok;
	}
	if(index<(int)list.size())
	{
		list[index]=val->_string;
		return zs_ok;
	}
	return zs_out_of_range;
	

}


/*________________________________________________________________________

zf_var_funcs<zp_obj_vector> 
________________________________________________________________________*/
template <> void* zf_var_funcs<zp_obj_vector>::create_obj(void* v,z_factory* fact) const
{
	RECAST(zp_obj_vector,list);
	zf_obj obj;
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
template <> z_status zf_var_funcs<zp_obj_vector>::set_from_value(zp_value* val, void* v,int index)		const{	
	RECAST(z_strlist,list);

	Z_ASSERT(0);//TODO
	return Z_ERROR(zs_operation_not_supported);
}
/*________________________________________________________________________

zp_var_list_funcs<zp_obj_vector> 
________________________________________________________________________*/

void zp_var_list_funcs_base::clear(void* v) const 
{

	z_obj_vector_base* plist=get_list(v);
	Z_ASSERT(plist);
	size_t count=plist->size();
	size_t i;
	for(i=0;i<count;i++)
	{
		void* p=plist->get_void(i);
		get_fact()->delete_obj(p);
	}
	plist->clear();

}
void* zp_var_list_funcs_base::get_ptr(void* v,int* iter ) const
{
	z_obj_vector_base* plist=get_list(v);
	if(!iter)
	{
		Z_ERROR_MSG(zs_bad_parameter,"Objects type does not match member variable");
		return 0;
	}
	if(*iter==-1)
		*iter=0;
	else 
	{
		(*iter)++;
		if(*iter>=(int)plist->size())
		{
			*iter=-1;
			return 0;
		}
	}
	return plist->get_void(*iter);
}
void zp_var_list_funcs_base::dump(z_file& f, void* v) const 
{
	z_obj_vector_base* plist=get_list(v);

	size_t count=plist->size();
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
		void* p= plist->get_void(i);
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
template <> template <class TYPE> void* zf_var_funcs<z_obj_vector<TYPE>>::create_obj(void* v,z_factory* new_child_type) const
{
	RECAST(zp_obj_vector,list);
	zf_obj obj;
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

z_factory
________________________________________________________________________*/

ctext z_factory::get_map_key() const
{
	return get_name();

}


z_status z_factory::get_var_ptr(void* obj,ctext var_name,void** ppChild,int* iter) const
{
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	z_status status=get_var_info(var_name,offset,funcs);
	if(status)
		return status;
	char* pvar=(char*)obj+offset;
	if(!funcs)
		return zs_operation_not_supported; //could be ACT

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
	funcs->set(value,pvar);
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
	funcs->get(value,pvar);
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

void z_factory::dump_obj(z_file& f,void* obj) const
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
		char* pvar=(char*)obj+offset;
		if(funcs)//only vars, not acts
		{
			f.indent();
			f << name;
			f << "=";
			funcs->dump(f,pvar);
			f <<'\n';
		}
		index++;
	}
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
const zf_var_entry* z_factory::get_var_entry (ctext name) const
{
	size_t i;
	const zf_var_entry* list=get_var_list();
	for(i=0;i<get_var_list_size();i++)
		if(strcmp(name,	list[i].name)==0)
		{
			return &list[i];
		}
	return 0;		

}
const zf_var_entry* z_factory::get_var_entry (size_t i) const
{
	if(i>=get_var_list_size())
		return 0;		
	return &get_var_list()[i];

}
z_status z_factory::get_list_features(z_strlist& list) const
{
	int index=0;
	ctext name;
	z_memptr offset;
	const zf_var_funcs_base* funcs;
	while(	get_var_info_i(index,name,offset,funcs)==zs_ok)
	{
		list<<name;
		index++;
	}
	return zs_ok;
}
z_status z_factory::get_var_info_i(size_t index,ctext& name,z_memptr &offset,
								   const zf_var_funcs_base*& funcs)  const
{
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
		index-=_dynamic->features.size();
	}
	ent=get_var_entry(index);
	if(!ent)
		return zs_item_not_found;
	name=ent->name;
	offset=ent->offset;
	funcs=0;
	if(ent->fp_var_func_get)
		funcs=ent->fp_var_func_get();
	return zs_ok;		
		
}

zf_feature* z_factory::get_feature(ctext name) 
{
	zf_feature* f;
	if(_dynamic)
	{
 		f=_dynamic->features.get_by_name(name);
		if(f)
		{
			return f;
		}
	}
	const zf_var_entry* ent=0;
	ent=get_var_entry(name);
	if(!ent)
		return 0;
	const zf_var_funcs_base* funcs=0;
	if(ent->fp_var_func_get) 
		funcs=ent->fp_var_func_get();
	f=add_prop(ent->name,ent->type,funcs,(z_memptr)ent->offset,"?");


	return f;		
		
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
	const zf_var_entry* ent=0;
	ent=get_var_entry(name);
	if(!ent)
		return zs_item_not_found;
	offset=ent->offset;
	if(ent->fp_var_func_get)
		funcs=ent->fp_var_func_get();
	else
		funcs=0;
	return zs_ok;		
		
}


//C:\Users\218015568\Documents\Visual Studio 2012\Templates\ProjectTemplates

/*________________________________________________________________________

GLOBAL z_factory functions
________________________________________________________________________*/
extern "C" {
extern const zp_module_entry *zp_module_master_list_default[]={0};
extern const  int zp_module_master_list_size_default=0;
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
		return f->create_obj();
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
			gz_out.indent_reset();
			fact->dump_static(gz_out);

		}
	}

}
