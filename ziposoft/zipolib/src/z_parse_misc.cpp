#include "zipolib_cpp_pch.h"

#include "z_parse_internal.h"




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
void zp_parser::reset_results()
{
	if(!_results)
		_results=new zp_test_result();
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
	_output_obj_index=-1;
}

void zpi_context::init(zpi_context* parent,
					  const z_factory_static* ie, ctext parse_string	 )
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
//zp_parser
//________________________________________________________________


void zp_parser::context_set_root(void* p_obj,
									 const z_factory_static* ie,
									 ctext parse_string
									 )
{
	if(!parse_string)
	{
		parse_string=ie->get_parse_string();
	}
	
	_ctx_root.init(0,ie,parse_string);
	_ctx_current=&_ctx_root;
	_ctx_root._obj=p_obj;
	_ctx_root._output_obj_index=-1;
	//_ctx_root._output_result_index=0;

}
void zp_parser::context_sub_item_push(void* obj,const z_factory_static* ie)
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
	ctx->_output_obj_index=-1;
	//ctx->_output_result_index=0;
}

void zp_parser::context_sub_item_pop()
{
	if(_ctx_current->_parent)
	{
		_ctx_current=_ctx_current->_parent;
	}

}



//________________________________________________________________
//
//zp_parser
//________________________________________________________________


zp_parser::zp_parser()
{
//	_item_table=0;
//	_item_table_size=0;
	_results=0;
	_ctx_current=0;
	_last_status=zs_ok;
	_index_furthest=0;
}
zp_text_parser& zp_parser::context_get_current_template_parser()
{
	return _ctx_current->_current_template_parser;
}


z_status zp_parser::report_error()
{
	z_string data;
	printf("status=%s\n",z_status_get_text(_last_status));
	if(!_ctx_current) 
		return _last_status;
	if(_last_status==zs_ok)
		return zs_ok;
	if(_ctx_current->_obj_factory==0)
	{
		printf("No factory\n");

	}
	else
	printf("Error while parsing object type \"%s\"\n",_ctx_current->_obj_factory->get_name());
	zp_text_parser& tmpl=context_get_current_template_parser();
	//TODO it should report from the FARTHEST that we got
	//printf("template=\n%s\n",t.get_buffer());
	
	if(_last_status)
	{
		print_context();
		tmpl.print_context();
	}

	if(_last_status==zs_no_entry_for_item)
	{
		z_string match;
		tmpl.get_match(match);
		printf("No entry found for item \"%s\"",match.c_str());

	}
	if(_last_status==zs_syntax_error)
	{



	}
	return _last_status;
}




z_status zp_parser::parse_obj_f(void* p_obj,const z_factory_static* factory,ctext data)
{
	Z_ASSERT(p_obj);
	set_source(data,strlen(data));
	reset_results();
	factory->clear_all_vars(p_obj);
	context_set_root(p_obj,factory,0);
	_last_status=_process_template(zp_mode_parse_input);
	if(_last_status==zs_matched)
	{
		ZT("==========ITEM[%s] MATCHED, CREATING=====\n",factory->get_name());
		index_reset();
		context_get_current_template_parser().index_reset();
		reset_results();
		_last_status=_process_template(zp_mode_parse_create);
	}
	return _last_status;
}

z_status zp_parser::parse_item(void*& p_item,
									ctext item_entry_name
									)
{
	//ZTF;
	z_status status;
	const z_factory_static* ie=find_item(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	void* obj=ie->create_obj();
	context_set_root(obj,ie,0);
	reset_results();


	status=_process_template(zp_mode_parse_input);

	if(status==zs_matched)
	{
		ZT("==========ITEM[%s] MATCHED, CREATING=====\n",item_entry_name);
		index_reset();
		context_get_current_template_parser().index_reset();
		reset_results();

		status=_process_template(zp_mode_parse_create);

		p_item=obj;
	}
	return status;
}
z_status zp_parser::create_obj(ctext item_entry_name,void* &p_obj)
{

	//ZTF;
	const z_factory_static* ie=find_item(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	p_obj=ie->create_obj();


	return zs_ok;

}

z_status zp_parser::output_obj(z_file* fp,const z_factory_static* factory,void* obj)
{
	if(!factory)
		return -1;
	ZT("==========TEMPLATE[%s] OUTPUT OBJ=====\n",factory->get_name());

	_file_out=fp;
	//ZTF;
	context_set_root(obj,factory,0);
	zp_mode mode=zp_mode_output_obj;


	_last_status=_process_template(mode);

	//*fp<< "\n\n";

	return _last_status;

}




z_status zp_parser::create_empty_item(void*& p_item,
										   ctext item_entry_name
										   )
{
	//ZTF;
	z_status status;
	const z_factory_static* ie=find_item(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	void* obj=ie->create_obj();
	context_set_root(obj,ie,0);

	status=_process_template(zp_mode_create_empty);
	if(status==zs_matched)
		p_item=obj;

	return status;
}

z_status zp_parser::output_default_template(z_file* fp,ctext tmpl)
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


z_status zp_parser::parse_template(	ctext tmpl)
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
		Z_ASSERT(0); //void* obj=new void_generic(tmpl);
		Z_ASSERT(0); //context_set_root(obj,0,0);

		ZT("==========TEMPLATE[%s] MATCHED, CREATING=====\n",tmpl);
		index_reset();
		context_get_current_template_parser().index_reset();
		reset_results();
		status=_process_template(zp_mode_parse_create);
		Z_ASSERT(0); //p_item=obj;
	}
	return status;
	*/
}

//item table
const z_factory_static* zp_parser::find_item(ctext item_name,size_t len)
{
	if(len==-1)
	{
		len=strlen(item_name);
	}
	return zfs_get_factory_by_name(item_name,len);
}

