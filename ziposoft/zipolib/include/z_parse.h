#ifndef z_parse_h
#define z_parse_h
#include "zipolib/include/z_factory_static.h"

ZP_MODULE_DECLARE(parse);
/*
Stage Operators:
	& = AMD
	| = OR
	: = Sequence
	% = random stage

Item Modifiers
	+ = one or more
	^ = NOT
	* = zero or more
	? = zero or one
	{var} = Input Variable

Items
	'a'  = literal
	^'a' = not literal
Item Keyword
	string = 
	string_sq =
	ident =
	whsp = whitespcace 
	scoped = 
	toeob = 


















*/


class zp_parser;
class z_parser
{
	zp_parser* _p;
public:
	z_parser();
	virtual ~z_parser();
	//TODO UG!!!
	template <class CLASS> z_status parse_obj(CLASS* p_obj,ctext data,ctext tmpl=0)
	{
		z_factory* factory=&z_factory_T<CLASS>::self;//TODO UG!!!
		return 	parse_obj_f(p_obj, factory,data);

	}
	template <class CLASS> z_status parse_obj_d(CLASS* p_obj,ctext data,ctext tmpl=0)
	{
		z_factory* factory=&z_factory_T<CLASS>::self;	  //TODO UG!!!
		return 	parse_obj_f(p_obj, factory,data,tmpl);

	}
 	z_status report_error();
	z_status output_default_template(z_file* fp,ctext tmpl);
	z_status output_obj(z_file* fp,z_factory* factory,void* obj);
	z_status parse_template(ctext tmpl,ctext data);
	z_status parse_obj_f(void* p_obj,z_factory* factory,ctext data,ctext tmpl=0);


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


class zp_subscript 
{
public:
	z_string _id;
};


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
		_sub=0;

	}
	virtual ctext get_map_key()
	{
		return _name;
	}
	z_string _name;
	zp_subscript* _sub;

	//int get_num_chars();
	ctext get_full_name(z_string& fullname)
	{
		fullname=_name;
		if(_sub)
		{
			fullname<<'['<<_sub->_id<<']';
		}
		return fullname.c_str();

	}
};
class zp_path 
{
public:
	zp_path() 
	{
	}

};

class zp_cmdline 
{
public:
	zp_cmdline() 
	{
		_assignment=false;
		//_path=0;
		_assign_val=0;
		_params=0;
		_root_slash=false;
	}
	bool _assignment;
	//zp_path* _path;
	zp_params* _params;

	zp_value* _assign_val;
	bool _root_slash;
	z_obj_vector_map<zp_feature> _path;
	zp_feature* get_feature();


	bool has_path();
};

class zp_cfg_file
{
public:
	zp_cfg_file() 
	{
	}
	 z_status load_obj(void* obj, z_factory* f)
	{
		ctext name=f->get_name();
		zp_cfg_obj* cfg=_obj.get_by_key(name);
		if(cfg)
		{
			cfg->load_obj(obj,f);
		}
		return zs_ok;

	}
	template <class C> z_status load_obj(C* obj)
	{
		z_factory* f=&z_factory_T<C>::self;
		ctext name=f->get_name();
		zp_cfg_obj* cfg=_obj.get_by_key(name);
		if(cfg)
		{
			cfg->load_obj(obj,f);
		}
		return zs_ok;

	}
	z_obj_vector_map<zp_cfg_obj> _obj;
};
#endif

