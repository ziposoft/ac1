
#include "z_parse_internal.h"


z_status z_zipex_base::report_error()
{
	z_logger_dump();
	printf("status=%s\n",zs_get_status_text(_last_status));
	if(_last_status==zs_ok)
		return zs_ok;
	zp_text_parser& tpl=tmpl();
	//TODO it should report from the FARTHEST that we got
	//printf("template=\n%s\n",t.get_buffer());
	
	if(_last_status)
	{
		//print_context();
		//tmpl.print_context();
		data().set_index(_furthest_index);
		data().print_context();
	}

	if(_last_status==zs_no_entry_for_item)
	{
		z_string match;
		tmpl().get_match(match);
		printf("No entry found for item \"%s\"",match.c_str());

	}
	if(_last_status==zs_syntax_error)
	{



	}
	return _last_status;
}

#if 0 

/*________________________________________________________________

zp_test_result
_________________________________________________________________*/
zp_test_result::zp_test_result()
{
}
U32 zp_test_result::add_result(U32 result)
{
	Z_ASSERT((0));

	return 0;
}
U32 zp_test_result::set_result(U32 index,U32 result)
{
	if(_test_results.size()<(index+5))
		_test_results.resize(index+5);
	if(result>zp_result_unknown)
	{
		_test_results_ex[index]=result;
		_test_results[index]=zp_result_extended_value;
		return 0;
	}

	if(_test_results.size()<(index+1))
		_test_results.resize(index+1);
	
	_test_results[index]=result;
	return 0;
}
U32 zp_test_result::get_result(U32 index)
{
	Z_ASSERT((_test_results.size()>index+4));
	if(_test_results.size()<=index+4)
		return 0;
	U32 result=_test_results[index];
	if(result==zp_result_extended_value)
	{
		result=_test_results_ex[index];
	}

	return result;
}
U32 zp_test_result::get_result_count()
{
	return (U32)(_test_results.size());
}


void zp_test_result::clear()
{
	return _test_results.resize(0);
}
void z_zipex_base::reset_results()
{
	if(!_results)
		_results=z_new zp_test_result();
	_test_result_current_index=0;
}


/*________________________________________________________________

zpi_context
_________________________________________________________________*/

zpi_context::zpi_context()
{
	_parent=0;
	_child=0;
	_obj_factory=0;
	_obj=0;
	_flags.as_u32=0;
}

void zpi_context::init(zpi_context* parent,
					  z_factory* ie, ctext parse_string	 )
{
	_parent=parent;
	_obj_factory=ie;
	_current_template_parser.set_source(parse_string);

}
/*
void* zpi_context::get_next_child_obj()
{
	if(!_obj)
		return 0;
	void* child=_obj->get_child(_obj_child_output_idx);
	return child;
}
*/

//________________________________________________________________
//
//z_zipex_base
//________________________________________________________________


void z_zipex_base::context_set_root(void* p_obj,
									 z_factory* ie,
									 ctext parse_string
									 )
{
	if(!parse_string)
	{
		parse_string=ie->get_parse_string();
	}
	Z_ASSERT(parse_string);
	_furthest_index=0;
	_ctx_root.init(0,ie,parse_string);
	_ctx_current=&_ctx_root;
	_ctx_root._obj=p_obj;
	_ctx_root._output_obj_iter.reset();
	//_ctx_root._output_result_index=0;

}
void z_zipex_base::context_sub_item_push(void* obj,z_factory* ie)
{
	//TODO need to change this so that we can remember where we failed
	//and give an accurate parse error message
	if(_ctx_current->_child==0)
	{
		_ctx_current->_child=z_new zpi_context();
	}
	zpi_context* ctx=_ctx_current->_child;
	ctx->_parent=_ctx_current;
	_ctx_current=ctx;
	ctx->_current_template_parser.set_source(ie->get_parse_string());
	ctx->_obj=obj;
	ctx->_obj_factory=ie;
	ctx->_output_obj_iter.reset();
	//ctx->_output_result_index=0;
}

void z_zipex_base::context_sub_item_pop()
{
	if(_ctx_current->_parent)
	{
		_ctx_current=_ctx_current->_parent;
	}

}



//________________________________________________________________
//
//z_zipex_base
//________________________________________________________________



zp_text_parser& z_zipex_base::tmpl()
{
	return _ctx_current->_current_template_parser;
}





z_status z_zipex_base::parse_obj_f(void* p_obj,z_factory* factory,ctext data,ctext tmpl)
{
	Z_ASSERT(p_obj);
	set_source(data,strlen(data));
	reset_results();
	factory->clear_all_vars(p_obj);
	context_set_root(p_obj,factory,tmpl);
	_last_status=_process_template(zp_mode_parse_input);
	if(_last_status==zs_matched)
	{
		ZT("==========ITEM[%s] MATCHED, CREATING=====\n",factory->get_name());
		index_reset();
		tmpl().index_reset();
		reset_results();
		_last_status=_process_template(zp_mode_parse_create);
	}
	return _last_status;
}

z_status z_zipex_base::parse_item(void*& p_item,
									ctext item_entry_name
									)
{
	//ZTF;
	z_status status;
	z_factory* ie=zf_get_factory(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	void* obj=ie->create_default_obj();
	context_set_root(obj,ie,0);
	reset_results();


	status=_process_template(zp_mode_parse_input);

	if(status==zs_matched)
	{
		ZT("==========ITEM[%s] MATCHED, CREATING=====\n",item_entry_name);
		index_reset();
		tmpl().index_reset();
		reset_results();

		status=_process_template(zp_mode_parse_create);

		p_item=obj;
	}
	return status;
}
z_status z_zipex_base::create_obj(ctext item_entry_name,void* &p_obj)
{

	//ZTF;
	z_factory* ie=zf_get_factory(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	p_obj=ie->create_default_obj();


	return zs_ok;

}

z_status z_zipex_base::output_obj(z_file* fp,z_factory* factory,void* obj)
{
	if(!factory)
		return zs_bad_parameter;
	ZT("==========TEMPLATE[%s] OUTPUT OBJ=====\n",factory->get_name());

	_file_out=fp;
	//ZTF;
	context_set_root(obj,factory,0);
	zp_mode mode=zp_mode_output_obj;


	_last_status=_process_template(mode);

	//*fp<< "\n\n";

	return _last_status;

}




z_status z_zipex_base::create_empty_item(void*& p_item,
										   ctext item_entry_name
										   )
{
	//ZTF;
	z_status status;
	z_factory* ie=zf_get_factory(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	void* obj=ie->create_default_obj();
	context_set_root(obj,ie,0);

	status=_process_template(zp_mode_create_empty);
	if(status==zs_matched)
		p_item=obj;

	return status;
}

z_status z_zipex_base::output_default_template(z_file* fp,ctext tmpl)
{
	z_status status;
	//_append_mode=false;
	ZT("==========TEMPLATE[%s] OUTPUT DEFAULT=====\n",tmpl);

	_file_out=fp;

	reset_results();


	context_set_root(0,0,tmpl);

	status=_process_template(zp_mode_output_default);
	if(status<zs_fatal_error)
		status=zs_ok;

	return status;
}


z_status z_zipex_base::parse_template(	ctext tmpl)
{
	z_status status;
	//_append_mode=false;

	//TODO this is UGLY!

	reset_results();


	context_set_root(0,0,tmpl);

	status=_process_template(zp_mode_parse_input);
	return status;
	/*
	if(status==zs_matched)
	{
		Z_ASSERT(0); //void* obj=z_new void_generic(tmpl);
		Z_ASSERT(0); //context_set_root(obj,0,0);

		ZT("==========TEMPLATE[%s] MATCHED, CREATING=====\n",tmpl);
		index_reset();
		tmpl().index_reset();
		reset_results();
		status=_process_template(zp_mode_parse_create);
		Z_ASSERT(0); //p_item=obj;
	}
	return status;
	*/
}

//item table
z_factory* z_zipex_base::find_item(ctext item_name,size_t len)
{
	z_string s;
	s.assign( item_name,len);

	return zf_get_factory(s.c_str());
}

#endif