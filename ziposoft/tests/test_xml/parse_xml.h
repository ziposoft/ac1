// test_console.cpp : Defines the entry point for the console application.
//
#ifndef z_parse_xml_h
#define z_parse_xml_h
#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_factory_static.h"

#include <vector>
 ZP_MODULE_DECLARE(parser_xml);


class z_xml_elm;

class z_xml_item
{
public:
	virtual  void output(z_file &outfile) {}
	virtual  z_xml_elm* match_element(ctext name) { return 0;}
	virtual void get_data(z_string &data) { }
	virtual ~z_xml_item()
	{
	}
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
	virtual void get_data(z_string &data) { data +=_data;}

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
	z_status remove_by_pointer(z_xml_item* item_to_remove)
	{
		size_t i;
		for(i=0;i<size();i++)
		{
			z_xml_item* x=get(i);
			if(item_to_remove==x)
			{
				remove(i);
				return zs_ok;
			}


		}
		return zs_item_not_found;



	}

} ;
class z_xml_elm;
class z_xml_elm_type;
class z_xml_elm_type_list : public z_obj_map<z_xml_elm_type>
{
public:
	z_xml_elm_type* get_or_create_type(ctext name,z_xml_elm_type* parent);
	void add_type(z_xml_elm_type* t);

} ;
class z_xml_elm_type
{
public:
	z_xml_elm_type(ctext name,z_xml_elm_type* parent) 
	{ 
		_name=name; _count=0;
		if(parent)
			_parents.add_type(parent);
	}
	z_string _name;
	ctext get_map_key() { return _name;}
	int _count;
	z_xml_elm_type_list _parents;
	z_xml_elm_type_list _children;
	z_xml_elm_type* add_child(z_xml_elm_type* t);
	z_status dump_types_tree(z_file &out)
	{
		zout.indent();
		zout << _name <<'('<<_count <<")\n";
		
		z_map_iter i;
		out.indent_inc();

		z_xml_elm_type* t;
		while(t=_children.get_next(i))
		{
			t->dump_types_tree(out);

		}
		out.indent_dec();

		 return zs_ok;
	}

	virtual z_xml_elm* create_element(z_xml_elm* parent);
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
	ctext get_map_key() { return _name;}

};

class z_xml_elm : public z_xml_item
{
protected:
	z_xml_elm* _parent;
	z_xml_elm_type* _type;
	z_xml_item_list _tree;
public:
	z_obj_vector_map<z_xml_attrib> _attribs;
	z_xml_elm()
	{
		_parent=0;
		_type=0;
	}

	void  add_child_item(z_xml_item* item) { _tree << item;}

	z_xml_elm_type* get_type() { return _type; }
	z_xml_elm* get_parent() { return _parent; }
	z_status remove_child(z_xml_item* item)
	{

		return _tree.remove_by_pointer(item);
	}

	z_status move_to_new_parent(z_xml_elm* newparent /* if 0, then make it an orphan */)
	{
		z_status status=zs_ok;
		if(_parent)
			status=_parent->remove_child(this);
		if(newparent)
			newparent->add_child_item(this);
		_parent=newparent;
		return status;
	}

	z_xml_elm(z_xml_elm_type* type,z_xml_elm* parent)
	{
		_type=type;
		_parent=parent;
	}
	virtual ~z_xml_elm()
	{
		_attribs.destroy();
		_tree.destroy();
	}
	void init(z_xml_elm_type* type,z_xml_elm* parent)
	{
		_type=type;
		_parent=parent;
	}

	ctext get_attrib(ctext var)
	{
		z_xml_attrib* a=_attribs.get_by_key(var);
		if(a)
			return a->_val;
		return 0;
	}
	z_xml_elm* get_first_elm(ctext type)
	{
		size_t cursor=0;
		z_xml_elm* elm=0;
		while(!elm)
		{
			if(_tree.size()<=cursor) 
				return 0;
			elm=_tree[cursor]->match_element(type);
			cursor++;
		}
		return elm;
	}
	virtual void get_data(z_string &data) 
	{ 
		size_t i;
		for(i=0;i<_tree.size();i++)
			_tree[i]->get_data(data);
	}


	z_xml_elm* get_next_elm(size_t& cursor,ctext type)
	{
		z_xml_elm* elm=0;
		while(!elm)
		{
			if(_tree.size()<=cursor) 
				return 0;
			elm=_tree[cursor]->match_element(type);
			cursor++;
		}
		return elm;
	}
	z_xml_elm* get_next_elm_tree(size_t& cursor,ctext type)
	{
		while(1)
		{
			if(_tree.size()<cursor) 
				return 0;
			if(_tree.size()==cursor) 
				return 0;
			z_xml_elm* elm=match_element(type);
		}
	}

	virtual void output(z_file &outfile);
	virtual  z_xml_elm* match_element(ctext name) 
	{ 
		if(_type->_name == name) 
			return this;
		return 0;
	}

};

class z_xml_parser : public z_xml_elm
{
public:
	z_xml_parser()
	{
		_current_node=this;
		_root_node=0;

	}
	void reset()
	{
		_current_node=this;
		_tree.destroy();
		_root_node=0;


	}
	z_xml_elm_type_list _elm_types;
	zp_text_parser _p;
	z_file _file;

	z_xml_elm* _current_node;
	z_xml_elm* _root_node;
	z_status act_dump_types_tree()
	{
		zout.indent_reset();
		_root_node->get_type()->dump_types_tree(zout);
		
		 return zs_ok;
	}
	z_status act_dump_types()
	{
		z_map_iter i;
		z_xml_elm_type* t;
		while(t=_elm_types.get_next(i))
		{
			zout << t->_name <<'('<<t->_count <<")\n";

		}
		 return zs_ok;
	}
	void act_dump()
	{
		_tree.output(z_stdout_get());
	}

	virtual z_status parse_file(ctext filename);//,z_xml_elm* &root);
	virtual z_status parse();
	virtual z_status process_comment();
	virtual z_status process_element();
	virtual z_status process_instruction();
	virtual z_status process_element_end();
	virtual z_status process_cdata();
	virtual z_status process_data();
	virtual void output(z_file &outfile);

};


template <class ELM_CLASS> class  z_xml_elm_type_T : public z_xml_elm_type
{
public:
	z_xml_elm* create_element(z_xml_elm* parent)
	{
		ELM_CLASS* elm = z_new ELM_CLASS();
		elm->init(this,parent);
		callback_new_elm(elm);
		return elm;
	}
	virtual void callback_new_elm(ELM_CLASS* elm) {};


};


#endif