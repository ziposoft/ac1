#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory_dyn.h"
z_obj_vector_map<z_factory_dyn> g_factories_dynamic;

zf_feature::zf_feature(ctext name,zf_var_funcs_base* funcs,size_t offset) 
{
	_name=name;
	df=funcs;
	_offset=offset;

}

ctext z_factory_dyn::get_name() const
{
	return _name;

}


z_status z_factory_dyn::get_var_info_i(size_t index,ctext& name,size_t &offset,const zf_var_funcs_base*& funcs)  const
{
	zf_feature* f=_features[index];
	if(!f)
		return z_status_item_not_found;
	name=f->_name;
	offset=f->_offset;
	funcs=f->df;
	return zs_ok;		
		
}
z_status z_factory_dyn::get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const
{
	zf_feature* f=_features.get_by_name(name);
	if(!f)
		return z_status_item_not_found;
	offset=f->_offset;
	funcs=f->df;
	return zs_ok;		
		
}
