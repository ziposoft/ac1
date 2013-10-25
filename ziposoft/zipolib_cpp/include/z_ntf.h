/*________________________________________________________________________

 z_ntf_h

________________________________________________________________________*/


#ifndef z_ntf_h
#define z_ntf_h

#include "zipolib_cpp/include/zipolib_cpp_pch.h"
#include "zipolib_cpp/include/z_stl_map.h"
#include "zipolib_cpp/include/z_string.h"
class z_ntf_obj;



typedef z_ntf_obj* (*z_ntf_create_item_func)();
typedef int (z_ntf_obj::*z_ntf_member_func)();

class z_ntf_action
{
	z_string _name;
	z_ntf_obj* _parent;
	z_ntf_member_func* _func;
public:
	z_ntf_action(z_ntf_obj* parent,z_ntf_member_func* func);
	ctext get_name() { return _name; }

};
class z_ntf_obj
{
	z_string _name;
	z_map<z_ntf_obj> _children;
	z_map<z_ntf_action> _actions;
public:

	
	ctext get_name() { return _name; }

};




#endif

