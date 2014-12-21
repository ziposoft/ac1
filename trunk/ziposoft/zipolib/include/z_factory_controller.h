/*________________________________________________________________________

z_factory_h

________________________________________________________________________*/


#ifndef z_factory_controller_h
#define z_factory_controller_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_parse_text.h"
#include "zipolib/include/z_factory.h"

class zf_feature_node
{
public:
	zf_feature_node(zf_feature *f,z_string& s)
	{
		_f=f;
		_index=s;
	}
	zf_feature *_f;
	z_string _index;
	z_status append_to_path(z_string &path);

} ;


class z_fact_obj_path : public std::vector<zf_feature_node>
{
public:

	z_status get_path_string(z_string& s);
	z_status up(zf_obj& new_selected);

};
class z_factory_controller
{
	zf_obj _root;
	zf_obj _self;
	zf_obj _selected;
	z_fact_obj_path _obj_path;
protected:
	

	//zf_obj _temp;
	//z_string _temp_path;
	zp_text_parser _tparser;
public:
	template <class CLASS> void setroot(CLASS * obj)
	{
		_root._obj=obj;
		_root._fact=&z_factory_T<CLASS>::self;
		_selected= _root;

	}
	const zf_obj& get_selected() { return  _selected; }
	void  set_selected(zf_obj& newobj) {   _selected=newobj; }
	const zf_obj& get_self() { return  _self; }
	const zf_obj& get_root() { return  _root; }

	void set_path(z_fact_obj_path& newpath) { _obj_path= newpath;         }
	z_fact_obj_path& get_path() { return _obj_path;         }
	zp_text_parser& get_parser() { return  _tparser;}
 	void get_current_text_path_from_obj_path(z_string &s) { _obj_path.get_path_string(s);         }

	z_factory_controller(z_factory* f,void* obj)
	{
		void* me=(void*)this;
		_self._fact=f;
		_self._obj=(void*)obj;	
		_selected=_self;
	}
 	z_status select_obj_from_path(z_string& path);

	z_status path_up();
	z_status parse_feature_and_index(z_string& out_feature,z_string& out_index);
	z_status EvaluatePath(ctext text_path, zf_obj& selected,z_fact_obj_path& out_obj_path,z_string & out_feature,z_string& out_feature_index);
};
#endif

