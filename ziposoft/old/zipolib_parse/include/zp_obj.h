#ifndef z_obj_h
#define z_obj_h

#include "zipolib_parse/include/z_parse_pch.h"
#include "zipolib_parse/include/z_parse_def.h"


class zp_obj_base;
class zo_manipulator;

enum type_zo_memvar_oper
{
	zo_mvo_get,
	zo_mvo_get_next_from_list,
	zo_mvo_get_child_obj,
	zo_mvo_clear,
	zo_mvo_get_to_string,
	zo_mvo_set_from_string,
	zo_mvo_set_from_value,
	zo_mvo_set,
	zo_mvo_reset_iter,
	zo_mvo_dump,
	zo_mvo_display,
	zo_mvo_exec,
	zo_mvo_get_num_obj,


};


#define ZO_MT_VAR   0x00FF //member type variable: child or property
#define ZO_MT_ACT   0x0100 //member type action: only one type right now
#define ZO_MT_PROP  0x0001 //member type prop: simple member variable, integer, string, etc
#define ZO_MT_CHILD 0x0002 //member type child: 
#define ZO_MT_LIST  0x0004 //
#define ZO_MT_ALL   0xFFFF //all member types


enum type_zo_opt_security
{
	zo_opt_sec_none,
	zo_opt_sec_user,
	zo_opt_sec_admin,
	zo_opt_sec_design,
	zo_opt_sec_debug
};
enum type_zo_opt_ui
{
	zo_opt_ui_basic,
	zo_opt_ui_advanced,
	zo_opt_ui_everything,
	zo_opt_ui_debug
};
typedef zp_obj_base* (*zo_create_item_func)();

#define ZO_OPT_SEC_NONE    (zo_opt_sec_none<<28)
#define ZO_OPT_SEC_USER    (zo_opt_sec_user<<28) 
#define ZO_OPT_SEC_ADMIN   (zo_opt_sec_admin<<28) 
#define ZO_OPT_SEC_DESIGN  (zo_opt_sec_design<<28) 
#define ZO_OPT_SEC_DEBUG   (zo_opt_sec_debug<<28) 

#define ZO_OPT_UI_BASIC    (zo_opt_ui_basic<<24)   
#define ZO_OPT_UI_ADV      (zo_opt_ui_advanced<<24)   
#define ZO_OPT_UI_EXPERT   (zo_opt_ui_everything<<24)   
#define ZO_OPT_UI_DEBUG    (zo_opt_ui_debug<<24)   

#define ZO_OPT_PROP_HEX    0x1
#define ZO_OPT_NO_LOAD     0x2
#define ZO_OPT_NO_SAVE     0x4

typedef U32 zo_fet_opt;
#define DEF_OPT (ZO_OPT_UI_DEBUG|ZO_OPT_SEC_DEBUG)
struct zo_ftr_entry
{
	ctext _internal_name;
	ctext _display_name;
	ctext _short_id;
	ctext _desc;
	zo_fet_opt   _options;
	U32   _type;
	const void* _ftr_specific;
};
ctext zo_ftr_get_name(zo_ftr_entry* fe);

class zo_feature_list: public z_map_ctext<zo_ftr_entry>
{
public:
};

struct zo_action_params 
{
	ctext *list;
	U32 size;

};
#define NO_PARAMS {0}
struct z_obj_fact
{
	ctext name_internal;
	ctext name_display;
	ctext desc;
	zo_create_item_func create_func;
	zo_ftr_entry* var_list;
	const int var_list_size;
	const z_obj_fact* base_fact;
	ctext parse_string;
};
struct z_module_obj_entry 
{
	ctext name;
	const z_obj_fact* fact;
};
struct z_module_entry 
{
	ctext name;
	z_module_obj_entry *facts;
	const int num_facts;

};
extern const z_module_entry *z_module_master_list[];
extern const int z_module_master_list_size;
#define Z_MODULE_DECLARE(_NAME_) extern const z_module_entry z_module_##_NAME_;
#define Z_MOD(_NAME_) &z_module_##_NAME_
#define Z_MODULE_INCLUDE(...) const z_module_entry *z_module_master_list[] = { __VA_ARGS__ };const int z_module_master_list_size=sizeof(z_module_master_list)/sizeof(void*);
ctext z_obj_fact_get_name(const z_obj_fact* f);



const z_obj_fact*  zo_get_factory_by_name_and_length(ctext name,size_t len);
const z_obj_fact*  zo_get_factory(ctext name); //gets the factory from the master list


#define ZO_OBJ_H \
	public:virtual z_status feature_manipulate(zo_fet_man_context* context); \
	static const z_obj_fact FACT;\
	virtual const z_obj_fact* get_fact();



#define NULL_PARSE 0

class zo_manipulator;
class zp_value;

class zo_fet_man_context
{
public:
	zo_fet_man_context(zo_manipulator* man,type_zo_memvar_oper oper,ctext name,zo_ftr_entry* fet_ent);

	zo_manipulator* _man;
	type_zo_memvar_oper _oper;
	ctext _name;
	zo_ftr_entry* _p_fet_ent;
	zp_value* _p_assignment_value;

};


class z_obj_man_data
{
public:
	z_obj_man_data();
	zp_obj_base* _parent_obj;
	z_string _name;
	zo_ftr_entry _fet_ent;
	void init_from_fact(const z_obj_fact* f,zp_obj_base* parent);
};
class zp_obj_base
{
	z_obj_man_data* _man_data;
	z_obj_man_data* get_man_data();
public:
	zp_obj_base();

	virtual z_status feature_manipulate(zo_fet_man_context* context) {
		return zs_feature_not_found;
	};
	virtual const z_obj_fact* get_fact();
	zp_obj_base* get_parent_obj();
	void set_parent_obj(zp_obj_base* obj);
	virtual void get_path(z_string& path);
	virtual ctext get_map_key();
	//virtual bool compare_id(ctext id_to_compare);
	virtual ctext get_type();
	virtual ctext get_id();
	virtual void init_man_data(zp_obj_base* parent_obj);


	virtual void set_name(ctext t);
	static const z_obj_fact FACT;
	virtual zo_ftr_entry* get_feature_for_obj();

	virtual zo_ftr_entry* get_feature(ctext f);
	static zo_ftr_entry* get_feature(const z_obj_fact* fact,ctext f);
	virtual z_status get_feature_map(zo_manipulator* man,zo_feature_list& list,U32 feature_type,bool include_alias);


};	





class zo_str_container
{
public:
	virtual ctext get_next()=0;
	virtual void add(ctext x)=0;
	virtual void clear()=0;
	virtual size_t count()=0;
	virtual void reset_iter()=0;
	virtual void copy(zo_str_container& other);

};
class zo_str_vect : public z_strlist,public zo_str_container
{
public:
	U32 _iter;
	zo_str_vect()
	{
		_iter=0;
	}
    virtual void add(ctext x)
    {
		push_back(x);
    };
	virtual void clear() 
	{ 
		z_strlist::clear();
	}
	virtual size_t count() 
	{ 
		return size();
	}
	virtual void reset_iter() 
	{ 
		_iter=0;
	}
	virtual ctext get_next()
	{
		if(_iter>=size())
			return 0;
		ctext t= (*this)[_iter];
		_iter++;
		return t;
	}   
};
class zo_str_map : public z_stl_map<z_string,int>,public zo_str_container
{
public:
	zo_str_map()
	{
		iter=begin();

	}
	virtual zo_str_map & operator << (ctext s) { add(s); return *this; }

    virtual void add(ctext x)
    {
		Z_ASSERT(x);
        (*this)[x]=0;
    };

	z_stl_map<z_string,int>::iterator iter;
	virtual void reset_iter() 
	{ 
		iter=begin();
	}
	virtual void clear() 
	{ 
		z_stl_map<z_string,int>::clear();
	}
	virtual ctext get_next()
	{
		if(iter==end())
			return 0;
		const z_string& str=iter->first;
		iter++;
		return str.c_str();
	}   
	virtual size_t count() 
	{ 
		return size();
	}
};

class z_obj_container : public zp_obj_base
{
public:
	virtual zp_obj_base* get_next_obj()=0;
	virtual zp_obj_base* get_obj(ctext key)=0;
	virtual zp_obj_base* add_new(ctext type)=0;
	virtual void add_obj(zp_obj_base* obj)=0;
	virtual size_t get_count()=0;
	virtual void clear_objs()=0;
	virtual void reset_iter()=0;
	virtual void get_current_iter_as_string(z_string &s)=0;
	virtual z_status get_feature_map(zo_manipulator* man,zo_feature_list& list,U32 feature_type,bool include_alias);
	virtual zo_ftr_entry* get_feature(ctext f);
	virtual z_status feature_manipulate(zo_fet_man_context* context);


};
template <class ITEM_CLASS > class z_obj_vect : public std::vector<ITEM_CLASS*>, public z_obj_container
{
	size_t iter;
public:
	z_obj_vect<ITEM_CLASS> ()
	{
		iter=0;
	}
	typedef typename std::vector<ITEM_CLASS*> v; //gnu
	virtual zp_obj_base* get_obj(ctext key)
	{
		if(!key)
			return 0;
		U32 i;
		for(i=0;i<v::size();i++)
		{
			zp_obj_base* o=(*this)[i];
			if(o)
			{
				if(o->get_id())
					if(strcmp(o->get_id(),key)==0)
						return o;
			}

		}
		return 0;
	}

	virtual zp_obj_base* get_next_obj()
	{
		if(iter>=v::size() )
			return 0;
		return (*this)[iter++];
	}
	virtual zp_obj_base* add_new(ctext type)
	{
		const z_obj_fact* fact=zo_get_factory(type);
		if(!fact) return 0;
		zp_obj_base* new_obj=(fact->create_func)();
		if(!new_obj) return 0;
		add_obj(new_obj);
		return new_obj;
	}
	virtual void add_obj(zp_obj_base* obj)
	{
		push_back((ITEM_CLASS*)obj);
		obj->init_man_data(this);
	}
	virtual z_obj_vect<ITEM_CLASS> & operator << (ITEM_CLASS* obj) { add_obj(obj); return *this; }

	virtual size_t get_count() 
	{ 
		iter=0;return v::size(); 
	}
	virtual void clear_objs()
	{ 
		v::clear(); 
	}
	virtual void reset_iter()
	{ 
		iter=0; 
	}
	virtual void get_current_iter_as_string(z_string &s)
	{
		s = (int)iter;
	}


};

 
template <class ITEM_CLASS > class z_obj_map : public z_map<ITEM_CLASS>, public z_obj_container
{

	z_map_iter iter;
public:
	z_obj_map()
	{

	}
	virtual zp_obj_base* add_new(ctext type)
	{
		const z_obj_fact* fact=zo_get_factory(type);
		if(!fact) return 0;
		zp_obj_base* new_obj=(fact->create_func)();
		if(!new_obj) return 0;
		add_obj(new_obj);
		return new_obj;
	}
	virtual zp_obj_base* get_next_obj()
	{
		return z_map<ITEM_CLASS>::get_next(iter);
	}
	virtual ITEM_CLASS* get_next()
	{
		return z_map<ITEM_CLASS>::get_next(iter);
	}
	virtual void add_obj(zp_obj_base* obj)
	{
		add(obj->get_map_key(),(ITEM_CLASS*)obj);
	}
	virtual size_t get_count() { reset_iter(); return z_map<ITEM_CLASS>::size(); }
	virtual void clear_objs(){ z_map<ITEM_CLASS>::clear(); }
	virtual void reset_iter(){ iter.reset(); }
	virtual void get_current_iter_as_string(z_string &s)
	{
		z_map<ITEM_CLASS>::get_current(iter);
		s = iter.key;
	}
	virtual zp_obj_base* get_obj(ctext key)
	{

		return z_map<ITEM_CLASS>::get(key);
	}

};


// These are just dummy copies used by the editors, not compiled.
#define LIST(_VAR,_NAME,_ID,_DESC,_OPTIONS)
#define PROP(_VAR,_NAME,_ID,_DESC,_OPTIONS) 
#define ACT(_VAR,_NAME,_ID,_DESC,_OPTIONS,_PARAMLIST)
#define CHILD(_VAR,_NAME,_ID,_DESC,_OPTIONS) 
#define VAR(_VAR) 


#endif
