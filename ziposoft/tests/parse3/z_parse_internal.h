#ifndef z_parse_internal_h
#define z_parse_internal_h
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_factory.h"
#include "zipolib/include/z_error.h"
#include "zipolib/include/z_stl_vector.h"

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

union zp_mode {
	struct {
	U32 skip_test:1;
	U32 nested_group:1;
	};
	U32 as_u32;
};


//________________________________________________________________
//
//z_zipex_base
//________________________________________________________________
class z_zipex_result_base
{
public:
	z_zipex_result_base() {}
	virtual ~z_zipex_result_base() {}
	virtual void output(z_file* fp);

};
class z_zipex_result_single : public z_zipex_result_base
{
public:
	z_string _data;
	z_zipex_result_single(ctext t) {_data=t;}


};
class z_zipex_result_group : public z_zipex_result_base
{
public:
	z_vector_obj<z_zipex_result_base> _list;


};
class z_zipex_result_group_multi : public z_zipex_result_base
{
public:
	z_vector_obj<z_zipex_result_group> _list;


};

class z_zipex_base
{

	virtual zp_text_parser& tmpl()=0;
	virtual zp_text_parser& data()=0;
	virtual void reset_streams()=0;
	ctext _err_msg;
	ctext _furthest_index;//TODO
	int _groupnum;
	int _result_index;
	zp_flags _flags;
	zp_mode _mode;
	z_string _member_var_name;
	z_status _last_status;

	z_status create_empty_item(void*& p_item_out,ctext item_entry_name);


	z_status _process_stage(zp_mode mode ,zp_flags* pflags=0);
	z_status _process_template();
	z_status _process_group(zp_flags flags,zp_mode mode);
	z_status test_white_space(zp_mode mode);
	z_status _process_sub_item(void* obj,z_factory* ie,zp_mode mode,zp_flags flags);
	z_status _process_sub_obj(ctext start, size_t len,zp_mode mode,zp_flags flags);
	z_status _process_single_item(zp_mode mode,zp_flags flags);
	z_status get_flags(zp_flags& flags);

protected:
	virtual void mark_group_start() {};
	virtual void mark_group_end(z_status result) {};
	virtual void mark_create_string() {};

	z_status relay_error(z_status status)
	{
		return status;
	}

	z_status return_error(z_status status,ctext msg)
	{
		_err_msg=msg;
		_last_status=status;
		return status;
	}



public:
	//String literal
	z_status _f_test_string_literal();

	//NOT String literal
	z_status _f_not_test_string_literal();

	//Simple strings
	z_status _f_test_ident();
	z_status _f_test_to_eob();
	z_status _f_test_whsp();
	z_status _f_test_path();
	z_status _f_digits();
	z_status _f_letters();
	z_status _f_scoped_identchars();



	//ident list
	z_status _f_ident_list_test();

	//quoted strings

	z_status _f_quoted_string_test();
	z_status _f_squoted_string_test();


public:

	z_zipex_base();
	z_status report_error();
	z_status _parse();
	z_status output(z_file* fp);
	ctext get_group(size_t i);
	bool get_group(size_t i, z_string &s);
	void reset();


};


class z_zipex_full : public z_zipex_base
{
public:

	zp_text_parser _templ;
	zp_text_parser _data;
	z_zipex_full()
	{
	}
	z_zipex_full(ctext templ)
	{
		_templ.set_source(templ);
	}
	z_zipex_full(ctext templ,ctext data)
	{
		_templ.set_source(templ);
		_data.set_source(data);
	}
	z_status parse(ctext data)
	{
		_data.set_source(data);
		return _parse();
	}

	virtual zp_text_parser& tmpl() { return _templ;}
	virtual zp_text_parser& data() { return _data;}


	virtual void reset_streams()
	{
		_templ.index_reset();
		_data.index_reset();


	}

};


class z_zipex : public z_zipex_base
{
public:

	zp_text_parser _templ;
	zp_text_parser _data;
	z_zipex()
	{
	}
	z_zipex(ctext templ)
	{
		_templ.set_source(templ);
	}
	z_zipex(ctext templ,ctext data)
	{
		_templ.set_source(templ);
		_data.set_source(data);
	}
	z_status parse(ctext data)
	{
		_data.set_source(data);
		return _parse();
	}

	virtual zp_text_parser& tmpl() { return _templ;}
	virtual zp_text_parser& data() { return _data;}


	virtual void reset_streams()
	{
		_templ.index_reset();
		_data.index_reset();


	}

};


#endif

