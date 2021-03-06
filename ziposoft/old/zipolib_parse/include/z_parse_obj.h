#ifndef z_parse_obj_h
#define z_parse_obj_h
#include "zipolib_parse/include/zp_obj.h"

Z_MODULE_DECLARE(parse);

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
class zp_obj;
class z_parser ;

typedef zp_obj* (*zp_create_item_func)();

zp_obj* zp_create_item(z_parser& p);


typedef bool (*zp_test_func)(z_parser& p);
/*
struct z_obj_fact
{
	z_obj_fact* fact;
	ctext parse_string;
};
*/
class zp_obj_parser;



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

class zp_obj_vect_base : public z_obj_vect<zp_obj>
{
};
class zp_obj : public zp_obj_base
{
	zp_obj_vect_base* _children;
//	const z_obj_fact* _entry;
public:
	size_t _templ_offset;
//	zp_obj(const z_obj_fact* entry);
	zp_obj();
	~zp_obj();
	virtual void output(z_file* fp);
	virtual ctext get_parse_string();
	virtual void add_child(zp_obj* item);
	zp_obj* get_child(int index);
	zp_obj* get_child_by_offset(int &index,size_t off,int si);

	void dump(int level,z_file& outf,int flags);
	virtual ctext get_map_key();
	virtual ctext get_template();
	int compare_id(ctext id)
	{
		return z_str_same(id,get_map_key());
	}
	virtual void dump_custom(z_file& outf) {};
};
class zp_obj_generic : public zp_obj
{
public:
	ZO_OBJ_H;
	zp_obj_generic(ctext t) 
	{
		parse_string=t;
	}
	zp_obj_generic()
	{
		parse_string=0;

	}
	ctext parse_string;
	virtual ctext get_parse_string() { return parse_string; }



};
class zp_text : public  zp_obj
{
	z_string _text;
public:
	int _type;

	virtual void output(z_file* fp);
	zp_text(ctext todo);
	zp_text();
	~zp_text();

	virtual ctext get_map_key();
	void set_text(ctext text,size_t len=0);
	virtual ctext get_text();
	//virtual const z_obj_fact* get_fact(){ return &entry_item_text; }
	virtual void dump(int level,z_file& outf,int flags);
};



/*
#define ZP_OBJ(_X_,...) 


#define PARSE_TABLE(_x_) &_x_[0],sizeof(_x_)/sizeof(z_obj_fact*)

#define ZP_MNULL  ""

#define ZPV(_X_)   0;if(var_name==0) p->feature_callback(oper,0,_X_);if(var_name&&z_str_same(#_X_,var_name))  return	p->feature_callback(oper, p_size,_X_);;0


#define ZP_OBJ(_CLASS_,_BASE_,_NAME_,_PARSE_,...) \
zp_obj* new_##_CLASS_() {\
	_CLASS_* obj=new  _CLASS_(); \
	return obj; }\
ctext _CLASS_##PARSE=_PARSE_;\
ctext _CLASS_##NAME=_NAME_;\
z_status _CLASS_::feature_manipulate(type_memvar_oper oper,size_t* p_size,ctext var_name,zp_obj_parser* p) {\
   __VA_ARGS__ ;	return _BASE_::feature_manipulate(oper,p_size,var_name,p);} \
const z_obj_fact  _CLASS_::ENTRY=  {_CLASS_##NAME,_CLASS_##PARSE,new_##_CLASS_};\
	const z_obj_fact* _CLASS_::get_fact() { return &_CLASS_::ENTRY;} 

#define ZP_OBJ_H \
	public:virtual z_status feature_manipulate(type_memvar_oper set,size_t* i,ctext var_name,zp_obj_parser* p); \
	static const z_obj_fact ENTRY;\
	virtual const z_obj_fact* get_fact();

#define ZP_LISTOBJ(_CLASS_) &_CLASS_::ENTRY





*/

#define ZP_OBJ_H ZO_OBJ_H;


//________________________________________________________________
//
//zp_pair
//________________________________________________________________





class zp_pair : public zp_obj
{
public:
	ZP_OBJ_H
	zp_pair() : zp_obj()
	{
	}
	zp_pair(ctext name,ctext val) : zp_obj()
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
class zp_obj_feature;
class zp_obj_parse : public zp_obj
{
public:
	ZP_OBJ_H
	zp_obj_parse() : zp_obj()
	{
	}

	z_obj_map<zp_obj_feature> _fet_list;
	z_obj_vect<zp_obj_parse> _child_list;

	z_string _name;
};
class zp_str_list : public zp_obj
{
public:
	ZP_OBJ_H
	zp_str_list() : zp_obj()
	{
	}
	zo_str_vect  _list;
};

class zp_value : public zp_obj
{
public:
	ZP_OBJ_H
	zp_value() : zp_obj()
	{
	}

	zp_obj_parse _obj;
	z_obj_vect<zp_obj_parse> _child_list;
	zp_str_list _string_list;
	int  _integer;
	z_string _string;
};

class zp_obj_feature : public zp_obj
{
public:
	ZP_OBJ_H
	zp_obj_feature() : zp_obj()
	{
	}
	virtual ctext get_map_key()
	{
		return _name;
	}
	z_string _name;
	zp_value _val;
};
class zp_params : public zp_obj
{
public:
	ZP_OBJ_H
	zp_params() : zp_obj()
	{
	}
	z_obj_vect<zp_value> _param_list;
};
class zp_feature : public zp_obj
{
public:
	ZP_OBJ_H

	zp_feature() : zp_obj()
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
class zp_cmdline : public zp_obj
{
public:
	ZP_OBJ_H
	zp_cmdline() : zp_obj()
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
#endif

