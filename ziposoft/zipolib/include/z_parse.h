#ifndef z_parse_h
#define z_parse_h
#include "zipolib/include/z_factory_static.h"
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
		const z_factory* factory=&z_factory_static_T<CLASS>::self;//TODO UG!!!
		return 	parse_obj_f(p_obj, factory,data);

	}
	template <class CLASS> z_status parse_obj_d(CLASS* p_obj,ctext data,ctext tmpl=0)
	{
		const z_factory* factory=&z_factory_T<CLASS>::self;	  //TODO UG!!!
		return 	parse_obj_f(p_obj, factory,data,tmpl);

	}
 	z_status report_error();
	z_status output_default_template(z_file* fp,ctext tmpl);
	z_status output_obj(z_file* fp,const z_factory* factory,void* obj);
	z_status parse_template(ctext tmpl,ctext data);
	z_status parse_obj_f(void* p_obj,const z_factory* factory,ctext data,ctext tmpl=0);


};


ZP_MODULE_DECLARE(parse);
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
		_feature=0;
		_assign_val=0;
		_params=0;
	}
	bool _root_slash;
	bool _assignment;
	z_strlist _path_list;
	zp_params* _params;

	z_string _object;
	zp_feature* _feature;
	zp_value* _assign_val;

};
#endif

