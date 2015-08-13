#ifndef z_parse_text_h
#define z_parse_text_h
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_string.h"
#include "zipolib/include/ptypes_cset.h"




enum item_type {
	item_invalid_type,
	item_literal,
	item_integer,
	item_letters,
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

/*________________________________________________________________________

				zp_text_parser
________________________________________________________________________*/

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
		 U32 count_lines:1;
	};
	U32 as_u32;
};
class zp_text_parser 
{
	const char* _start;
	const char* _end;
	const char* _index_current;
	const char* _match_start;
	const char* _match_end;
	int n_newlines;
	zpt_options _options;
	//internal functions
	z_status start_test();


	//internal stream tests
	z_status _test_char(char c);
	z_status _test_end_char(char c); //  ~>(char) 
protected:
	const char* _index_under_test;
public:
	/*______________________________________

				initialization
	  ______________________________________*/
	zp_text_parser();
	zp_text_parser(const char* code,size_t len=(size_t)-1);
	void set_source(const char* code,size_t len=(size_t)-1);
	void set_option_ignore_whitespace();
	void set_option_count_lines();

	/*______________________________________

				Tests
	  ______________________________________*/
	z_status test_single_quoted_string(); //  ~>(char) 
	z_status test_not_single_quoted_string(const void* dummy); //  ~>(char) 
	z_status test_end_char(char c); //  ~>(char) 
	z_status test_not_string(const char* str,size_t len);
	z_status test_end_string(const char* str);
	z_status test_string(const char* str,size_t len);
	z_status test_string(const char* str);
	z_status test_not_char(char c); //  ~^(char)  ~^\n
	z_status test_any_chars(size_t n);// #5
	z_status test_chars(const cset &set);
	z_status test_identifier(const char* str);
	z_status test_any_identifier();
	z_status test_any_identifier_scoped();
	z_status test_integer();
	z_status test_file_path();
	z_status test_code_string();
	z_status test_cset(const cset &set);
	z_status test_char(char c);
	z_status test_to_eob();	
	/*______________________________________

				Test function wrappers
	  ______________________________________*/
	z_status ft_test_identifier(const void* str);
	z_status ft_single_quoted_string(const void* dummy);
	z_status ft_digits(const void* dummy);
	z_status ft_letters(const void* dummy);
	
	z_status ft_file_path(const void* dummy);
	virtual z_status ft_any_identifier(const void* dummy);
	z_status ft_scoped_identchars(const void* dummy);
	z_status ft_to_eol(const void* dummy);
	z_status ft_test_char(const void* c);
	/*______________________________________

				Index functions
	  ______________________________________*/
	inline ctext get_index() { return _index_current;}
	void  set_index(ctext in) {  _index_current=in;}
	inline void set_index_under_test(ctext in) {  _index_under_test=in;}
	inline ctext get_index_under_test() {  return _index_under_test;}
	inline char get_char_under_test() {  return *(_index_under_test);}
	void index_reset();
	char inc();
	virtual z_status advance(size_t count);
	size_t get_index_offset();
	bool eob(char* i);
	z_status _peek_next_char(char c);
	bool eob();
	inline ctext get_buffer() { return _start;}
 	/*______________________________________

				Retrieving Data
	  ______________________________________*/
	char current_ch();

	void get_match(ctext& match_start,size_t& len);
	void get_match(z_string& s);
  	/*______________________________________

				Utility
	  ______________________________________*/
	z_status skip_ws();
	size_t debug(z_string &out,ctext pbuff=0,size_t before=0,size_t after=9 );
	void print_context();

	z_status check_status(z_status status);





	/*
	z_status test_white_space();
	z_status test_new_line();
	*/

	//stream util
	//ctext get_index_skip_ignored_chars();
//	bool teob();


	

};
typedef z_status (zp_text_parser::*type_txt_parser_fp)(const void* p1);


#endif

