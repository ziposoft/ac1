#ifndef z_parse_obj_h
#define z_parse_obj_h
#include "zipolib/include/z_factory_static.h"
#include "zipolib/include/z_stl_list.h"
#include "zipolib/include/z_list.h"
#include "zipolib/include/z_stl_vector.h"
#include <map>

ZP_MODULE_DECLARE(parse);

/*
enum type_memvar_oper
{
	mvo_clear,
	mvo_get_size_reset,
	mvo_get_static_var,
	zo_mvo_get,
	zo_mvo_set,
};
*/
class z_parser ;

typedef void* (*zp_create_item_func)();

void* zp_create_item(z_parser& p);


typedef bool (*zp_test_func)(z_parser& p);
/*
struct z_factory_static
{
	z_factory_static* fact;
	ctext parse_string;
};
*/
class zp_parser;



const U8 zp_result_no_match=0;

const U8 zp_result_unknown=0xFD;
const U8 zp_result_eof=0xFE;
const U8 zp_result_extended_value=0xFF;

class zp_test_result
{
	//TODO HANLDE results more than 254
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




	


class zp_pair
{
public:
	zp_pair() 
	{
	}
	zp_pair(ctext name,ctext val) 
	{
		_name=name;
		_val=val;
	}
	z_string _name;
	z_string _val;
	virtual void dump_custom(z_file& outf) 
	{
		outf<<_name<<'='<<_val<<"\n";
	};
};

class zp_str_list 
{
public:
	zp_str_list()
	{
	}
	z_strlist  _list;
};

class zp_value 
{
public:
	zp_value() 
	{
	}

	zp_str_list _string_list;
	int  _integer;
	z_string _string;
};
/*
class void_feature 
{
public:
	void_feature() 
	{
	}
	virtual ctext get_map_key()
	{
		return _name;
	}
	z_string _name;
	zp_value _val;
};
*/
class zp_params 
{
public:
	z_obj_vector<zp_value> _param_list;
};
class zp_feature 
{
public:

	zp_feature() 
	{
		_subscript=false;

	}
	virtual ctext get_map_key()
	{
		return _name;
	}
	z_string _name;
	z_string _subscript_id;
	bool _subscript;
	int get_num_chars();
	ctext get_full_name(z_string& fullname);
};
class zp_cmdline 
{
public:
	zp_cmdline() 
	{
		_root_slash=false;
		_assignment=false;
		_subscript=false;
		_feature=0;
		_assign_val=0;
		_params=0;
	}
	bool _root_slash;
	bool _assignment;
	bool _subscript;

	z_strlist _path_list;
	zp_params* _params;

	z_string _object;
	zp_feature* _feature;
	z_string _subscript_id;
	zp_value* _assign_val;

};

#endif

