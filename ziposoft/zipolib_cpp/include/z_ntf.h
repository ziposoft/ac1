/*________________________________________________________________________

 z_ntf_h

________________________________________________________________________*/


#ifndef z_ntf_h
#define z_ntf_h

#include "zipolib_cpp/include/zipolib_cpp_pch.h"
#include "zipolib_cpp/include/z_stl_map.h"
#include "zipolib_cpp/include/z_string.h"
#include "zipolib_cpp/include/z_type_converter.h"
class z_ntf_obj;



typedef z_ntf_obj* (*z_ntf_create_item_func)();
typedef int (z_ntf_obj::*z_ntf_member_func)();


class z_ntf_feature
{
	z_string _name;
	z_ntf_obj* _parent;
public:
	z_ntf_feature(z_ntf_obj* parent,ctext name);
	ctext get_map_key() { return _name; }
};

class z_ntf_action : public z_ntf_feature
{
	z_ntf_member_func* _func;
public:
	z_ntf_action(z_ntf_obj* parent,ctext name,z_ntf_member_func* func);
};
class z_ntf_prop : public z_ntf_feature
{
public:
	z_ntf_prop(z_ntf_obj* parent,ctext name);
	virtual~ z_ntf_prop();
	virtual void set_value(ctext from)=0;
	virtual void get_value( z_string& str)=0;
	virtual void convert(ctext from, z_string& to){to=from;}
	virtual void convert(ctext from,U32 & to){to=    atol(from); ;}
	virtual void convert(const U32 & from,z_string& to){to=from;}
	virtual void convert(ctext from,int & to){to=    atol(from); ;}
	virtual void convert(const int & from,z_string& to){to=from;}
	//virtual void convert(const U64 & from,z_string& to){to=from;}

};


template <class ITEM> class z_ntf_prop_t : public z_ntf_prop
{
public:
	ITEM* _p_memvar;

	z_ntf_prop_t(z_ntf_obj* parent,ctext name,ITEM* var) :z_ntf_prop( parent,name)
	{
		_p_memvar=var;	
	}
	virtual ~z_ntf_prop_t()
	{
		
	}	
	void set_value(ctext from)
	{
		convert(from,*_p_memvar);	
	}
	void get_value(z_string& str)
	{
		convert(*_p_memvar,str);	
	}
};

class z_ntf_prop_hex32 : public z_ntf_prop_t<U32>
{
public:
	z_ntf_prop_hex32(z_ntf_obj* parent,ctext name,U32* var) :z_ntf_prop_t<U32>( parent,name,var)  {}

	virtual void convert(ctext from,U32 & to){to=    z_stringtoue(from,16); ;}
	virtual void convert(const U32 & from,z_string& to){to.hex(from);}

};
#define PROP_X(_X_,...) addProp(new z_ntf_prop_t (this,#_X_,&_X_,...))
#define PROP(_X_) addProp(#_X_,&_X_)
#define PROP_T(_TYPE_,_VAR_) addPropT<z_ntf_prop_##_TYPE_>(#_VAR_,&_VAR_)


class z_ntf_obj
{
	z_string _name;
	z_map<z_ntf_obj> _children;
	z_map<z_ntf_action> _actions;
	z_map<z_ntf_feature> _features;
public:
	z_map<z_ntf_prop> props;
	template <class PROP_TYPE,class ITEM> void addPropT(ctext id,ITEM* var)
	{
		z_ntf_prop* p= new PROP_TYPE (this,id,var);
		props << p;
	}	
	template <class ITEM> void addProp(ctext id,ITEM* var)
	{
		z_ntf_prop* p= new z_ntf_prop_t<ITEM> (this,id,var);
		props << p;
	}
	ctext get_map_key() { return _name; }

};




#endif

