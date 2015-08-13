// test_console.cpp : Defines the entry point for the console application.
//

#include "parse_xml.h"



void z_xml_item_list::output(z_file &o)
{
	size_t i;
	for (i=0;i<size();i++)
	{
		get(i)->output(o);


	}
	


}

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

z_status z_xml_parser::process_instruction()
{
	z_status status;
	status=_p.test_end_string("?>");
	if(status==zs_matched)
	{
		z_string data;
		_p.get_match(data);
		data.resize(data.size()-2);
		z_xml_process_instruction* d=new z_xml_process_instruction(data);


		_current_node->add_child_item( d);
		return status;
	}

	return Z_ERROR_MSG(zs_syntax_error,"Error parsing process instruction");

}

void z_xml_parser::output(z_file &outfile)
{

		_tree.output(outfile);

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

		_current_node->add_child_item( d);
	}
	return status;;

}
z_status z_xml_parser::process_cdata()
{
	z_status status=_p.test_end_string("]]>");

	
	return status;;

}



z_status z_xml_parser::process_element_end()
{
	if(!_current_node)
		return Z_ERROR(zs_internal_error);
	if(!_current_node->get_type())
		return Z_ERROR_MSG(zs_syntax_error,"Unexpected end tag");
	z_string &elm_name=_current_node->get_type()->_name;
	z_status status=_p.test_identifier(elm_name);
	if(status)
	{
		return Z_ERROR_MSG(zs_syntax_error,"Expected \"</%s>\" XML end tag",elm_name.c_str());
	}
	_p.skip_ws();
	status=_p.test_char('>');
	if(status==zs_ok)
	{
		_current_node=_current_node->get_parent();

		return zs_ok;
	}

	return status;


}
z_xml_elm* z_xml_elm_type::create_element(z_xml_elm* parent)
{
	z_xml_elm* elm = z_new z_xml_elm(this,parent);

	return elm;
}

void z_xml_elm_type_list::add_type(z_xml_elm_type* t)
{
	z_xml_elm_type* e=get(t->_name);
	if(!e)
	{
		add(t->_name,t);
		return;
	}
	if(e!=t)
	{

		Z_ERROR_MSG(zs_wrong_object_type,"Elm type \"%s\" already is registered, but with different pointer.",t->_name.c_str());

	}
	
}


z_xml_elm_type* z_xml_elm_type_list::get_or_create_type(ctext elm_name,z_xml_elm_type* parent)
{
	z_xml_elm_type* elm_type=get(elm_name);

	if(!elm_type)
	{
		elm_type= z_new z_xml_elm_type(elm_name,parent);
		*this << elm_type;
	}
	if(parent)
		parent->_children.add_type(elm_type);
	return elm_type;
}


z_status z_xml_parser::process_element()
{
	z_status status=zs_ok;


	z_string elm_name;
	_p.get_match(elm_name);
	z_xml_elm_type* elm_type=_elm_types.get_or_create_type(elm_name,_current_node->get_type());
	elm_type->_count++;


	z_xml_elm* elm = elm_type->create_element(_current_node);
	if(!_root_node)
		_root_node=elm;

	
	_current_node->add_child_item(elm);
	_current_node=elm;
	while(1)
	{
		_p.skip_ws();
		status=_p.test_string("/>");
		if(status==zs_ok)
		{
			_current_node=_current_node->get_parent();
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
				status=_p.test_single_quoted_string();
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
	{
		return Z_ERROR_MSG(st,"Could not open file \"%s\"",filename);
	}
	char* data=0;
	size_t size;
	_file.read_all(data,size);//TODO make this mapping instead.
	_p.set_source(data,size);
	z_status status= parse();

	if(data)
		free( data);
	return status;

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
				status=_p.test_string("[CDATA");
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
ZFACT(z_xml_parser)
{
	ZACT_XP(act_dump_types,"types",ZFF_ACT_DEF,"types",0);
	ZACT_XP(act_dump_types_tree,"typetree",ZFF_ACT_DEF,"typetree",0);

};

