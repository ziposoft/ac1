#include "z_parse_pch.h"
#include "z_parse.h"


ctext status_text[]=
{
	"zs_matched",
	"zs_no_match",
	"zs_skipped",
	"zs_unparsed_data",
	"zs_eof",
	"zs_end_of_list",
	"zs_child_not_found",
	"zs_fatal_error", //fatal errors below. Status above!
	"zs_template_syntax_error",
	"zs_error",
	"zs_feature_not_found",
	"zs_operation_not_supported",
	"zs_cannot_create_virtual_obj",
	"zs_no_entry_for_item",
	"zs_syntax_error",
	"zs_tmpl_expected_closing_parenthesis",
	"zs_internal_error",
	"zs_bad_argument_2",

};

ctext z_status_get_text(zp_status status)
{
		return status_text[status];
}


zp_status zp_obj_parser::get_flags(zp_flags& flags)
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

			/*
			case '$':
			{
			tmpl.inc();
			flags|=FLAG_PARENT_DATA;
			char digit=tmpl.current_ch();
			if(!tmpl.eob())
			{
			if((digit>='1')&&(digit<='9'))
			{
			_ctx_current->_variable_index=digit-'0';
			break;
			}
			}
			return check_status(zs_template_syntax_error);
			}
			break;
			
		case 'w':
			flags.ignore_ws=1;
			break;*/
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

zp_status zp_obj_parser::_process_template(zp_mode mode)
{
	zp_flags flags;
	flags.as_u32=0;
	flags.required=1;
	zp_status s= _process_group(flags,mode);
	if(s) 
		return s;
	skip_ws();

	if(eob())
		return s;
	return zs_unparsed_data;



}

zp_status zp_obj_parser::_process_group(zp_flags flags,zp_mode mode)
{
	//ZTF;
	zp_group_type group_type=zp_group_single;
	zp_status group_status=zs_no_match;
	zp_status status=zs_no_match;



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
						return check_status(zs_template_syntax_error);
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

zp_status zp_obj_parser::test_white_space(zp_mode mode)
{
	zp_status status=test_cset(*cset_white_space);
	if (status==zs_matched)
	{
		ctext match;
		size_t len;
		get_match(match,len);

		if(mode.create)
		{
			zp_text* item=new zp_text("ws");
			item->set_text(match,len);
			_ctx_current->_obj->add_child(item);
			ZT(("adding whitespace"));
		}
	}
	return status;
}
zp_status zp_obj_parser::_process_stage(zp_mode mode,zp_flags* pflags)
{
	//ZTF;
	zp_status status;
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
			ZT(("TEST#%d START>>",testnum));
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
						ZT(("eob, but satisfied"));
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

			ZT(("%s",debug_test_results.c_str()));
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

			ZT(("CREATE#%d[%d] START>>",testnum,test_result));

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
			ZT(("<<EXIT#%d -%s",testnum,z_status_get_text(status)));
			return status;
		}
	}
	return check_status(zs_internal_error);

}


zp_status zp_obj_parser::_process_sub_item(zp_obj* sub_obj,
										  const z_obj_fact* ie,
										  zp_mode mode,zp_flags flags)
{
	//ZTF;
	context_sub_item_push(sub_obj,ie);
	z_string raw;
	debug(raw);
	ZT(("%c %s[%s]-> %s",(mode.create?'C':'T'),
		z_obj_fact_get_name(ie)
		,ie->parse_string,raw.c_str()));
	U32 ng=mode.nested_group;
	mode.nested_group=0;


	
	zp_status status=_process_group(flags,mode);
	mode.nested_group=ng;

	ZT(("<- %s=%s",z_obj_fact_get_name(ie),z_status_get_text(status)));

	if(status>zs_internal_error)
		return status;
	context_sub_item_pop();

	return status;


}


