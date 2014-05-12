#include "zipolib_cpp_pch.h"
#include "z_ntf.h"

z_ntf_feature::z_ntf_feature(z_ntf_obj* parent,ctext name)
{
	_name=name;
	_parent=parent;


}

z_ntf_prop::z_ntf_prop(z_ntf_obj* parent,ctext name)  :z_ntf_feature(parent,name)
{


}
z_ntf_prop::~z_ntf_prop() 
{


}