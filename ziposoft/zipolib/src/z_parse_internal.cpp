#include "zipolib_cpp_pch.h"
#include "z_parse_internal.h"


typedef z_status (zp_parser::*type_obj_parser_fp)(const void* p1);
typedef z_status (zp_parser::*type_obj_parser_fp_flags)(zp_flags p1);
typedef z_status (zp_parser::*type_obj_parser_fp_create)(zp_flags p1,int type);
typedef z_status (zp_parser::*type_obj_parser_fp_output)(zp_flags p1,zp_mode mode);


struct keyword_item
{
	item_type type;
	ctext name;
	ctext desc;
	type_txt_parser_fp _f_identify;
	const void* param;
	type_obj_parser_fp _f_test;
	type_obj_parser_fp_create _f_create;
	type_obj_parser_fp_output _f_output;

};

#define FPT(_X_) &zp_text_parser::_X_
#define FPO(_X_) &zp_parser::_X_

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
	{item_quoted_double,	"string",	"quoted_double",	
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
	//{item_sub_obj,	"sub_obj",	"sub object",	FPT(ft_any_identifier),"",FPO(_f_test_sub_obj),0,FPO(_f_output_sub_obj)}
	{item_literal,	"not string",	"string literal",
	FPT(test_not_single_quoted_string),0,
	FPO(_f_not_test_string_literal),0,FPO(_f_output_string)},

};
#define keyword_list_count (sizeof(keyword_list)/sizeof(keyword_item))

z_status zp_parser::get_flags(zp_flags& flags)
{
	//zp_flags& flags=_ctx_current->_flags;
	zp_text_parser& tmpl=context_get_current_template_parser();

	flags.as_u32=0;
	flags.required=1;
	char c=0;
	while(1)
	{
		c=tmpl.current_ch();
		if(tmpl.eob())
		{
			return check_status(zs_template_syntax_error);
		}
		switch(c)
		{


		case '#':
			flags.create_default=1;
			flags.required=0;
			break;
			
		case '%':
			flags.random=1;
			flags.required=0;
			flags.multi=1;

			break;
			
		case '@':
			flags.this_obj=1;
			break;
		case '{':
			tmpl.inc();
			flags.parent_data=1;

			if(tmpl.test_any_identifier()!=zs_matched) 
				return check_status(zs_template_syntax_error);
			tmpl.get_match( _ctx_current->_member_var_name);
			if(tmpl.current_ch()!='}')
				return check_status(zs_template_syntax_error);
			break;


		case '+':
			flags.multi=1;
			break;
		case '!':
			flags.commited=1;
			break;
		case '*':
			flags.multi=1;
			flags.required=0;
			break;
		case '?':
			flags.required=0;
			break;
		case ' ': //skip white space
			break;
		case '-': //skip dash
			break;
		default:
			return zs_matched;
		}
		tmpl.inc();
	}

}

z_status zp_parser::_process_template(zp_mode mode)
{
	zp_flags flags;
	flags.as_u32=0;
	flags.required=1;
	z_status s= _process_group(flags,mode);
	if(s) 
		return s;
	skip_ws();

	if(eob())
		return s;
	return zs_unparsed_data;



}

z_status zp_parser::_process_group(zp_flags flags,zp_mode mode)
{
	//ZTF;
	zp_group_type group_type=zp_group_single;
	z_status group_status=zs_no_match;
	z_status status=zs_no_match;



	zp_text_parser& tmpl=context_get_current_template_parser();
	if(mode.skip_test)
		group_status=zs_skipped;

	zp_mode local_mode=mode;
	bool end_of_group=false;//reached end of template
	bool complete=false;//done processing, but need to skip to end

	//AND group stuff
	bool satisfied=true;
	bool at_least_one=false;
	U64 and_group_bitmask_satisfied=0;
	U64 and_group_bitmask_done=0;
	int current_stage=0;
	ctext tpl_start=tmpl.get_index();
	ctext tpl_random=0;

	while(!end_of_group)
	{

		zp_flags stage_flags=flags; //pass inherited flags
		ctext stage_start=tmpl.get_index();
		U64 stage_bit_mask=(1<<current_stage);
		if(zp_group_and==group_type)
		{
			if(and_group_bitmask_done&stage_bit_mask)
				local_mode.skip_test=1;
			if(mode.output)
				if(and_group_bitmask_satisfied&stage_bit_mask)
					local_mode.skip_test=1;

		}
		/*
		if((mode.output)&&(tpl_random))
		{
			tmpl.set_index(tpl_random);
			zp_flags dummy_flags;
			status=_process_stage(local_mode,&dummy_flags);
			tmpl.set_index(stage_start);
		}*/

		status=_process_stage(local_mode,&stage_flags);
		if(status>zs_fatal_error)
			return check_status(status);

		if(status==zs_no_match)
		{
			if(tpl_random)
			{

				ctext current_index=tmpl.get_index();
				ctext current_index_under_test=tmpl.get_index_under_test();
				tmpl.set_index(tpl_random);
				zp_flags dummy_flags;
				status=_process_stage(local_mode,&dummy_flags);
				tmpl.set_index_under_test(current_index_under_test);
				if(status==zs_matched)
				{
					tmpl.set_index(stage_start);
					continue; //try the stage again
				}
				tmpl.set_index(current_index);
			}
		}		
		if(stage_flags.random)
		{
			tpl_random=stage_start;
		}


		//--------------------------------------
		//	Advance to next stage, check group
		//--------------------------------------
		char next_char=tmpl.current_ch();
		if(tmpl.eob())
		{
			end_of_group=true;
		}		
		else
		{
			switch(next_char)
			{
			case '&':
			case '|':
			case ':':
				{
					if(group_type==zp_group_single)
					{
						group_type=(zp_group_type)next_char;
						if(group_type==zp_group_stage)
							group_status=zs_matched;
						if(group_type==zp_group_or)
							mode.or_group=1;

					}
					if(next_char!=group_type)
					{
						Z_ERROR_MSG(zs_template_syntax_error,"Mixed group types!");
						return check_status(zs_template_syntax_error);
					}
					tmpl.inc(); //Advance
					break;
				}
			case ')':
				end_of_group=true;
				break;
			default:
				return check_status(zs_template_syntax_error);
			}
		}
		//--------------------------------------
		//	Process result based on group type
		//--------------------------------------

		switch(group_type)
		{
		case zp_group_single:
			{
				group_status=status;
			}
			break;
		case zp_group_or:
			{
				switch(status)
				{
				case zs_matched:
					group_status= zs_matched;
					complete=true;
					break;
				case zs_no_match:
				case zs_skipped:
					break;
				case zs_eof:
					//this is an eof for one item. it will rewind and try the next in the OR group
					//complete=true;
					//group_status= zs_eof;
					break;
				default:
					return  status;
					
				}
			}
			break;
		case zp_group_stage:
			{
				switch(status)
				{
				case zs_matched:
				case zs_skipped:
					break;
				case zs_eof:
					/*even if we got an end of buffer error,
					we need to keep going. We only stop if the stage is required
					*/
					//complete=true;
				case zs_no_match:
					if(stage_flags.required)
					{
						group_status=zs_no_match;
						group_status= status;
						complete=true;
					}
					break;
				default:
					return  status;
					break;
				}
			}
			break;
		case zp_group_and:
			{
				// AND groups have to match all stages, BUT
				// in any order, so if any match, the template gets reset and scans again
				
				local_mode=mode;
				
				switch(status)
				{
				case zs_matched:
					at_least_one=true;
					if(!stage_flags.multi)
						and_group_bitmask_done|=stage_bit_mask;
					and_group_bitmask_satisfied|=stage_bit_mask;
					break;
				case zs_eof: 
					//complete=true;
				case zs_no_match:
					if((stage_flags.required)&&
						(!(and_group_bitmask_satisfied&stage_bit_mask)))
							satisfied=false;
					break;

				case zs_skipped: 
					break;
				default:
					return status;
				}
				current_stage++; //only used by AND groups
				if(end_of_group)
				{
					if(at_least_one)
					{
						satisfied=true;//by default, satisfied. 
						at_least_one=false;

						tmpl.set_index(tpl_start);
						current_stage=0;
						end_of_group=false;
						continue;
					}
					complete=true;
					if(satisfied)
					{
						group_status=zs_matched;
						break;
					}
					group_status=zs_no_match;
					
					break;
				}
			}
			break;
		}//Switch for group type
		if(complete)
		{
			if(local_mode.nested_group)
				local_mode.skip_test=1;
			else
				break;
		}
	}
	if(mode.skip_test)
		return zs_skipped;
	return group_status;
}



struct item_process
{
	item_type type;
	ctext name;
	ctext desc;
	type_txt_parser_fp _f_identify;
	ctext param;
	type_obj_parser_fp _f_test;
	type_obj_parser_fp _f_create;
	type_obj_parser_fp _f_output;

};

z_status zp_parser::test_white_space(zp_mode mode)
{
	z_status status=test_cset(*cset_white_space);
	if (status==zs_matched)
	{
		ctext match;
		size_t len;
		get_match(match,len);

		if(mode.create)
		{
			/*
			zp_text* item=new zp_text("ws");
			item->set_text(match,len);
			_ctx_current->_obj->add_child(item);
			*/
			ZT("adding whitespace");
		}
	}
	return status;
}
z_status zp_parser::_process_stage(zp_mode mode,zp_flags* pflags)
{
	//ZTF;
	z_status status;
	zp_flags flags;
	status=get_flags(flags);
	if(status)
		return status;

	if(pflags) 
		*pflags=flags; //return flags to caller
	
	zp_text_parser& tmpl=context_get_current_template_parser();
	ctext tpl_start=tmpl.get_index();

	if(mode.skip_test)
	{
		status=_process_single_item(mode,flags);
		Z_ASSERT(( status==zs_skipped));
		return status;

	}

	if(mode.output)
	{
		int sanity_check_count=0;

		bool satified=false;
		while(1)
		{
			Z_ASSERT(sanity_check_count++<100);
			ctext data_start=get_index();
			if(satified)
				flags.required=0;

			status=_process_single_item(mode,flags);
			if(status==zs_matched)
			{
				if(flags.multi)
				{
					tmpl.set_index(tpl_start);
					satified=true;
					continue;
				}		
			}
			if(status==zs_skipped)
				status=zs_matched;
			if((status==zs_no_match)||(status==zs_eof))
			{
				if(satified) 
					status=zs_matched;
			}
			break;
		}
		return status;
	
	}//End of output



	if(mode.input_text)
	{
		U32 result_index_multi_success_mark=0;

		U32 testnum=_test_result_current_index;
		Z_ASSERT((_results));
		_test_result_current_index++;
		if(!mode.create)
		{
			U32 result=0;
			U32 quanity_matched=0;
			ZT("TEST#%d START>>",testnum);
			_results->set_result(testnum,zp_result_unknown);
			SANITY_CHECK(
				_results->_test_result_tmpl[testnum]=tpl_start;
				ctext sanity_check_data_index=0;
				U32 sanity_check_loop_count=0;
			);

			bool satified=false;
			while(1)
			{
				ctext data_start=get_index();
				SANITY_CHECK(
				{
					sanity_check_loop_count++;
					//Is this a reasonable assumption?
					Z_ASSERT((sanity_check_loop_count<0x100000));

					if(sanity_check_data_index)
					{
						//We should always be advancing the data stream
						Z_ASSERT((data_start!=sanity_check_data_index));
					}
					sanity_check_data_index=data_start;
				});

				status=_process_single_item(mode,flags);
				if(status==zs_matched)
				{
					quanity_matched++;
					if(flags.multi)
					{
						satified=true;
						if(!eob()) 
						{
							tmpl.set_index(tpl_start);
							result_index_multi_success_mark=_test_result_current_index;
							continue;
						}
						ZT("eob, but satisfied");
					}		
				}
				if(status==zs_skipped)
					status=zs_matched;
				if((status==zs_no_match)||(status==zs_eof))
				{
					if(flags.multi)
					{
						_test_result_current_index=(result_index_multi_success_mark);
					}
					set_index(data_start);
					if(satified) 
						status=zs_matched;
				}
				break;
			}
			result=quanity_matched;
			if((quanity_matched==0)&&(status==zs_eof))
			{
				result=zp_result_eof;
			}
			_results->set_result(testnum,result);
			if(quanity_matched==0)
			{
				_test_result_current_index=(testnum+1);
			}
#ifdef DEBUG_RESULT
			U32 i;
			z_string debug_test_results;
			debug_test_results<<"TEST#"<<testnum<<'('<<z_status_get_text(status)<<"):";
			//debug_test_results<<'='<<quanity_matched<< ':';
			for(i=0;i<_results->get_result_count();i++)
			{
				U32 result=(U32)_results->get_result(i);
				if(result==zp_result_unknown)
					debug_test_results<<'?';
				else
					if(result==zp_result_eof)
						debug_test_results<<'e';
					else
						debug_test_results<<result;
				debug_test_results<<' ';
			}

			ZT("%s",debug_test_results.c_str());
#endif
			return status;
		}
		else//if(mode.create)
		{
			U32 test_result=_results->get_result(testnum);
			U32 iterations=1;

			//quanity_matched=_results->get_result(testnum);
			

			SANITY_CHECK(
			ctext tmpl_check=_results->_test_result_tmpl[testnum];
			Z_ASSERT((tmpl_check==tpl_start ));
			)

			ZT("CREATE#%d[%d] START>>",testnum,test_result);

			if((test_result==zp_result_no_match)
			   ||(test_result==zp_result_eof))
			{
				mode.skip_test=1;
			}			
			else
				iterations=test_result;

			while(iterations--)
			{
				ctext data_start=get_index();

				status=_process_single_item(mode,flags);
				if(status>zs_fatal_error)
					return status;
				if(iterations)
				{
					tmpl.set_index(tpl_start);
				}
			}
			if(test_result==zp_result_no_match)
				status=zs_no_match;
			if(test_result==zp_result_eof)
				status=zs_eof;
			ZT("<<EXIT#%d -%s",testnum,z_status_get_text(status));
			return status;
		}
	}
	return check_status(zs_internal_error);

}


z_status zp_parser::_process_sub_item(void* sub_obj,
										  const z_factory_static* ie,
										  zp_mode mode,zp_flags flags)
{
	//ZTF;
	context_sub_item_push(sub_obj,ie);
	z_string raw;
	debug(raw);
	ZT("%c %s[%s]-> %s",(mode.create?'C':'T'),
		ie->get_name()
		,ie->get_parse_string(),raw.c_str());
	U32 ng=mode.nested_group;
	mode.nested_group=0;


	
	z_status status=_process_group(flags,mode);
	mode.nested_group=ng;

	ZT("<- %s=%s",ie->get_name(),z_status_get_text(status));

	if(status>zs_internal_error)
		return status;
	context_sub_item_pop();

	return status;


}




z_status zp_parser::_f_ident_list_test(const void* dummy)
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
				Z_ASSERT(0); //ADD STRING?
				z_status i_result=0;//feature_set_string(_ctx_current->_obj,_ctx_current->_member_var_name,match,match_len);

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

z_status zp_parser::_f_ident_list_create(zp_flags flags,int type)
{
	return zs_matched;

}
z_status zp_parser::_f_ident_list_output(zp_flags flags,zp_mode mode)
{
	z_status result=zs_ok;
	bool need_comma=false;

	if((_ctx_current->_obj)&&(flags.parent_data))
	{
		Z_ASSERT(0); //FIX THIS
		//feature_reset_iter(_ctx_current->_obj,_ctx_current->_member_var_name);
		while(result==zs_ok)
		{
			ctext s="";
			Z_ASSERT(0); //get STRING by index?
			result=0;//feature_get_string(_ctx_current->_obj,_ctx_current->_member_var_name,s);
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
z_status zp_parser::_f_squoted_string_test(const void* dummy)
{
	return test_single_quoted_string();
}
z_status zp_parser::_f_quoted_string_test(const void* dummy)
{
	return test_code_string();
}
z_status zp_parser::_f_test_ident(const void* dummy)
{
	return test_any_identifier();
}
z_status zp_parser::_f_test_whsp(const void* dummy)
{
	return test_cset(*cset_white_space);
}
z_status zp_parser::_f_test_path(const void* dummy)
{
	return test_cset(*cset_path_string);

}
z_status zp_parser::_f_test_to_eob(const void* dummy)
{
	return test_to_eob();
}

z_status zp_parser::_f_create_string(zp_flags flags,int type)
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
				z_string temp;
				temp.assign( match_start,match_len);
				status=_ctx_current->_obj_factory->set_var_as_string( _ctx_current->_obj,_ctx_current->_member_var_name,temp.c_str());
				//status=feature_set_string(_ctx_current->_obj,_ctx_current->_member_var_name,match_start,match_len);
			}
		}
		else
		{
			/*
			zp_text* item=0;
			item=new zp_text();
			item->_type=type;
			_ctx_current->_obj->add_child(item);
			item->set_text(match_start,match_len);
			item->_templ_offset=
				context_get_current_template_parser().get_index_offset();
				*/
		}
	}
	return status;
}

z_status zp_parser::_f_test_string_literal(const void* dummy)
{
	z_status status=zs_no_match;
	ctext match_start=0;
	size_t match_len=0;
	zp_text_parser& tmpl=context_get_current_template_parser();
	tmpl.get_match(match_start,match_len);
	status= test_string(match_start,match_len);
	return status;
}
z_status zp_parser::_f_not_test_string_literal(const void* dummy)
{
	//TODO!!!
	z_status status=zs_no_match;
	ctext match_start=0;
	size_t match_len=0;
	zp_text_parser& tmpl=context_get_current_template_parser();
	tmpl.get_match(match_start,match_len);
	status= test_not_string(match_start,match_len);
	return status;
}

z_status zp_parser::_f_string_literal_create(zp_flags flags,int type)
{
	z_status status=zs_ok;
	if(flags.parent_data)
	{
		if(_ctx_current->_obj)
		{
			bool* pVar=0;
			_ctx_current->_obj_factory->get_var_ptr(
				_ctx_current->_obj,_ctx_current->_member_var_name,(void**)&pVar,0);
			if(pVar)
				*pVar=true;
		}
	}
	return status;
}
z_status zp_parser::_f_string_literal_output(zp_flags flags,zp_mode mode)
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
	//ZT("OUT: %s",dbgout.c_str()));
#endif
	return status;
}
z_status zp_parser::_f_output_string(zp_flags flags,zp_mode mode)
{
	z_status status=zs_ok;
	if(_ctx_current->_obj)
	{
		if(flags.parent_data)
		{
			z_string val;
			_ctx_current->_obj_factory->get_var_as_string(
				_ctx_current->_obj,_ctx_current->_member_var_name,val);
			if(status==zs_ok)
			{
				*_file_out<<val;
				return zs_matched;
			}
			//Z_ASSERT((0));
			return zs_no_match;
		}

	}
	
	if(flags.required || flags.create_default)
		return zs_matched;

	return zs_no_match;
}

z_status zp_parser::_process_single_item(zp_mode mode,zp_flags flags)
{
	//ZTF;
	z_status item_result=zs_template_syntax_error;
	zp_text_parser& tmpl=context_get_current_template_parser();
	int list_item;
	ctext match_start=0;
	size_t match_len=0;
	size_t template_offset=tmpl.get_index_offset();

#if DEBUG
	z_string debug_data;	
	z_string debug_temp;
	z_string debug_mode;

	debug(debug_data);
	tmpl.debug(debug_temp);
	if(mode.skip_test) debug_mode<<"skip ";
	if(mode.create) debug_mode<<"create ";

	//if(mode.create_empty) debug_mode<<"create_empty ";
	if(mode.nested_group) debug_mode<<"nested_group ";


	ZT("(%s) <%s> [%s]",debug_mode.c_str(),debug_temp.c_str(),debug_data.c_str());

#endif
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
			void* sub_obj=0;
			if(mode.skip_test)
				return zs_skipped;
			tmpl.get_match(match_start,match_len);
			const z_factory_static* fact_new_obj=find_item(match_start,match_len);
			if(fact_new_obj==0)
				return check_status(zs_no_entry_for_item);
			if(mode.create)
			{
				if(flags.this_obj)
				{
					sub_obj=_ctx_current->_obj;	// what does this even mean? its for the @ operator 
				}
				else
				{
					if(flags.parent_data)
					{
						_ctx_current->_obj_factory->create_child(_ctx_current->_obj,
							_ctx_current->_member_var_name,fact_new_obj,
							&sub_obj);
					}
					/*
					if(!sub_obj)
					{
						sub_obj=ie->create_obj();
					}
					else
						feature_clear(sub_obj);
					//sub_obj->_templ_offset=	 						context_get_current_template_parser().get_index_offset();
					*/

				}
			}

			if(mode.output)
			{

				if(flags.parent_data)
				{
					/*
					if(flags.multi)
					{
						item_result=feature_objlist_get_next(_ctx_current->_obj,
							_ctx_current->_member_var_name,(void**)&sub_obj);
						if(item_result)
							return zs_no_match;
					}
					else
					{*/
					_ctx_current->_obj_factory->get_var_ptr(
						_ctx_current->_obj,
						_ctx_current->_member_var_name,
						&sub_obj, 
						&_ctx_current->_output_obj_index);
					if(!sub_obj)
						return zs_no_match;
					
				}
				if((!sub_obj)&&(_ctx_current->_obj))
				{
					/*
					void* obj=
						_ctx_current->_obj->get_child_by_offset(
						_ctx_current->_output_obj_index,
						context_get_current_template_parser().get_index_offset(),
						0
						);

					sub_obj=dynamic_cast<void*>(obj);
					*/
				}
				if(!sub_obj)
				{
					if(!(flags.required || flags.create_default))
						return zs_skipped;
					sub_obj=fact_new_obj->create_obj();
				}
				//Reset all of the member var iterators.


			}
			item_result= _process_sub_item(sub_obj,fact_new_obj,mode,flags);
			if(mode.create)
			{
				if(item_result==zs_matched)
				{
					if(flags.parent_data)
					{
						//ASSIGNMENT of child object? 
						/*
						item_result=feature_objlist_add(_ctx_current->_obj,	_ctx_current->_member_var_name,sub_obj);
						if(item_result)
							return check_status(item_result);
							*/

					}
					else
					{
							//_ctx_current->_obj->add_child(sub_obj);
					}
				}
			}
		}
	}
	return check_status(item_result);
	//	return result;
}

z_status zp_parser::advance(size_t count)
{
	z_status s=zp_text_parser::advance(count);
	if(s)
		return s;

	if(get_index()>_index_furthest)
		_index_furthest=get_index();
	
	return zs_ok;
}