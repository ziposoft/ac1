// test_console.cpp : Defines the entry point for the console application.
//

#include "test_xml.h"
#include "zipolib/include/z_factory_static.h"

#include <vector>

class z_xml_item
{
public:
	virtual  void output(z_file &outfile) {}

} ;

class z_xml_data: public z_xml_item
{
public:
	z_xml_data()
	{}

	z_xml_data(ctext data)
	{
		_data=data;

	}
	z_string _data;
	virtual  void output(z_file &outfile) 
	{
		outfile<<_data;
	}

};

class z_xml_process_instruction: public z_xml_item
{
public:
	z_xml_process_instruction(ctext contents)
	{
		_data=contents;

	}
	z_string _data;
	virtual  void output(z_file &outfile) 
	{
		outfile<<"<?";
		outfile<<_data;
		outfile<<"?>";
	}

};
class z_xml_item_list : public z_obj_vector<z_xml_item> 
{
public:
	virtual void output(z_file &outfile);

} ;
void z_xml_item_list::output(z_file &o)
{
	size_t i;
	for (i=0;i<size();i++)
	{
		get(i)->output(o);


	}
	


}

class z_xml_elm_type
{
public:
	z_xml_elm_type(z_string& name) { _name=name; }
	z_string _name;
	ctext get_map_key() { return _name;}

};
class z_xml_attrib
{
public:
	z_xml_attrib(ctext name,ctext val)
	{
		_name=name;
		_val=val;
	}
	z_string _name;

	z_string _val;
	void output(z_file &o)
	{
		 
		o<< _name << "=\"" << _val<<'\"';

	}

};
class z_xml_elm : public z_xml_item
{
public:
	z_xml_elm()
	{
		_parent=0;
		_type=0;
	}
	z_xml_elm(z_xml_elm_type* type,z_xml_elm* parent)
	{
		_type=type;
		_parent=parent;
	}
	z_xml_elm* _parent;
	z_xml_elm_type* _type;
	z_obj_vector<z_xml_attrib> _attribs;
	z_xml_item_list _tree;

	virtual void output(z_file &outfile);

};
void z_xml_elm::output(z_file &o)
{
	o.indent();
	o<< '<'<< _type->_name;
	//output attributes
	size_t i;
	for (i=0;i<_attribs.size();i++)
	{
		o<<" ";
		_attribs.get(i)->output(o);
		

	}

	if(_tree.size()==0)
	{
		o<<"/>";
		//TODO pretty print

		return;
	}
	o<<">";//TODO pretty print
	//o.indent_inc();
	_tree.output(o);
	//o.indent_dec();
	//o.indent();

	o<<"</"<<_type->_name <<">";//TODO pretty print

}
class z_xml_parser : public z_xml_elm
{
public:
	z_xml_parser()
	{
		_current_node=this;

	}
	z_obj_map<z_xml_elm_type> _elm_types;
	zp_text_parser _p;
	z_file _file;

	z_xml_elm* _current_node;


	void act_dump()
	{
		_tree.output(z_stdout_get());
	}

	virtual z_status parse_file(ctext filename);
	virtual z_status parse();
	virtual z_status process_comment();
	virtual z_status process_element();
	virtual z_status process_instruction();
	virtual z_status process_element_end();
	virtual z_status process_cdata();
	virtual z_status process_data();

};

z_status z_xml_parser::process_instruction()
{
	z_status status;
	status=_p.test_end_string("?>");
	if(status==zs_matched)
	{
		return status;
	}

	return Z_ERROR_MSG(zs_syntax_error,"Error parsing process instruction");

}
z_status z_xml_parser::process_comment()
{

	return Z_ERROR_NOT_IMPLEMENTED;

}
z_status z_xml_parser::process_data()
{
	z_status status=_p.test_not_char('<');

	if(status==zs_matched)
	{
		z_xml_data* d=new z_xml_data();

		_p.get_match(d->_data);

		_current_node->_tree << d;
	}
	return status;;

}
z_status z_xml_parser::process_cdata()
{

	return Z_ERROR_NOT_IMPLEMENTED;

}
z_status z_xml_parser::process_element_end()
{
	if(!_current_node)
		return Z_ERROR(zs_internal_error);
	if(!_current_node->_type)
		return Z_ERROR_MSG(zs_syntax_error,"Unexpected end tag");
	z_string &elm_name=_current_node->_type->_name;
	z_status status=_p.test_identifier(elm_name);
	if(status)
	{
		return Z_ERROR_MSG(zs_syntax_error,"Expected \"</%s>\" XML end tag",elm_name.c_str());
	}
	_p.skip_ws();
	status=_p.test_char('>');
	if(status==zs_ok)
	{
		_current_node=_current_node->_parent;

		return zs_ok;
	}

	return status;


}
z_status z_xml_parser::process_element()
{
	z_status status=zs_ok;


	z_string elm_name;
	_p.get_match(elm_name);
	z_xml_elm_type* elm_type=_elm_types.get(elm_name);
	if(!elm_type)
	{
		elm_type= z_new z_xml_elm_type(elm_name);
		_elm_types << elm_type;
	}
	z_xml_elm* elm = z_new z_xml_elm(elm_type,_current_node);
	_current_node->_tree<< elm;
	_current_node=elm;
	while(1)
	{
		_p.skip_ws();
		status=_p.test_string("/>");
		if(status==zs_ok)
		{
			_current_node=_current_node->_parent;
			Z_ASSERT(_current_node);
			if(!_current_node)
				return Z_ERROR_MSG(zs_syntax_error,"Unexpected XML element end tag");
			//check valid node
			return zs_ok;
		}
		
		//process attributes
		status=_p.test_any_identifier_scoped();
		if(status==zs_ok)
		{
			z_string name,val;
			_p.get_match(name);	
			_p.skip_ws();
			status=_p.test_char('=');
			
			if(status)
			{
				return Z_ERROR_MSG(zs_syntax_error,"Missing attribute value");
			}
			_p.skip_ws();

			status=_p.test_code_string();
			if(status)
			if(status)
			{
				return Z_ERROR_MSG(zs_syntax_error,"Missing attribute value");
			}			
			_p.get_match(val);
			z_xml_attrib* a=z_new z_xml_attrib(name,val);
			_current_node->_attribs<<a;


		}
		status=_p.test_char('>');
		if(status==zs_ok)
			return zs_ok;



	}


	return Z_ERROR_NOT_IMPLEMENTED;

}
z_status z_xml_parser::parse_file(ctext filename)
{
	z_status st=_file.open( filename,"rb");
	if(st)
		return st;
	char* data;
	size_t size;
	_file.read_all(data,size);//TODO make this mapping instead.
	_p.set_source(data,size);
	return parse();



}



z_status z_xml_parser::parse()
{
	z_status status=zs_ok;
	while(status==zs_ok)
	{
		status=_p.test_char(	'<');
		if(status==zs_matched)
		{
	  		status=_p.test_char(	'?');
			if(status==zs_matched)
			{
				status=	process_instruction();
				continue;
			}
			status=_p.test_char(	'/');
			if(status==zs_matched)
			{
				status=	process_element_end();
				continue;
			}
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
			status=process_data();

	}
	if(status==zs_eof)
		status=zs_ok;
	if(status)
	{
		_p.print_context();
	}
	return status;
}

class root
{
public:
	root()
	{
		_p_logger=&z_logger_get();
		_param_xml_filename="test.xml";


	}
	z_console console;
	z_logger* _p_logger;
	z_string _param_xml_filename;
	z_status act_parse();
	z_status act_dump();
	z_status act_dump_types();
	z_xml_parser _parser;
};

z_status root::act_parse()
{
	z_status status;
	status= _parser.parse_file(_param_xml_filename);
	 return status;

}
z_status root::act_dump()
{
	z_status status;
		_parser.dump();
	 return status;
}
z_status root::act_dump_types()
{
	z_status status;
		_parser.dump();
	 return status;
}


ZFACT(root)
{
	ZOBJ(console);
	ZOBJ(_parser,"p",ZFF_PROP,"parser");
	ZPOBJ(_p_logger,"log",ZFF_PROP,"Logger");
	//ZPROP_X(_param_xml_filename,"filename",ZFF_PROP ,"  ") ;
	ZACT_XP(act_dump_types,"types",ZFF_ACT_DEF,"types",0);
	ZACT_XP(act_dump,"dump",ZFF_ACT_DEF,"dump",0);
	ZACT_XP(act_parse,"parse",ZFF_ACT_DEF,"parse",1,
		ZPARAM_X(_param_xml_filename,"filename",ZFF_PARAM,"Name of file"));

};
int main(int argc, char* argv[])
{

	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);

	return 0;
}





ZP_MODULE_INCLUDE(  ZP_MOD(logger));
