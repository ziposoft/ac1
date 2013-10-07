#ifndef z_parse_text_h
#define z_parse_text_h
#include "zipolib_parse/include/z_parse_pch.h"
enum zp_status
{
	zs_ok=0,
	zs_matched=0,
	zs_no_match,
	zs_skipped,
	zs_template_syntax_error,
	zs_unparsed_data,
	zs_fatal_error,
	zs_eof,
	zs_feature_not_found,
};

/*
const cset cset_stringcontrol("\"\\");
const cset cset_stringdata = !cset_stringcontrol;
const cset cset_whitespace(" \t\r");
const cset cset_letters("_A-Za-z");
const cset cset_digits("0-9");
const cset cset_hex_digits("0-9A-Fa-f");
const cset cset_float("0-9.");
const cset cset_misc(":;,&%<*>?().=+!~~[]|#/");
const cset cset_modifiers("*&");
const cset cset_identchars = cset_letters + cset_digits;
const cset cset_path_string = cset_letters + cset_digits+"\\/.:"+'-';

const cset cset_filename = cset_letters + cset_digits+'.';
const cset cset_otherchars = !cset_letters;

const cset z_parser::cset_letters("_A-Za-z");
const cset z_parser::cset_identchars(cset_letters);

*/

enum item_type {
	item_invalid_type,
	item_literal,
	item_integer,
	item_identifier,
	item_whsp,
	item_filepath,
	item_scoped,
	item_toeol,
	item_sub_group,
	item_sub_obj,
	item_quoted_double,
	item_quoted_single,
};



//________________________________________________________________
//
//zp_text_parser
//________________________________________________________________
extern cset *cset_identchars;
extern cset *cset_letters;
extern cset *cset_digits;
extern cset *cset_white_space;
extern cset *cset_path_string;
extern cset *cset_scoped_identchars;
union zpt_options {
	struct {
		 U32 ignore_tabs:1;
		 U32 ignore_space:1;
		 U32 ignore_newline:1;
	};
	U32 as_u32;
};
class zp_text_parser 
{
	const char* _start;
	const char* _end;
	const char* _index_current;
	const char* _index_under_test;
	//const char* _index_furthest;
	const char* _match_start;
	const char* _match_end;
	int n_newlines;
	zpt_options _options;
	//internal funcitons
	zp_status start_test();


	//internal stream tests
	zp_status _test_char(char c);
	zp_status _test_end_char(char c); //  ~>(char) 
public:
	//initialization
	zp_text_parser();
	zp_text_parser(const char* code,size_t len=(size_t)-1);
	void set_source(const char* code,size_t len=(size_t)-1);
	void set_ignore_whitespace();
	//stream test func pointers

	//stream tests
	zp_status test_single_quoted_string(); //  ~>(char) 

	zp_status test_end_char(char c); //  ~>(char) 
	zp_status test_not_string(const char* str);
	zp_status test_string(const char* str,size_t len);
	zp_status test_string(const char* str);

	zp_status ft_test_identifier(const void* str);
	zp_status ft_single_quoted_string(const void* dummy);
	zp_status ft_digits(const void* dummy);
	zp_status ft_file_path(const void* dummy);
	virtual zp_status ft_any_identifier(const void* dummy);
	zp_status ft_scoped_identchars(const void* dummy);
	zp_status ft_to_eol(const void* dummy);
	zp_status ft_test_char(const void* c);
	zp_status test_not_single_quoted_string(const void* dummy); //  ~>(char) 




	zp_status test_not_char(char c); //  ~^(char)  ~^\n
	zp_status test_any_chars(size_t n);// #5
	zp_status test_chars(const cset &set);
	zp_status test_identifier(const char* str);
	zp_status test_any_identifier();
	zp_status test_file_path();
	zp_status test_code_string();
	zp_status test_cset(const cset &set,size_t limit=0xFFFFFFFF);
	zp_status test_char(char c);
	zp_status test_to_eob();
	/*
	zp_status test_white_space();
	zp_status test_new_line();
	*/

	//stream util
	void index_reset();
	size_t get_index_offset();
	inline ctext get_index() { return _index_current;}
	inline void set_index_under_test(ctext in) {  _index_under_test=in;}
	inline ctext get_index_under_test() {  return _index_under_test;}
	inline char get_char_under_test() {  return *(_index_under_test);}
	inline ctext get_buffer() { return _start;}
	void  set_index(ctext in) {  _index_current=in;}
	char inc();
	zp_status advance(size_t count);
	bool eob(char* i);
	bool eob();
//	bool teob();
	char current_ch();
	void get_match(ctext& match_start,size_t& len);
	void get_match(z_string& s);

	zp_status skip_ws();
	size_t debug(z_string &out,ctext pbuff=0,size_t before=0,size_t after=9 );

	void print_status();
	void print_context();

	zp_status check_status(zp_status status);
	

};
typedef zp_status (zp_text_parser::*type_txt_parser_fp)(const void* p1);


#endif

