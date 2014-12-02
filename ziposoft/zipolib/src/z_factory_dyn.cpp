#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory.h"
#include "zipolib/include/z_parse.h"
#include "zipolib/include/z_parse_text.h"


void z_factory::add_features_recurse()
{
}


int z_factory::get_num_features() const
{
	int total=0;
	if(get_base_factory())
	{
		total=get_base_factory()->get_num_features();
	}
	total+=get_static_feature_count();
	total+=_dynamic->features.size();
	return total;
}


z_status z_factory::get_map_features(zf_feature_list&  list,
									 zf_feature_type desired_type,
									 zf_feature_flags oper)
{
	init_dynamic();
	size_t i=0;
	zf_feature* p_feature;

	for(i=0;i<_dynamic->features.size();i++)
	{
		p_feature=	   _dynamic->features[i];
		if(desired_type	!=zf_ft_all)
		{
			if(	desired_type!=	p_feature->get_type())
				continue;

		}
		if(!(p_feature->_flags & oper))
			continue;
		list<<p_feature;
	}
	z_factory* base=get_base_factory();
	if(base)
		return base->get_map_features(list,desired_type,oper);
	return zs_ok;
}
z_status z_factory::get_list_features(z_strlist& list,void* obj) 
{
	init_dynamic();
	size_t i=0;
	for(i=0;i<_dynamic->features.size();i++)
	{


		_dynamic->features[i]->add_to_list(list,obj);

	}
	z_factory* base=get_base_factory();
	if(base)
		return base->get_list_features(list,obj);
	return zs_ok;
}
z_status z_factory::load_obj_contents(zp_text_parser &parser,void* pObj,zf_feature_flags oper) 
{
	z_string s;
	init_dynamic();
	z_status status=zs_ok;
	zf_obj fobj(this,pObj);

	while(status==zs_ok)
	{
		parser.skip_ws();
		status=parser.test_any_identifier();
		if(status==zs_no_match)
			return zs_ok;
		if(status)
			break;
		parser.get_match(s);
		
		zf_feature *f=_dynamic->features.get_by_name(s);
		if(!f)
 		{
			return  Z_ERROR_MSG(zs_feature_not_found,"Feature \"%s\" in not found in class \"%s\"",s.c_str(),get_name());
			continue;
		}
		z_status status=parser.test_char('=');
		if(status)
			return Z_ERROR_MSG(status,"Expected '=' ");

		status=f->load(parser,fobj,oper);
	 
 		if(status)
		{
			if(zs_skipped!=status) 
				break;
		}
	}
	if(status==zs_eof)
		return zs_ok;
	return status;//Don't log error, should already be logged.
}
z_status zf_create_obj_from_text_stream_dyn(zp_text_parser &parser, z_factory* &factory,void* &objpointer,zf_feature_flags oper) 
{
	parser.skip_ws();
	z_status status=parser.test_any_identifier();
	if(status==zs_no_match)
		return status;
	if(status)
		return Z_ERROR_MSG(status,"Error loading obj type");
	z_string s;
	parser.get_match(s);
	factory=zf_get_factory(s);
	if(!factory)
		return Z_ERROR_MSG(zs_cannot_create_virtual_obj,"Unknown obj type \"%s\"",s.c_str());

	if(!(oper&ZFF_SKIP))
		if(objpointer==0)//we need to create it
		{
			objpointer=factory->create_default_obj();

		}
	parser.skip_ws();
	status=parser.test_char('{');
	if(status)
		return Z_ERROR_MSG(status,"Expected '{' ");


	status=factory->load_obj_contents(parser,objpointer,oper);
 	if(status)
		return status;

	parser.skip_ws();
	status=parser.test_char('}');
	if(status)
		return Z_ERROR_MSG(status,"Expected '}' ");



	return zs_ok;

}
