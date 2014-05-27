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
 int z_factory::add_act(ctext name,size_t act_addr)
 {
	get_dyn().features.add(z_new	zf_feature(name,0,*(size_t*)(void*)&act_addr));
		return 0;

 }
 int z_factory::add_prop(ctext name,const zf_var_funcs_base* f,size_t offset)
 {
 	get_dyn().features.add(z_new	zf_feature(name,f,offset));
		return 0;



 }

zf_feature::zf_feature(ctext name,const zf_var_funcs_base* funcs,size_t offset) 
{
	_name=name;
	df=funcs;
	_offset=offset;

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
		return z_status_item_not_found;
	name=f->_name;
	offset=f->_offset;
	funcs=f->df;
	return zs_ok;		
		
}
z_status z_factory::get_var_info(ctext name,size_t &offset,const zf_var_funcs_base*& funcs) const
{
	zf_feature* f=_features.get_by_name(name);
	if(!f)
		return z_status_item_not_found;
	offset=f->_offset;
	funcs=f->df;
	return zs_ok;		
		
}
 */