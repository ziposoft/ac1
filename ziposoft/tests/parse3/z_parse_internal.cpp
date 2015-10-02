#include "zipolib/include/zipo.h"


#include "z_parse_internal.h"
#if 0
#undef	ZT
#define	ZT(...)
#endif  


typedef z_status (z_zipex_base::*type_obj_parser_fp)(ctext p1);
typedef z_status (z_zipex_base::*type_obj_parser_fp_test)();
typedef z_status (z_zipex_base::*type_obj_parser_fp_flags)(zp_flags p1);
typedef z_status (z_zipex_base::*type_obj_parser_fp_create)(zp_flags p1,int type);
typedef z_status (z_zipex_base::*type_obj_parser_fp_output)(zp_flags p1,zp_mode mode);


struct keyword_item
{
	item_type type;
	ctext name;
	ctext desc;
	type_txt_parser_fp _f_identify;
	const void* param;
	type_obj_parser_fp_test _f_test;

};

#define FPT(_X_) &zp_text_parser::_X_
#define FPO(_X_) &z_zipex_base::_X_

keyword_item keyword_list[]={
	{item_literal,		"literal",	"string literal",		
	FPT(ft_single_quoted_string),0,			
	FPO(_f_test_string_literal),
	},
	{item_integer,		"int",		"integer",				
	FPT(ft_test_identifier),(void*)"int",	
	FPO(_f_digits),
	},
	{item_letters,		"letters",		"letters",				FPT(ft_test_identifier),(void*)"Az",	FPO(_f_letters)},
	{item_quoted_double,	"string",	"quoted_double",	
	FPT(ft_test_identifier),"string",		
	FPO(_f_quoted_string_test)},
	{item_quoted_single,	"string_sq",	"quoted_single",
	FPT(ft_test_identifier),"string_sq",	
	FPO(_f_squoted_string_test)},
	{item_filepath,"item_filepath",	"identifier",			FPT(ft_test_identifier),"filepath",0},
	{item_identifier,"ident",	"identifier",				FPT(ft_test_identifier),"ident",		FPO(_f_test_ident)},
	{item_identifier,"identlist",	"identifier",			FPT(ft_test_identifier),"identlist",	FPO(_f_ident_list_test)},
	{item_scoped,	"scoped",	"scoped",					FPT(ft_test_identifier),"scoped",		FPO(_f_scoped_identchars)},
	{item_toeol,	"toeob",	"Find EOB",					FPT(ft_test_identifier),"toeol",		FPO(_f_test_to_eob)},
	{item_whsp,	"whsp",	"White space",						FPT(ft_test_identifier),"whsp",			FPO(_f_test_whsp)},
	//{item_sub_obj,	"sub_obj",	"sub object",	FPT(ft_any_identifier),"",FPO(_f_test_sub_obj),0,FPO(_f_output_sub_obj)}
	{item_literal,	"not string",	"string literal",	FPT(test_not_single_quoted_string),0,	FPO(_f_not_test_string_literal)},

};
#define keyword_list_count (sizeof(keyword_list)/sizeof(keyword_item))



z_zipex_base::z_zipex_base()
{
	_flags.as_u32=0;
	_groupnum=0;
	_mode.nested_group=0;
	_mode.skip_test=0;
	_last_status=zs_ok;
	_furthest_index=0;
	_result_index=0;

	_err_msg="";
}
void z_zipex_base::reset()
{
	reset_streams();
	_flags.as_u32=0;
	_groupnum=0;
	_mode.nested_group=0;
	_mode.skip_test=0;
	_last_status=zs_ok;
	_furthest_index=0;
	_result_index=0;
	_err_msg="";

}

z_status z_zipex_base::get_flags(zp_flags& flags)
{
	//zp_flags& flags=_ctx_current->_flags;
	zp_text_parser& tpl=tmpl();

	flags.as_u32=0;
	flags.required=1;
	char c=0;
	while(1)
	{
		c=tpl.current_ch();
		if(tpl.eob())
		{
			return return_error(zs_template_syntax_error,"Unexpected end of template");
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
			tpl.inc();
			flags.parent_data=1;

			if(tpl.test_any_identifier()!=zs_matched) 
				return return_error(zs_template_syntax_error,"Expected label");
			tpl.get_match( _member_var_name);
			if(tpl.current_ch()!='}')
				return return_error(zs_template_syntax_error,"Expected curly brace");
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
		tpl.inc();
	}

}

z_status z_zipex_base::_process_template()
{
	zp_flags flags;
	zp_mode mode;
	reset();
	flags.as_u32=0;
	mode.as_u32=0;

	flags.required=1;
	z_status s= _process_group(flags,mode);
	if(s) 
		return s;
	data().skip_ws();

	if(data().eob())
		return s;
	return zs_unparsed_data;



}

z_status z_zipex_base::_process_group(zp_flags flags,zp_mode mode)
{
	//ZTF;
	zp_group_type group_type=zp_group_single;
	z_status group_status=zs_no_match;
	z_status status=zs_no_match;



	zp_text_parser& tpl=tmpl();
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
	ctext tpl_start=tpl.get_index();
	ctext tpl_random=0;

	while(!end_of_group)
	{

		zp_flags stage_flags=flags; //pass inherited flags
		ctext stage_start=tpl.get_index();
		U64 stage_bit_mask=(1<<current_stage);
		if(zp_group_and==group_type)
		{
			if(and_group_bitmask_done&stage_bit_mask)
				local_mode.skip_test=1;

		}


		status=_process_stage(local_mode,&stage_flags);
		if(status>zs_fatal_error)
		{
			return data().check_status(status);
			//return Z_ERROR(status);
		}

		if(status==zs_no_match)
		{
			if(tpl_random)
			{

				ctext current_index=tpl.get_index();
				ctext current_index_under_test=tpl.get_index_under_test();
				tpl.set_index(tpl_random);
				zp_flags dummy_flags;
				status=_process_stage(local_mode,&dummy_flags);
				tpl.set_index_under_test(current_index_under_test);
				if(status==zs_matched)
				{
					tpl.set_index(stage_start);
					continue; //try the stage again
				}
				tpl.set_index(current_index);
			}
		}		
		if(stage_flags.random)
		{
			tpl_random=stage_start;
		}


		//--------------------------------------
		//	Advance to next stage, check group
		//--------------------------------------
		char next_char=tpl.current_ch();
		if(tpl.eob())
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

					}
					if(next_char!=group_type)
					{
						Z_ERROR_MSG(zs_template_syntax_error,"Mixed group types!");
						return return_error(zs_template_syntax_error,"Mixed group types");
					}
					tpl.inc(); //Advance
					break;
				}
			case ')':
				end_of_group=true;
				break;
			default:
				return return_error(zs_template_syntax_error,"Invalid template character");
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

						tpl.set_index(tpl_start);
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





z_status z_zipex_base::test_white_space(zp_mode mode)
{
	z_status status=data().test_cset(*cset_white_space);
	if (status==zs_matched)
	{
		ctext match;
		size_t len;
		data().get_match(match,len);

	}
	return status;
}
z_status z_zipex_base::_process_stage(zp_mode mode,zp_flags* pflags)
{
	//ZTF;
	z_status status;
	zp_flags flags;
	status=get_flags(flags);
	if(status)
		return status;

	if(pflags) 
		*pflags=flags; //return flags to caller

	zp_text_parser& tpl=tmpl();
	ctext tpl_start=tpl.get_index();

	if(mode.skip_test)
	{
		status=_process_single_item(mode,flags);
		Z_ASSERT(( status==zs_skipped));
		return status;

	}

	U32 result=0;
	U32 quanity_matched=0;
	static U32 test_num=0;
	ZT("TEST# %d START>>",++test_num);

	bool satified=false;
	while(1)
	{
		ctext data_start=data().get_index();
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
				if(!data().eob()) 
				{
					tpl.set_index(tpl_start);
					continue;
				}
				ZT("eob, but satisfied");
			}		
		}
		if(status==zs_skipped)
			status=zs_matched;
		if((status==zs_no_match)||(status==zs_eof))
		{

			data().set_index(data_start);
			if(satified) 
				status=zs_matched;
		}
		break;
	}
	result=quanity_matched;
	if((quanity_matched==0)&&(status==zs_eof))
	{
		result=-1; // ?????????
	}
	ZT("<<TEST# %d =%s",test_num--,zs_get_status_text(status));
	return status;


}



z_status z_zipex_base::_f_ident_list_test()
{
	z_status result; 
	z_status result_total=zs_no_match; 


	while(1)
	{
		result= data().test_any_identifier();
		if((result==zs_no_match)||(result==zs_eof))
			return result_total;
		if(result)
			return result;
		result_total=zs_matched;

		result=data().test_char(',');
		if((result==zs_no_match)||(result==zs_eof))
			return result_total;
		if(result)
			return result;
	}
}

z_status z_zipex_base::_f_squoted_string_test()
{
	return data().test_single_quoted_string();
}
z_status z_zipex_base::_f_digits()
{
	return data().ft_digits(0);
}
z_status z_zipex_base::_f_letters()
{
	return data().ft_letters(0);
}
z_status z_zipex_base::_f_scoped_identchars()
{
	return data().ft_scoped_identchars(0);
}

z_status z_zipex_base::_f_quoted_string_test()
{
	return data().test_code_string();
}
z_status z_zipex_base::_f_test_ident()
{
	return data().test_any_identifier();
}
z_status z_zipex_base::_f_test_whsp()
{
	return data().test_cset(*cset_white_space);
}
z_status z_zipex_base::_f_test_path()
{
	return data().test_cset(*cset_path_string);

}
z_status z_zipex_base::_f_test_to_eob()
{
	return data().test_to_eob();
}
#if 0 
z_status z_zipex_base::_f_create_string(zp_flags flags,int type)
{
	z_status status=zs_matched;
	ctext match_start=0;
	size_t match_len=0;

	data().get_match(match_start,match_len);
	if(match_start)
	{
		if(flags.parent_data)
		{
			z_string temp,unescaped;
			temp.assign( match_start,match_len);
			z_str_unescape(temp,unescaped);
			//status=_ctx_current->_obj_factory->set_var_as_string( _ctx_current->_obj,_ctx_current->_member_var_name,unescaped.c_str());
			//status=feature_set_string(_ctx_current->_obj,_ctx_current->_member_var_name,match_start,match_len);
		}
		else
		{
			/*
			zp_text* item=0;
			item=z_new zp_text();
			item->_type=type;
			_ctx_current->_obj->add_child(item);
			item->set_text(match_start,match_len);
			item->_templ_offset=
			tmpl().get_index_offset();
			*/
		}
	}
	return status;
}
#endif
z_status z_zipex_base::_f_test_string_literal()
{
	z_status status=zs_no_match;
	ctext match_start=0;
	size_t match_len=0;
	tmpl().get_match(match_start,match_len);
	status= data().test_string(match_start,match_len);
	return status;
}
z_status z_zipex_base::_f_not_test_string_literal()
{
	//TODO!!!
	z_status status=zs_no_match;
	ctext match_start=0;
	size_t match_len=0;
	tmpl().get_match(match_start,match_len);
	status= data().test_not_string(match_start,match_len);
	return status;
}


z_status z_zipex_base::_process_single_item(zp_mode mode,zp_flags flags)
{
	//ZTF;
	z_status item_result=zs_template_syntax_error;
	zp_text_parser& tpl=tmpl();
	int list_item;
	ctext match_start=0;
	size_t match_len=0;
	size_t template_offset=tpl.get_index_offset();

	if(data().get_index()>_furthest_index)
	{
		_furthest_index=data().get_index();

	}
#if DEBUG
	z_string debug_data;	
	z_string debug_temp;
	z_string debug_mode;

	data().debug(debug_data);
	tpl.debug(debug_temp);
	if(mode.skip_test) debug_mode<<"skip ";

	//if(mode.create_empty) debug_mode<<"create_empty ";
	if(mode.nested_group) debug_mode<<"nested_group ";


	ZT("(%s) <%s> [%s]",debug_mode.c_str(),debug_temp.c_str(),debug_data.c_str());

#endif
	item_type match_type=item_invalid_type;
	//----------------------------------------------
	// Process subgroup
	//
	//----------------------------------------------
	if(tpl.test_char('(')==zs_matched)
	{
		z_status result;
		mark_group_start();
		_groupnum++;
		mode.nested_group=1;
		match_start=data().get_index();
		result=_process_group(flags,mode);

		mark_group_end(result);
		if(result>zs_fatal_error)
			return result;
		if(tpl.test_char(')')!=zs_matched)
			return zs_tmpl_expected_closing_parenthesis;

		return result;
	}

	z_status identify_result=zs_no_match;
	for(list_item=0;list_item<keyword_list_count;list_item++)
	{
		keyword_item& ip= keyword_list[list_item];
		identify_result=(tpl.*(ip._f_identify))(ip.param);
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
		Z_ASSERT((ip._f_test));
		item_result=(this->*(ip._f_test))();

		if(item_result==zs_matched)
		{
			//----------------------------------------------
			// Create String
			//
			//----------------------------------------------
			mark_create_string();
		}


		break;
	}
	//----------------------------------------------
	// Unkown keyword
	//
	//----------------------------------------------
	if(identify_result==zs_no_match)
	{
		if(tpl.test_any_identifier()==zs_matched)
		{
			z_status status=zs_ok;
			void* sub_obj=0;
			if(mode.skip_test)
				return zs_skipped;
			tpl.get_match(match_start,match_len);

			// This is a keyword we do not recognize
			return return_error(zs_no_entry_for_item,"Unknown template keyword");

		}
		else
			return return_error(zs_template_syntax_error,"Unexpected template character");
	}
	return relay_error(item_result);
	//	return result;
}
z_status z_zipex_base::output(z_file* fp)
{
	return zs_operation_not_supported;
}


z_status z_zipex_base::_parse()
{
	z_status status;
	status=_process_template();
	return status;
}
