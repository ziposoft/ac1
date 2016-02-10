#include "zipolib_pch.h"

#include "zipolib/include/z_factory_controller.h"
#include "zipolib/include/z_factory_var_funcs.h"
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_logger.h"


z_status zf_feature_node::append_to_path(z_string &path)
{
	Z_ASSERT(_f);
	path<< _f->get_name();
	if(_index)
	{
	  path<<'['<<_index<<']';
	}

	return zs_ok;
}

z_status z_fact_obj_path::get_path_string(z_string& s)
{
	size_t i;
	s="/";
	for(i=0;i<size();i++)
	{
		if(i) 
			s<<'/';
		at(i).append_to_path(s);
	}
	return zs_ok;
};
z_status z_fact_obj_path::up(zf_obj& new_selected)
{
	z_status status;	
	if(size())
		pop_back();

	size_t i;
 	for(i=0;i<size();i++)
	{
		zf_feature* f=at(i)._f;
		status= f->get_zf_obj(new_selected,at(i)._index,new_selected);
		if(status)
			return status;
	}
	return zs_ok;
}

z_status z_factory_controller::select_obj_from_path(z_string& path)
{
	
	zf_obj selected=_selected;
	z_fact_obj_path obj_path;
	z_string feature;
	z_string feature_index;
	z_status status=EvaluatePath(path,selected,obj_path,feature,feature_index);
	if(status==zs_ok)
	{
		_selected=selected;
		_obj_path=obj_path;
	

	}
	return status;
}

z_status z_factory_controller::path_up()
{
	zf_obj new_selected=get_root();
	z_status status=_obj_path.up(new_selected);
	if(status)
		return status;
	_selected= new_selected;
	return zs_ok;
}

z_status z_factory_controller::parse_feature_and_index(z_string& out_feature,z_string& out_index)
{
	z_status status;	
	status=_tparser.test_any_identifier();

	if(status) 
	{
		if(status==zs_no_match)
		{
			char c=_tparser.get_char_under_test();
			return Z_ERROR_MSG(zs_parse_error,"Unexpected '%c'",c);
		}
		return status;//End of buffer
	}
	_tparser.get_match(out_feature);
	if(_tparser.test_char('[')==zs_ok)
	{
		status=_tparser.test_any_identifier();
		_tparser.get_match(out_index);
		if(_tparser.test_char(']'))
		{
			return Z_ERROR_MSG(zs_parse_error,"Expecting ']'");
		}
	}


	return zs_ok;
}

z_status z_factory_controller:: EvaluatePath(ctext text_path,
											 zf_obj& selected,
											 z_fact_obj_path& out_obj_path,
											 z_string & out_feature,
											 z_string& out_feature_index)
{
	ZT("%s",text_path);
	z_status status=zs_ok;	
	out_feature.clear();
	out_feature_index.clear();
	_tparser.set_source(text_path);

	if(_tparser.test_char('/')==zs_matched)
	{
		selected=_root;
		out_obj_path.clear();
	}
	while (1)
	{
		//This just gets next word in path
		status=parse_feature_and_index(out_feature,out_feature_index);
		if(status)
		{
			return status;
		}
		zf_feature *f;
		f=selected._fact->get_feature(out_feature);
		if(!f)
		{
			 /*
			 not our feature. could be a console feature.
			 all we care about here is that it is not an
			 object. deal with unknown features later*/
			return zs_unparsed_data; //
		}
		status= f->get_zf_obj(selected,out_feature_index,selected);
		if(status)
		{
			/*It is our feature, but it is not an object
			so we are done. 
			*/
			return zs_unparsed_data; 
		}
		out_obj_path.emplace_back(f,out_feature_index);


		if(_tparser.eob())
			return zs_ok;//we are done!

		if(_tparser.test_char('/') && _tparser.test_char('.'))
		{
			//if it is not a path char, stop
			return Z_ERROR_MSG(zs_parse_error,"Expected '.' or '/'\n");
		}
		out_feature.clear();
		out_feature_index.clear();
	}
	/*
	if(!_tparser.eob())
		return Z_ERROR_DBG(zs_unparsed_data);
		*/
	//should never get here	
	return status;
}