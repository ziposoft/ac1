#ifndef z_parse_h
#define z_parse_h
#include "zipolib_parse2/include/z_parse_text.h"
#include "zipolib_parse2/include/z_parse_obj.h"
#include "zipolib_parse2/include/z_obj_man.h"


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
					   const zp_factory* ie,
					   ctext parse_string
					   	);
	//void* get_next_child_obj();


	zpi_context* _parent;
	zpi_context* _child;
	void* _obj;
	const zp_factory* _obj_factory;
	zp_text_parser _current_template_parser;
	//U32 _output_result_index;
	int _output_obj_index;
	
	//current stage
	zp_flags _flags;
	zp_mode _mode;
	//U32 _variable_index;
	//TODO these dont need to be in the context
	z_string _member_var_name;
	


};
//________________________________________________________________
//
//void_parser
//________________________________________________________________

class void_parser : public zp_text_parser, public zo_manipulator
{
	zp_test_result* _results;
	zpi_context _ctx_root;
	zpi_context *_ctx_current;
	U32 _test_result_current_index;


public:


	z_file* _file_out;


	//initialization
	/*
	void set_obj_table(const zp_factory** it,size_t size);
	void_parser(const zp_factory** it,size_t size);
	*/
	void_parser();


	//parsing


	z_status parse_template(void*& p_item_out,ctext tmpl);
	z_status parse_item(void*& p_item_out,ctext item_entry_name);
	z_status parse_obj(void* p_obj,z_string& data_in);
	z_status parse_obj(void* p_obj,ctext data);
	z_status create_empty_item(void*& p_item_out,ctext item_entry_name);



	z_status report_error(z_status status);

	//item table
/*
	z_status feature_callback(type_memvar_oper oper,size_t* size,void*& ppObj);
	z_status feature_callback(type_memvar_oper oper,size_t* size,void_container& member_var);
	z_status feature_callback(type_memvar_oper oper,size_t* size,zo_str_map& member_var);
	z_status feature_callback(type_memvar_oper oper,size_t* size,void& ppObj);
	z_status feature_callback(type_memvar_oper oper,size_t* size,z_string& member_var);
*/
	void* create_new_obj(const zp_factory* ie);
	z_status create_obj(ctext item_entry_name,void* &p_item);


	z_status output_obj(z_file* fp,void* obj);
	z_status output_default_template(z_file* fp,ctext tmpl);
	//context
private:
	const zp_factory* find_item(ctext item_name,size_t len=(size_t)-1);
	z_status _process_stage(zp_mode mode ,zp_flags* pflags=0);
	void reset_results();
	z_status _process_template(zp_mode mode);
	z_status _process_group(zp_flags flags,zp_mode mode);
	z_status test_white_space(zp_mode mode);
	z_status _process_sub_item(void* obj,const zp_factory* ie,zp_mode mode,zp_flags flags);
	z_status _process_sub_obj(ctext start, size_t len,zp_mode mode,zp_flags flags);
	z_status _process_single_item(zp_mode mode,zp_flags flags);
	z_status get_flags(zp_flags& flags);
	zp_text_parser& context_get_current_template_parser();
	void context_set_root(void* p_item,const zp_factory* ie, ctext parse_string);
	void context_sub_item_push(void* obj,const zp_factory* ie);
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


};

typedef z_status (void_parser::*type_obj_parser_fp)(const void* p1);
typedef z_status (void_parser::*type_obj_parser_fp_flags)(zp_flags p1);
typedef z_status (void_parser::*type_obj_parser_fp_create)(zp_flags p1,int type);
typedef z_status (void_parser::*type_obj_parser_fp_output)(zp_flags p1,zp_mode mode);

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
extern keyword_item keyword_list[];

#endif

