#include "zipolib/include/z_trace.h"
#include "zipolib/include/z_file.h"
#include "zipolib/include/z_parse.h"





#define FPT(_X_) &zp_text_parser::_X_
#define FPO(_X_) &zp_obj_parser::_X_

keyword_item keyword_list[]={
	{item_literal,		"literal",	"string literal",		
			FPT(ft_single_quoted_string),0,			
			FPO(_f_test_string_literal),
			FPO(_f_string_literal_create),
			FPO(_f_string_literal_output)},
	{item_integer,		"int",		"integer",				
			FPT(ft_test_identifier),(void*)"int",	
			FPO(ft_digits),
			FPO(_f_create_string),
			FPO(_f_output_string)},
	{item_quoted_double,	"string",	"quoted_single",	
			FPT(ft_test_identifier),"string",		
			FPO(_f_quoted_string_test),
			FPO(_f_create_string),
			FPO(_f_output_string)},
	{item_quoted_single,	"string_sq",	"quoted_single",
			FPT(ft_test_identifier),"string_sq",	
			FPO(_f_squoted_string_test),
			FPO(_f_create_string),
			FPO(_f_output_string)},
	{item_filepath,"item_filepath",	"identifier",			FPT(ft_test_identifier),"filepath",0,	FPO(_f_create_string),FPO(_f_output_string)},
	{item_identifier,"ident",	"identifier",				FPT(ft_test_identifier),"ident",		FPO(_f_test_ident),FPO(_f_create_string),FPO(_f_output_string)},
	{item_identifier,"identlist",	"identifier",			FPT(ft_test_identifier),"identlist",	FPO(_f_ident_list_test),FPO(_f_ident_list_create),FPO(_f_ident_list_output)},
	{item_scoped,	"scoped",	"scoped",					FPT(ft_test_identifier),"scoped",		FPO(ft_scoped_identchars),FPO(_f_create_string),FPO(_f_output_string)},
	{item_toeol,	"toeob",	"Find EOB",					FPT(ft_test_identifier),"toeol",		FPO(_f_test_to_eob),FPO(_f_create_string),FPO(_f_output_string)},
	{item_whsp,	"whsp",	"White space",						FPT(ft_test_identifier),"whsp",			FPO(_f_test_whsp),FPO(_f_create_string),FPO(_f_output_string)},
	{item_whsp,	"path",	"File path",						FPT(ft_test_identifier),"path",			FPO(_f_test_path),FPO(_f_create_string),FPO(_f_output_string)},
	//{item_sub_obj,	"sub_obj",	"sub object",	FPT(ft_any_identifier),"",FPO(_f_test_sub_obj),0,FPO(_f_output_sub_obj)}
	{item_literal,	"not string",	"string literal",
	FPT(test_not_single_quoted_string),0,
	FPO(_f_not_test_string_literal),0,FPO(_f_output_string)},

};
#define keyword_list_count (sizeof(keyword_list)/sizeof(keyword_item))
z_status zp_obj_parser::_f_ident_list_test(const void* dummy)
{
	z_status result; 
	z_status result_total=zs_no_match; 


	while(1)
	{
		result= test_any_identifier();
		if((result==zs_no_match)||(result==zs_eof))
			return result_total;
		if(result)
			return result;
		result_total=zs_matched;
		if(_ctx_current->_mode.create)
			if((_ctx_current->_flags.parent_data)&&(_ctx_current->_obj))
			{
				ctext match;
				size_t match_len;
				get_match(match,match_len);
				z_status i_result=feature_set_string(
					_ctx_current->_obj,
					_ctx_current->_member_var_name,match,match_len);

				if(i_result)
					return result;
			}

		result=test_char(',');
		if((result==zs_no_match)||(result==zs_eof))
			return result_total;
		if(result)
			return result;
	}
}

z_status zp_obj_parser::_f_ident_list_create(zp_flags flags,int type)
{
	return zs_matched;

}
z_status zp_obj_parser::_f_ident_list_output(zp_flags flags,zp_mode mode)
{
	z_status result=zs_ok;
	bool need_comma=false;

	if((_ctx_current->_obj)&&(flags.parent_data))
	{
		feature_reset_iter(_ctx_current->_obj,_ctx_current->_member_var_name);
		while(result==zs_ok)
		{
			ctext s;
			result=feature_get_string(_ctx_current->_obj,_ctx_current->_member_var_name,s);
			if(result==zs_ok)
			{
				if(need_comma)
					*_file_out<<',';

				*_file_out<<s;
				need_comma=true;
			}
		}
	}
	if(result!=zs_end_of_list)
		return result;
	return zs_matched;
}
z_status zp_obj_parser::_f_squoted_string_test(const void* dummy)
{
	return test_single_quoted_string();
}
z_status zp_obj_parser::_f_quoted_string_test(const void* dummy)
{
	return test_code_string();
}
z_status zp_obj_parser::_f_test_ident(const void* dummy)
{
	return test_any_identifier();
}
z_status zp_obj_parser::_f_test_whsp(const void* dummy)
{
	return test_cset(*cset_white_space);
}
z_status zp_obj_parser::_f_test_path(const void* dummy)
{
	return test_cset(*cset_path_string);

}
z_status zp_obj_parser::_f_test_to_eob(const void* dummy)
{
	return test_to_eob();
}

z_status zp_obj_parser::_f_create_string(zp_flags flags,int type)
{
	z_status status=zs_matched;
	ctext match_start=0;
	size_t match_len=0;

	get_match(match_start,match_len);
	if(match_start)
	{
		if(flags.parent_data)
		{
			if(_ctx_current->_obj)
			{
				status=feature_set_string(_ctx_current->_obj,_ctx_current->_member_var_name,match_start,match_len);
			}
		}
		else
		{
			zp_text* item=0;
			item=new zp_text();
			item->_type=type;
			_ctx_current->_obj->add_child(item);
			item->set_text(match_start,match_len);
			item->_templ_offset=
				context_get_current_template_parser().get_index_offset();
		}
	}
	return status;
}

z_status zp_obj_parser::_f_test_string_literal(const void* dummy)
{
	z_status status=zs_no_match;
	ctext match_start=0;
	size_t match_len=0;
	zp_text_parser& tmpl=context_get_current_template_parser();
	tmpl.get_match(match_start,match_len);
	status= test_string(match_start,match_len);
	return status;
}
z_status zp_obj_parser::_f_not_test_string_literal(const void* dummy)
{
	//TODO!!!
	z_status status=zs_no_match;
	ctext match_start=0;
	size_t match_len=0;
	zp_text_parser& tmpl=context_get_current_template_parser();
	tmpl.get_match(match_start,match_len);
	status= test_string(match_start,match_len);
	return status;
}

z_status zp_obj_parser::_f_string_literal_create(zp_flags flags,int type)
{
	z_status status=zs_ok;
	if(flags.parent_data)
	{
		if(_ctx_current->_obj)
		{
			feature_set_integer(_ctx_current->_obj,_ctx_current->_member_var_name,1);
		}
	}
	return status;
}
z_status zp_obj_parser::_f_string_literal_output(zp_flags flags,zp_mode mode)
{
	z_status status=zs_ok;
	ctext match_start=0;
	size_t match_len=0;
	if(!(flags.required || flags.create_default))
		return zs_no_match;
	zp_text_parser& tmpl=context_get_current_template_parser();
	tmpl.get_match(match_start,match_len);
	_file_out->write(match_start,match_len);
#if DEBUG
	z_string dbgout;
	dbgout.assign(match_start,match_len);
	//ZT(("OUT: %s",dbgout.c_str()));
#endif
	return status;
}
z_status zp_obj_parser::_f_output_string(zp_flags flags,zp_mode mode)
{
	z_status status=zs_ok;
	if(_ctx_current->_obj)
	{
		if(flags.parent_data)
		{
			ctext val;
			feature_get_string(_ctx_current->_obj,_ctx_current->_member_var_name,val);
			if(status==zs_ok)
			{
				*_file_out<<val;
				return zs_matched;
			}
			//Z_ASSERT((0));
			return zs_no_match;
		}
		zp_obj_base* obj=
			_ctx_current->_obj->get_child_by_offset(
			_ctx_current->_output_obj_index,
			context_get_current_template_parser().get_index_offset(),
			0);
		if(obj)
		{
			obj->output(_file_out);
			return zs_matched;
		}
	}
	
	if(flags.required || flags.create_default)
		return zs_matched;

	return zs_no_match;
}

z_status zp_obj_parser::_process_single_item(zp_mode mode,zp_flags flags)
{
	//ZTF;
	z_status item_result=zs_template_syntax_error;
	zp_text_parser& tmpl=context_get_current_template_parser();
	z_string debug_data;	
	z_string debug_temp;
	z_string debug_mode;
	int list_item;
	ctext match_start=0;
	size_t match_len=0;
	size_t template_offset=tmpl.get_index_offset();

	debug(debug_data);
	tmpl.debug(debug_temp);
	if(mode.skip_test) debug_mode<<"skip ";
	if(mode.create) debug_mode<<"create ";

	//if(mode.create_empty) debug_mode<<"create_empty ";
	if(mode.nested_group) debug_mode<<"nested_group ";


	ZT(("(%s) <%s> [%s]",debug_mode.c_str(),debug_temp.c_str(),debug_data.c_str()));
	item_type match_type=item_invalid_type;
	_ctx_current->_mode=mode;
	_ctx_current->_flags=flags;
	//----------------------------------------------
	// Process subgroup
	//
	//----------------------------------------------
	if(tmpl.test_char('(')==zs_matched)
	{
		z_status result;
		mode.nested_group=1;
		if(mode.output)
		{
			if(!(flags.required || flags.create_default))
				mode.skip_test=1;
		}
		result=_process_group(flags,mode);
		if(result>zs_fatal_error)
			return result;
		if(tmpl.test_char(')')!=zs_matched)
			return zs_tmpl_expected_closing_parenthesis;

		return result;
	}

	z_status identify_result=zs_no_match;
	for(list_item=0;list_item<keyword_list_count;list_item++)
	{
		keyword_item& ip= keyword_list[list_item];
		identify_result=(tmpl.*(ip._f_identify))(ip.param);
		if(identify_result==zs_no_match)
			continue;
		if(identify_result)
		{
			//we got some other error, bail out.
			break;
		}
		//----------------------------------------------
		// Process Keyword
		//
		//----------------------------------------------
		if(mode.skip_test)
		{
			item_result= zs_skipped;
			break;
		}
		if(mode.input_text)
		{
			Z_ASSERT((ip._f_test));
			item_result=(this->*(ip._f_test))(0);
		}
		if(item_result==zs_matched)
		{
			//----------------------------------------------
			// Create String
			//
			//----------------------------------------------
			if(mode.create)
			{
				if(ip._f_create)
					item_result=(this->*(ip._f_create))(flags,list_item);

			}
		}
		//----------------------------------------------
		// Process Output for strings
		//
		//----------------------------------------------
		if(mode.output)
		{
			if(ip._f_output)
					item_result=(this->*(ip._f_output))(flags,mode);
		}
		break;
	}
	//----------------------------------------------
	// Process Subobj
	//
	//----------------------------------------------
	if(identify_result==zs_no_match)
	{
		if(tmpl.test_any_identifier()==zs_matched)
		{
			zp_obj* sub_obj=0;
			if(mode.skip_test)
				return zs_skipped;
			tmpl.get_match(match_start,match_len);
			const z_obj_fact* ie=find_item(match_start,match_len);
			if(ie==0)
				return check_status(zs_no_entry_for_item);
			if(mode.create)
			{
				if(flags.this_obj)
				{
					sub_obj=_ctx_current->_obj;
				}
				else
				{
					if(flags.parent_data)
					{
						sub_obj=(zp_obj*)get_child_obj(_ctx_current->_member_var_name,_ctx_current->_obj);
					}
					if(!sub_obj)
					{
						sub_obj=create_new_obj(ie);
					}
					else
						feature_clear(sub_obj);
					sub_obj->_templ_offset=
						context_get_current_template_parser().get_index_offset();

				}
			}

			if(mode.output)
			{

				if(flags.parent_data)
				{
					if(flags.multi)
					{
						item_result=feature_objlist_get_next(_ctx_current->_obj,
							_ctx_current->_member_var_name,(z_obj**)&sub_obj);
						if(item_result)
							return zs_no_match;
					}
					else
					{
						sub_obj=(zp_obj*)get_child_obj(_ctx_current->_member_var_name,_ctx_current->_obj);
						if(!sub_obj)
							return zs_no_match;
					}
				}
				if((!sub_obj)&&(_ctx_current->_obj))
				{
					zp_obj_base* obj=
						_ctx_current->_obj->get_child_by_offset(
						_ctx_current->_output_obj_index,
						context_get_current_template_parser().get_index_offset(),
						0
						);

					sub_obj=dynamic_cast<zp_obj*>(obj);
				}
				if(!sub_obj)
				{
					if(!(flags.required || flags.create_default))
						return zs_skipped;
					sub_obj=create_new_obj(ie);
				}
				feature_reset_iter(sub_obj);
				//Reset all of the member var iterators.


			}
			item_result= _process_sub_item(sub_obj,ie,mode,flags);
			if(mode.create)
			{
				if(item_result==zs_matched)
				{
					if(flags.parent_data)
					{
						item_result=feature_objlist_add(_ctx_current->_obj,
							_ctx_current->_member_var_name,sub_obj);
						if(item_result)
							return check_status(item_result);

					}
					else
						_ctx_current->_obj->add_child(sub_obj);
				}
			}
		}
	}
	return check_status(item_result);
	//	return result;
}

