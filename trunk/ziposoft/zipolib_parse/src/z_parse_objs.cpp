#include "z_parse_pch.h"
#include "z_parse.h"



//________________________________________________________________
//
//zp_text
//________________________________________________________________
zp_text::zp_text() 
{
}
zp_text::zp_text(ctext toddo) 
{
}
zp_text::~zp_text()
{}



ctext zp_text::get_text()
{
	return _text;
}

void zp_text::set_text(ctext t,size_t len)
{
	if(len)
		_text.assign(t,len);
	else
		_text=t;
}
ctext zp_text::get_map_key()
{
	return keyword_list[_type].desc;
}

void zp_text::output(z_file* fp)
{
	*fp << _text;
}
void zp_text::dump(int level,z_file& outf,int flags)
{
	int indent=level;
	while(indent--)	outf<<"  ";	

	outf<< keyword_list[_type].desc<<'='<<_text<<"\n";
}
//________________________________________________________________
//
//zp_obj
//________________________________________________________________
zp_obj::zp_obj() 
{
	_children=0;
//	_entry=0;
//	_parent=0;
//	_offset=0;
}
/*
zp_obj::zp_obj(const z_obj_fact* ie) 
{
	_children=0;
//	_entry=ie;
//	_parent=0;
//	_offset=0;
}
*/
zp_obj::~zp_obj()
{
#if DESTROY
	if(_children)
		delete _children;
	_children=0;
#endif
}
/*
void zp_obj::set_label(ctext t)
{
	if(_label) 			return _label->set_label( t);
	return ;
}

ctext zp_obj::get_label()
{
	if(_label) 	
		return _label->get_text();
	return "";
}
*/
/*
void zp_obj::output(z_file* fp)
{
	if(!_children) return;
	size_t size=_children->size();
	size_t i;
	for(i=0;i<size;i++)
	{
		zp_obj* item=_children->at(i);
		Z_ASSERT(item);
		item->output(fp);
	}
}
*/
void zp_obj::output(z_file* fp)
{
	/*
	if(!_children) return;
	size_t size=_children->size();
	size_t i;
	for(i=0;i<size;i++)
	{
		zp_obj_base* item=_children->at(i);
		Z_ASSERT(item);
		item->output(fp);
	}
	*/
}

zo_manipulator g_man;
void zp_obj::dump(int level, z_file& outf,int flags)
{
	zo_ftr_entry* f;
	int indent=level;
	while(indent--)	outf<<"  ";	
	//outf<<get_map_key()<<'['<<(int)_offset<<']'<< " ";
	outf<<get_map_key()<< "[";
	dump_custom(outf);
	/*
	zo_feature_list full_list;
	get_feature_map(&g_man,full_list,ZO_MT_PROP,false);
	full_list.reset_iter();
	while(f=full_list.get_next())
	{
		ctext name=full_list._internal_iter.key;
		outf<<name<<',';
		this->get_feature


	}*/

	outf<<"]\n";
	if(!_children) return;
	size_t size=_children->size();
	size_t i;
	for(i=0;i<size;i++)
	{
		zp_obj_base* item=_children->at(i);
		Z_ASSERT(item);
		item->dump(level+1,outf,flags);
	}
}
#if 0
zp_obj* zp_obj::get_next_child(ctext type,size_t& start)
{
	if(!_children)
		return 0;
	return _children->get_next_obj(type,start);
}
zp_obj* zp_obj::get_item_obj(ctext name)
{
	if(!_children)
		return 0;
	size_t start=0;
	return _children->get_next_obj_type<ctext,zp_obj>(name,start);
}
zp_obj* zp_obj::get_obj_by_name(ctext type,ctext name)
{
	size_t iter=0;
	zp_obj* child=0;
	while(1)
	{
		child=get_item_next_obj(type,iter);
		if(!child)
			break;
		/*
		if(strcmp(name,child->get_label())==0)
			return child;
			*/
		Z_ASSERT((0));
	}
	return 0;
}
zp_obj* zp_obj::get_item_next_obj(ctext type,size_t& start)
{
	if(!_children)
		return 0;
	return _children->get_next_obj_type<ctext,zp_obj>(type,start);
}



#endif
void zp_obj::add_child(zp_obj_base* item)
{
	if(!_children)
		_children=new zp_obj_vect_base();

	_children->push_back(item);
//	item->_parent=this;

}
zp_obj_base* zp_obj::get_child_by_offset(int &index,size_t off,int si)
{
	zp_obj_base* child=get_child(index);
	if(child)
		if((child->_templ_offset==off)
			/*&&(si==child->_stage_index)*/)
		{
			index++;
			return child;
	
		}
	return 0;

}

ctext zp_obj::get_parse_string()
{
	return get_fact()->parse_string;

}
zp_obj_base* zp_obj::get_child(int index)
{
	if(!_children)
		return 0;
	if(index>= (int)_children->size())
		return 0;
	return (*_children)[index];
}
/*
zp_obj* zp_item::new_child(ctext name,ctext data)
{
	zp_item* item=new zp_item(name);

	item->_text=data;
	add_child(item);
	return item;

}*/


ctext zp_obj::get_map_key()
{
	const z_obj_fact* ei=get_fact();
	Z_ASSERT((ei));
	return z_obj_fact_get_name(get_fact());
}
ctext zp_obj::get_template()
{
	const z_obj_fact* ei=get_fact();
	Z_ASSERT(0);
	return ei->parse_string;
}
int zp_feature::get_num_chars()
{
	size_t len=_name.size();
	if (_subscript)
		len+=_subscript_id.size();
	return (int)len;
}
ctext zp_feature::get_full_name(z_string& fullname)
{
	fullname=_name;
	if(_subscript)
		fullname<<':'<<_subscript_id;
	return fullname;
}



//________________________________________________________________
//
//zp_pair
//________________________________________________________________

#define ZO_OBJ_LIST \
	OBJ(zp_cmdline,zp_obj,"cmdline","cmdline",\
	"{_root}?'/':*({_path_list}ident:'/'):?({_object}ident:'.'):?{_feature}feat:"\
	"?({_subscript}'[':{_subscript_id}ident:']'):?( ({_assignment}'=':{_assign_val}value)|{_params}params)",\
	VAR(_feature) VAR(_subscript) VAR(_subscript_id)\
    VAR(_root)  VAR(_object) VAR(_assign_val)  VAR(_assignment) VAR(_params) VAR(_path_list) )\
	OBJ(zp_str_list,zp_obj,"strlist","strlist","'{':*(({_list}ident|{_list}string|{_list}string_sq):?','):'}'",VAR(_list) )\
	OBJ(zp_params,zp_obj,"params","params","('(':*({_param_list}value:?','):')')",VAR(_param_list) )\
	OBJ(zp_pair,zp_obj,"pair","pair desc","{_name}ident:'=':{_val}?ident:#','",VAR(_name) VAR(_val) )\
	OBJ(zp_value,zp_obj,"value",0,"(%whsp:'{':*{_child_list}obj:'}')|{_string_list}strlist|{_string}string|{_string}string_sq|{_obj}obj|{_string}ident",\
		VAR(_child_list) VAR(_string_list) VAR(_obj) VAR(_string)   )\
		OBJ(zp_feature,zp_obj,"feat",0,"{_name}ident:?({_subscript}':':{_subscript_id}ident)",VAR(_name) VAR(_subscript) VAR(_subscript_id)   )\
		/* TODO rename this */OBJ(zp_obj_feature,zp_obj,"feature",0,"%whsp:{_name}ident:'=':{_val}value",VAR(_name) VAR(_val)   )\
	OBJ(zp_obj_parse,zp_obj,"obj",0,"%whsp:{_name}ident:'<':*{_fet_list}feature:'>':*whsp",VAR(_name) VAR(_fet_list)  )\
	OBJ(zp_obj_generic,zp_obj,"generic","generic",0,NO_FTR)

//	OBJ(zp_pair,z_obj,"pair","pair desc",ZPS("{_name}ident:'=':{_val}ident:#','"),VAR(_name),VAR(_val))
//ZP_OBJ(zp_pair,zp_obj,"pair",,ZPV(_name),ZPV(_val));
#define Z_MODULE _Z_MODULE(parse)
#include "zipolib_parse/include/z_obj_macro.h"


