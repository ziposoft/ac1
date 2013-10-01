#include "zipolib/include/z_trace.h"
#include "zipolib/include/z_file.h"
#include "zipolib/include/z_parse.h"

/*________________________________________________________________

zp_test_result
_________________________________________________________________*/
zp_test_result::zp_test_result()
{
}
U32 zp_test_result::add_result(U32 result)
{
	_test_results.push_back(result);
	return 0;
}
U32 zp_test_result::set_result(U32 index,U32 result)
{
	Z_ASSERT((result<=zp_result_unknown));
	if(_test_results.size()==index)
	{
		_test_results.push_back(result);
		return 0;
	}

	if(_test_results.size()<(index+1))
		_test_results.resize(index+1);
	
	_test_results[index]=result;
	return 0;
}
U32 zp_test_result::get_result(U32 index)
{
	Z_ASSERT((_test_results.size()>index));
	if(_test_results.size()>index)
		return _test_results[index];

	return 0;
}
U32 zp_test_result::get_result_count()
{
	return (U32)(_test_results.size());
}

/*

void zp_test_result::inc_index()
{
	_test_result_current_index++;
}
U32 zp_test_result::set_index(U32 index)
{
	Z_ASSERT((TEST_RESULT_MAX>index));
	_test_result_current_index=index;
	return index;
}

*/
void zp_test_result::clear()
{
	return _test_results.resize(0);
}
void zp_obj_parser::reset_results()
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
	_output_obj_index=0;
}

void zpi_context::init(zpi_context* parent,
					  const z_obj_fact* ie, ctext parse_string	 )
{
	_parent=parent;
	_obj_factory=ie;
	_current_template_parser.set_source(parse_string);

}
/*
zp_obj* zpi_context::get_next_child_obj()
{
	if(!_obj)
		return 0;
	zp_obj* child=_obj->get_child(_obj_child_output_idx);
	return child;
}
*/

//________________________________________________________________
//
//zp_obj_parser
//________________________________________________________________


void zp_obj_parser::context_set_root(zp_obj* p_obj,
									 const z_obj_fact* ie,
									 ctext parse_string
									 )
{
	if(!parse_string)
	{
		if(p_obj)
			parse_string=p_obj->get_parse_string();
		else
			parse_string=ie->parse_string;
	}
	if(!ie)
		ie=p_obj->get_fact();
	_ctx_root.init(0,ie,parse_string);
	_ctx_current=&_ctx_root;
	_ctx_root._obj=p_obj;
	_ctx_root._output_obj_index=0;
	//_ctx_root._output_result_index=0;

}
void zp_obj_parser::context_sub_item_push(zp_obj* obj,const z_obj_fact* ie)
{
	if(_ctx_current->_child==0)
	{
		_ctx_current->_child=new zpi_context();
	}
	zpi_context* ctx=_ctx_current->_child;
	ctx->_parent=_ctx_current;
	_ctx_current=ctx;
	ctx->_current_template_parser.set_source(ie->parse_string);
	ctx->_obj=obj;
	ctx->_obj_factory=ie;
	ctx->_output_obj_index=0;
	//ctx->_output_result_index=0;
}

void zp_obj_parser::context_sub_item_pop()
{
	if(_ctx_current->_parent)
	{
		_ctx_current=_ctx_current->_parent;
	}

}



//________________________________________________________________
//
//zp_obj_parser
//________________________________________________________________

/*
zp_obj_parser::zp_obj_parser(const z_obj_fact** it,size_t size)
{
	set_obj_table(it,size);
	_results=0;
	_ctx_current=0;
}

void zp_obj_parser::set_obj_table(const z_obj_fact** it,size_t size)
{
	_item_table=it;
	_item_table_size=size;
}
*/
zp_obj_parser::zp_obj_parser()
{
//	_item_table=0;
//	_item_table_size=0;
	_results=0;
	_ctx_current=0;
}
zp_text_parser& zp_obj_parser::context_get_current_template_parser()
{
	return _ctx_current->_current_template_parser;
}


z_status zp_obj_parser::report_error(z_status status)
{
	z_string data;
	printf("status=%s\n",z_status_get_text(status));
	if(!_ctx_current) 
		return status;
	if(status==zs_ok)
		return zs_ok;
	Z_ASSERT(_ctx_current->_obj_factory);
	printf("Error while parsing object type \"%s\"\n",z_obj_fact_get_name(_ctx_current->_obj_factory));
	zp_text_parser& tmpl=context_get_current_template_parser();
	//printf("template=\n%s\n",t.get_buffer());
	
	if(status)
	{
		print_context();
		tmpl.print_context();
	}

	if(status==zs_no_entry_for_item)
	{
		z_string match;
		tmpl.get_match(match);
		printf("No entry found for item \"%s\"",match.c_str());

	}
	if(status==zs_syntax_error)
	{



	}
	return status;
}



zp_obj* zp_obj_parser::add_new_child_item(zp_obj*& parent,
										  ctext type,ctext data
										  )
{
	///ZTF;
	z_status status;

	zp_obj* item=0;

	status=create_empty_item(item,type);

	if(status==zs_ok)
		parent->add_child(item);

	return item;
}
z_status zp_obj_parser::parse_obj(zp_obj* p_obj,ctext data)
{
	z_status status;
	Z_ASSERT(p_obj);
	set_source(data,strlen(data));
	reset_results();
	feature_clear(p_obj);
	context_set_root(p_obj,p_obj->get_fact(),0);
	status=_process_template(zp_mode_parse_input);
	if(status==zs_matched)
	{
		ZT(("==========ITEM[%s] MATCHED, CREATING=====\n",p_obj->get_name()));
		index_reset();
		context_get_current_template_parser().index_reset();
		reset_results();
		status=_process_template(zp_mode_parse_create);
	}
	return status;
}
z_status zp_obj_parser::parse_obj(zp_obj* p_obj,z_string& data_in)
{

	return parse_obj(p_obj,data_in.c_str());

}

z_status zp_obj_parser::parse_item(zp_obj*& p_item,
									ctext item_entry_name
									)
{
	//ZTF;
	z_status status;
	const z_obj_fact* ie=find_item(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	zp_obj* obj=create_new_obj(ie);
	context_set_root(obj,ie,0);
	reset_results();


	status=_process_template(zp_mode_parse_input);

	if(status==zs_matched)
	{
		ZT(("==========ITEM[%s] MATCHED, CREATING=====\n",item_entry_name));
		index_reset();
		context_get_current_template_parser().index_reset();
		reset_results();

		status=_process_template(zp_mode_parse_create);

		p_item=obj;
	}
	return status;
}
z_status zp_obj_parser::create_obj(ctext item_entry_name,zp_obj* &p_obj)
{

	//ZTF;
	const z_obj_fact* ie=find_item(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	p_obj=create_new_obj(ie);


	return zs_ok;

}
z_status zp_obj_parser::output_obj(z_file* fp,zp_obj* obj)
{
	ZT(("==========TEMPLATE[%s] OUTPUT OBJ=====\n",obj->get_name()));

	_file_out=fp;
	//ZTF;
	z_status status;
	context_set_root(obj,obj->get_fact(),0);
	zp_mode mode=zp_mode_output_obj;

	feature_reset_iter(obj);

	status=_process_template(mode);

	//*fp<< "\n\n";

	return status;

}



z_status zp_obj_parser::create_empty_item(zp_obj*& p_item,
										   ctext item_entry_name
										   )
{
	//ZTF;
	z_status status;
	const z_obj_fact* ie=find_item(item_entry_name);
	if(ie==0)
		return zs_no_entry_for_item;
	zp_obj* obj=create_new_obj(ie);
	context_set_root(obj,ie,0);

	status=_process_template(zp_mode_create_empty);
	if(status==zs_matched)
		p_item=obj;

	return status;
}

z_status zp_obj_parser::output_default_template(z_file* fp,ctext tmpl)
{
	z_status status;
	//_append_mode=false;
	ZT(("==========TEMPLATE[%s] OUTPUT DEFAULT=====\n",tmpl));

	_file_out=fp;

	reset_results();


	context_set_root(0,&zp_obj_generic::FACT,tmpl);

	status=_process_template(zp_mode_output_default);
	if(status<zs_fatal_error)
		status=zs_ok;

	return status;
}


z_status zp_obj_parser::parse_template(zp_obj*& p_item,
										ctext tmpl)
{
	z_status status;
	//_append_mode=false;

	//TODO this is UGLY!

	reset_results();


	context_set_root(0,&zp_obj_generic::FACT,tmpl);

	status=_process_template(zp_mode_parse_input);
	if(status==zs_matched)
	{
		zp_obj* obj=new zp_obj_generic(tmpl);
		context_set_root(obj,0,0);

		ZT(("==========TEMPLATE[%s] MATCHED, CREATING=====\n",tmpl));
		index_reset();
		context_get_current_template_parser().index_reset();
		reset_results();
		status=_process_template(zp_mode_parse_create);
		p_item=obj;
	}
	return status;
}

//item table
const z_obj_fact* zp_obj_parser::find_item(ctext item_name,size_t len)
{
	if(len==-1)
	{
		len=strlen(item_name);
	}
	return zo_get_factory_by_name_and_length(item_name,len);
}

#if 0
z_status zp_obj_parser::access_obj_member_map(type_memvar_oper oper,int* pindex,z_obj_map* member_var)
{
	if(oper==mvo_clear)//clear
	{
		member_var->clear();
		return zs_ok;
	}
	if(oper==mvo_get_size_reset)//get list size
	{
		*pindex=(int)member_var->size();
		return zs_ok;
	}
	if(oper==zo_mvo_set) //add an object
	{
		member_var->add(_p_member_var_obj->get_name(),_p_member_var_obj);
		return zs_ok;
	}
	if(oper==zo_mvo_get)//get an object
	{
		int index=0;
		if(pindex)
			index=*pindex;

		if(index>=(int)member_var->size())	
			return zs_end_of_list;
		//TODO!!
		/*
		_p_member_var_obj=(zp_obj*)(*member_var)[index];
		*/
		return zs_ok;
	}
	return zs_internal_error;
}
#endif


zp_obj* zp_obj_parser::create_new_obj(const z_obj_fact* ie)
{
	zp_obj* sub_obj=0;
	if(ie->create_func)
	{
		sub_obj=(zp_obj*)ie->create_func();
	}
	else
		sub_obj=new zp_obj();
	return sub_obj;
}

