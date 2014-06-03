#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory.h"


z_obj_vector_map<z_factory>& get_factories_dynamic()
{
	static z_obj_vector_map<z_factory> g_factories_dynamic;
	return 	g_factories_dynamic;

}
z_factory_dyn& z_factory::get_dyn()
{
	if(!_dynamic)
		_dynamic=new z_factory_dyn();
	return *_dynamic;


}

zf_action* z_factory::add_act(ctext name,z_memptr act_addr,ctext desc)
{
	zf_action* action=z_new	zf_action(name,*(z_memptr*)&act_addr);
	get_dyn().features.add(action);
	return action;
}

zf_action* z_factory::add_act_params(ctext name,z_memptr act_addr,ctext desc,int num_params,...)
{
	int i;
	zf_action* action=add_act(name,act_addr,desc);
	va_list ArgList;
	va_start (ArgList, desc);
	for (i=0;i<num_params;i++)
	{
		zf_feature* p=va_arg(ArgList,zf_feature*);
		action->_params.add( p);
	}

	return action;
}

zf_feature* z_factory::add_prop(ctext name,zf_feature_type type,const zf_var_funcs_base* f,z_memptr offset,ctext desc)
{
	zf_feature* feat=z_new	zf_feature(name,type,f,offset,desc);
	get_dyn().features.add(feat);
	return feat;



}
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
zf_action::zf_action(ctext name,z_memptr offset,ctext desc) : zf_feature(name,zf_ft_act,0,offset,desc) 
{

}
void* zf_feature::get_var_ptr(void* obj,int* iter)
{
	char* pvar=(char*)obj+_offset;
	return df->get_ptr(pvar,iter);


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
/*
ctext z_factory::get_name() const
{
return _name;

}
z_factory::z_factory(ctext name) 
{
_name=name;
get_factories_dynamic().add(this);
}

z_status z_factory::get_var_info_i(size_t index,ctext& name,size_t &offset,const zf_var_funcs_base*& funcs)  const
{
zf_feature* f=_features[index];
if(!f)
return zs_item_not_found;
name=f->_name;
offset=f->_offset;
funcs=f->df;
return zs_ok;		

}
z_status z_factory::get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const
{
zf_feature* f=_features.get_by_name(name);
if(!f)
return zs_item_not_found;
offset=f->_offset;
funcs=f->df;
return zs_ok;		

}
*/