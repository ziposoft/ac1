#include "zipolib_cpp_pch.h"
#include "z_parse.h"
#include "z_parse_internal.h"


z_parser::z_parser()
{
	_p=z_new zp_parser();
}
z_parser::~z_parser()
{
	delete _p;
}
z_status z_parser::report_error()
{
	return _p->report_error();
}
z_status z_parser::output_default_template(z_file* fp,ctext tmpl)
{
	return _p->output_default_template(fp,tmpl);
}
z_status z_parser::output_obj(z_file* fp,z_factory* factory,void* obj)
{
	return _p->output_obj(fp,factory,obj);
}
z_status z_parser::parse_template(ctext tmpl,ctext data)
{
	_p->set_source(data);
	return _p->parse_template(tmpl);
}
z_status z_parser::parse_obj_f(void* p_obj,z_factory* factory,ctext data,ctext tmpl)
{
	return _p->parse_obj_f(p_obj,factory,data,tmpl);;
}