#ifndef z_parse_internal_h
#define z_parse_internal_h
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_factory.h"
#include "zipolib/include/z_error.h"

const U8 zp_result_no_match=0;

const U8 zp_result_unknown=0xFD;
const U8 zp_result_eof=0xFE;
const U8 zp_result_extended_value=0xFF;
#if DEBUG
//#define SANITY_CHECKS 1
//#define DEBUG_RESULT 1
//
#else
#endif //#if DEBUG

#if  SANITY_CHECKS
#define SANITY_CHECK(_X_) _X_
#else
#define SANITY_CHECK(_X_) 
#endif





enum zp_item_type
{
	zpi_literal,
	zpi_string,
	zpi_cont
};
enum zp_group_type
{
	zp_group_single='.',
	zp_group_or='|',
	zp_group_stage=':',
	zp_group_and='&'
};


union zp_flags {
	struct {
		 U32 ignore_ws:1;
		 U32 parent_data:1;
		 U32 multi:1;
		 U32 required:1;
		 U32 commited:1;
		 U32 parent_name:1; //???
		 U32 ignore_nl:1; //???
		 U32 create_default:1;
		 U32 this_obj:1;
		 U32 group_or:1;
		 U32 group_and:1;
		 U32 group_stage:1;
		 U32 group_single:1;
		 U32 random:1;
		 //U32 nested_group:1;
	};
	U32 as_u32;
};
/*
enum zp_mode
{
	zp_mode_skip,
	zp_mode_test,
	zp_mode_create,
	zp_mode_output,
};
*/
enum zp_operation
{
	zp_oper_test,
	zp_oper_create,
	zp_oper_output
};

struct zp_mode{
		 U32 input_text:1;
		 U32 input_obj:1;
		 U32 create:1;
		 U32 output:1;
		 U32 skip_test:1;
		 U32 nested_group:1;
		 U32 or_group:1;
		 //U32 unused:27;
};
const zp_mode zp_mode_parse_input={ 1,0,0,0,0 };
const zp_mode zp_mode_parse_create={ 1,0,1,0,0 };
const zp_mode zp_mode_create_from_obj={ 0,1,1,0,0 };
const zp_mode zp_mode_create_empty={ 0,0,1,0,0 };
const zp_mode zp_mode_output_obj={ 0,0,0,1, };
const zp_mode zp_mode_output_default={ 0,0,0,1 };



//________________________________________________________________
//
//zpi_context
//________________________________________________________________
class zpi_context
{
public:
	zpi_context();
	void init(zpi_context* parent,
					   z_factory* ie,
					   ctext parse_string
					   	);
	//void* get_next_child_obj();




	zpi_context* _parent;
	zpi_context* _child;
	void* _obj;
	z_factory* _obj_factory;
	zp_text_parser _current_template_parser;
	//U32 _output_result_index;
	z_obj_list_iter _output_obj_iter;
	
	//current stage
	zp_flags _flags;
	zp_mode _mode;
	//U32 _variable_index;
	//TODO these dont need to be in the context
	z_string _member_var_name;
private:

};




class zp_test_result
{
	std::vector<U8> _test_results;
	std::map<U32,U32> _test_results_ex;

public:
	zp_test_result();
	U32 add_result(U32 result);
	U32 set_result(U32 index,U32 result);
	U32 get_result(U32 index);
	U32 get_result_count();
	void clear();
	/*
	U32 get_index();
	void inc_index();
	U32 set_index(U32 index);
	*/
#ifdef SANITY_CHECKS
#endif
};
//________________________________________________________________
//
//zp_parser
//________________________________________________________________

class zp_parser : public zp_text_parser //, public zo_manipulator
{
	zp_test_result* _results;
	zpi_context _ctx_root;
	zpi_context *_ctx_current;
	U32 _test_result_current_index;

	ctext _furthest_index;//TODO
	ctext _furthest_obj;//TODO
	ctext _furthest_tmpl;//TODO



	z_status _last_status;

	z_file* _file_out;


	//initialization
	/*
	void set_obj_table(z_factory** it,size_t size);
	zp_parser(z_factory** it,size_t size);
	*/


	//parsing




	z_status create_empty_item(void*& p_item_out,ctext item_entry_name);







	//context
	z_factory* find_item(ctext item_name,size_t len);
	z_status _process_stage(zp_mode mode ,zp_flags* pflags=0);
	void reset_results();
	z_status _process_template(zp_mode mode);
	z_status _process_group(zp_flags flags,zp_mode mode);
	z_status test_white_space(zp_mode mode);
	z_status _process_sub_item(void* obj,z_factory* ie,zp_mode mode,zp_flags flags);
	z_status _process_sub_obj(ctext start, size_t len,zp_mode mode,zp_flags flags);
	z_status _process_single_item(zp_mode mode,zp_flags flags);
	z_status get_flags(zp_flags& flags);
	zp_text_parser& context_get_current_template_parser();
	void context_set_root(void* p_item,z_factory* ie, ctext parse_string);
	void context_sub_item_push(void* obj,z_factory* ie);
	void context_sub_group_push(void* obj);
	void context_sub_item_pop();
   public:
	//String literal
	z_status _f_string_literal_output(zp_flags p1,zp_mode mode);
	z_status _f_test_string_literal(const void* dummy);
	z_status _f_string_literal_create(zp_flags p1,int type);

	//NOT String literal
	z_status _f_not_test_string_literal(const void* dummy);

	//Simple strings
	z_status _f_output_string(zp_flags p1,zp_mode mode);
	z_status _f_test_ident(const void* dummy);
	z_status _f_create_string(zp_flags p1,int type);
	z_status _f_test_to_eob(const void* dummy);
	z_status _f_test_whsp(const void* dummy);
	z_status _f_test_path(const void* dummy);

	//ident list
	z_status _f_ident_list_test(const void* dummy);
	z_status _f_ident_list_create(zp_flags p1,int type);
	z_status _f_ident_list_output(zp_flags p1,zp_mode mode);
	//quoted strings

	z_status _f_quoted_string_test(const void* dummy);
	z_status _f_squoted_string_test(const void* dummy);


public:

	zp_parser();
 	z_status report_error();
 	z_status parse_template(ctext tmpl);
	z_status parse_item(void*& p_item_out,ctext item_entry_name);
	z_status output_obj(z_file* fp,z_factory* factory,void* obj);
	z_status output_default_template(z_file* fp,ctext tmpl);
	z_status create_obj(ctext item_entry_name,void* &p_item);
	z_status parse_obj_f(void* p_obj,z_factory* factory,ctext data,ctext tmpl=0);



};




#endif

