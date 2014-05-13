#ifndef z_parse_obj_h
#define z_parse_obj_h
#include "zipolib_parse2/include/zp_obj.h"

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
struct zp_factory
{
	zp_factory* fact;
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




/*
#define void(_X_,...) 


#define PARSE_TABLE(_x_) &_x_[0],sizeof(_x_)/sizeof(zp_factory*)

#define ZP_MNULL  ""

#define ZPV(_X_)   0;if(var_name==0) p->feature_callback(oper,0,_X_);if(var_name&&z_str_same(#_X_,var_name))  return	p->feature_callback(oper, p_size,_X_);;0


#define void(_CLASS_,_BASE_,_NAME_,_PARSE_,...) \
void* new_##_CLASS_() {\
	_CLASS_* obj=new  _CLASS_(); \
	return obj; }\
ctext _CLASS_##PARSE=_PARSE_;\
ctext _CLASS_##NAME=_NAME_;\
z_status _CLASS_::feature_manipulate(type_memvar_oper oper,size_t* p_size,ctext var_name,zp_parser* p) {\
   __VA_ARGS__ ;	return _BASE_::feature_manipulate(oper,p_size,var_name,p);} \
const zp_factory  _CLASS_::ENTRY=  {_CLASS_##NAME,_CLASS_##PARSE,new_##_CLASS_};\
	const zp_factory* _CLASS_::get_fact() { return &_CLASS_::ENTRY;} 

#define void_H \
	public:virtual z_status feature_manipulate(type_memvar_oper set,size_t* i,ctext var_name,zp_parser* p); \
	static const zp_factory ENTRY;\
	virtual const zp_factory* get_fact();

#define ZP_LISTOBJ(_CLASS_) &_CLASS_::ENTRY





*/

#define void_H ZO_OBJ_H;


//________________________________________________________________
//
//zp_pair
//________________________________________________________________


	
#if 0

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
class void_feature;
class void_parse : public void
{
public:
	void_parse() 
	{
	}

	z_obj_map<void_feature> _fet_list;
	z_obj_vect<void_parse> _child_list;

	z_string _name;
};
class zp_str_list 
{
public:
	zp_str_list()
	{
	}
	zo_str_vect  _list;
};

class zp_value : public void
{
public:
	void_H
	zp_value() : void()
	{
	}

	void_parse _obj;
	z_obj_vect<void_parse> _child_list;
	zp_str_list _string_list;
	int  _integer;
	z_string _string;
};

class void_feature : public void
{
public:
	void_H
	void_feature() : void()
	{
	}
	virtual ctext get_map_key()
	{
		return _name;
	}
	z_string _name;
	zp_value _val;
};
class zp_params : public void
{
public:
	void_H
	zp_params() : void()
	{
	}
	z_obj_vect<zp_value> _param_list;
};
class zp_feature : public void
{
public:
	void_H

	zp_feature() : void()
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
class zp_cmdline : public void
{
public:
	void_H
	zp_cmdline() : void()
	{
		_root_slash=false;
		_assignment=false;
		_subscript=false;
	}
	bool _root_slash;
	bool _assignment;
	bool _subscript;

	zo_str_vect _path_list;
	zp_params _params;

	z_string _object;
	zp_feature _feature;
	z_string _subscript_id;
	zp_value _assign_val;

};

*/
#endif
#endif

