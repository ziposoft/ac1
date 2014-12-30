// test_console.cpp : Defines the entry point for the console application.
//

#include "test_xml.h"
#include "zipolib/include/z_factory_static.h"

#include <vector>

class z_xml_item
{
public:

} ;
class z_xml_item_list : public std::vector<z_xml_item*> 
{
public:

} ;


class z_xml_elm_type
{
public:
	z_string _name;

};

class z_xml_parser
{
public:
	z_obj_map<z_xml_elm_type> _elm_types;
	zp_text_parser _p;
	z_file _file;
	virtual z_status parse();
	virtual z_status process_comment();
	virtual z_status process_element();
	virtual z_status process_cdata();
	virtual z_status process_data();

};





z_status z_xml_parser::parse()
{
	z_status status=zs_ok;
	while(status==zs_ok)
	{
		_p.skip_ws();
		status=_p.test_char(	'<');
		if(status==zs_matched)
		{
	  		status=_p.test_char(	'!');
			if(status==zs_matched)
			{
				status=_p.test_string("--");
				if(status==zs_matched)
				{
					status=	process_comment();
					continue;
				}
				status=_p.test_string("CDATA");
 				if(status==zs_matched)
				{
					status=	process_cdata();
					continue;
				}
			}
			status=_p.test_any_identifier();
			if(status==zs_matched)
			{
 				status=	process_element();
				continue;
			}
			break;
		}
		if(	status==zs_no_match)  
			process_data();

	}
	return status;
}
class z_xml_elm
{
public:


};

class z_xml_attrib
{
public:


};
class z_xml_process_instruction
{
public:


};
class root
{
public:
	root()
	{
		_p_logger=&z_logger_get();


	}
	z_console console;
	z_logger* _p_logger;
	z_string _param_xml_filename;
	z_status act_parse();
};

z_status root::act_parse()
{



}


ZFACT(root)
{
	ZOBJ(console);
	ZPOBJ(_p_logger,"log",ZFF_PROP,"Logger");
	ZPROP_X(_param_xml_filename,"filename",ZFF_PROP ,"  ") ;
	ZACT_XP(act_parse,"parse",ZFF_ACT_DEF,"parse",1,
		ZPARAM_X(_param_xml_filename,"filename",ZFF_PROP,"Name of file"));

};
int main(int argc, char* argv[])
{

	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);

	return 0;
}





ZP_MODULE_INCLUDE(  ZP_MOD(logger));
