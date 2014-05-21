#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory_dyn.h"

ctext z_factory_dyn::get_name() const
{
	return _name;

}


z_status z_factory_dyn::get_var_info_i(size_t index,ctext& name,size_t &offset,const zf_var_funcs_base*& funcs)  const
{
	zf_feature* f=_features[index];
	if(!f)
		return z_status_item_not_found;
	name=f->name;
	offset=f->offset;
	funcs=f->df;
	return zs_ok;		
		
}
z_status z_factory_dyn::get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const
{
	zf_feature* f=_features.get_by_name(name);
	if(!f)
		return z_status_item_not_found;
	offset=f->offset;
	funcs=f->df;
	return zs_ok;		
		
}
