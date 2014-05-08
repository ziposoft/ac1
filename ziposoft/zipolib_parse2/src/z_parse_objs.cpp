#if 0
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
//void
//________________________________________________________________
void::void() 
{
	_children=0;
	_templ_offset=0;

//	_entry=0;
//	_parent=0;
//	_offset=0;
}
/*
void::void(const zp_factory* ie) 
{
	_children=0;
//	_entry=ie;
//	_parent=0;
//	_offset=0;
}
*/
void::~void()
{
#if DESTROY
	if(_children)
		delete _children;
	_children=0;
#endif
}
/*
void void::set_label(ctext t)
{
	if(_label) 			return _label->set_label( t);
	return ;
}

ctext void::get_label()
{
	if(_label) 	
		return _label->get_text();
	return "";
}
*/
/*
void void::output(z_file* fp)
{
	if(!_children) return;
	size_t size=_children->size();
	size_t i;
	for(i=0;i<size;i++)
	{
		void* item=_children->at(i);
		Z_ASSERT(item);
		item->output(fp);
	}
}
*/
void void::output(z_file* fp)
{
	/*
	if(!_children) return;
	size_t size=_children->size();
	size_t i;
	for(i=0;i<size;i++)
	{
		void* item=_children->at(i);
		Z_ASSERT(item);
		item->output(fp);
	}
	*/
}

zo_manipulator g_man;
void void::dump(int level, z_file& outf,int flags)
{
	zp_var_entry* f;
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
		void* item=_children->at(i);
		Z_ASSERT(item);
		item->dump(level+1,outf,flags);
	}
}
#if 0
void* void::get_next_child(ctext type,size_t& start)
{
	if(!_children)
		return 0;
	return _children->get_next_obj(type,start);
}
void* void::get_item_obj(ctext name)
{
	if(!_children)
		return 0;
	size_t start=0;
	return _children->get_next_obj_type<ctext,void>(name,start);
}
void* void::get_obj_by_name(ctext type,ctext name)
{
	size_t iter=0;
	void* child=0;
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
void* void::get_item_next_obj(ctext type,size_t& start)
{
	if(!_children)
		return 0;
	return _children->get_next_obj_type<ctext,void>(type,start);
}



#endif
void void::add_child(void* item)
{
	if(!_children)
		_children=new void_vect_base();

	_children->push_back(item);
//	item->_parent=this;

}
void* void::get_child_by_offset(int &index,size_t off,int si)
{
	void* child=get_child(index);
	if(child)
		if((child->_templ_offset==off)
			/*&&(si==child->_stage_index)*/)
		{
			index++;
			return child;
	
		}
	return 0;

}

ctext void::get_parse_string()
{
	return get_fact()->parse_string;

}
void* void::get_child(int index)
{
	if(!_children)
		return 0;
	if(index>= (int)_children->size())
		return 0;
	return (*_children)[index];
}
/*
void* zp_item::new_child(ctext name,ctext data)
{
	zp_item* item=new zp_item(name);

	item->_text=data;
	add_child(item);
	return item;

}*/


ctext void::get_map_key()
{
	const zp_factory* ei=get_fact();
	Z_ASSERT((ei));
	return z_obj_fact_get_name(get_fact());
}
ctext void::get_template()
{
	const zp_factory* ei=get_fact();
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
	OBJ(zp_cmdline,void,"cmdline","cmdline",\
	"{_root_slash}?'/':*({_path_list}ident:'/'):?({_object}ident:'.'):?{_feature}feat:"\
	"?({_subscript}'[':{_subscript_id}ident:']'):?( ({_assignment}'=':{_assign_val}value)|{_params}params)",\
	VAR(_feature) VAR(_subscript) VAR(_subscript_id)\
    VAR(_root_slash)  VAR(_object) VAR(_assign_val)  VAR(_assignment) VAR(_params) VAR(_path_list) )\
	OBJ(zp_str_list,void,"strlist","strlist","'{':*(({_list}ident|{_list}string|{_list}string_sq):?','):'}'",VAR(_list) )\
	OBJ(zp_params,void,"params","params","('(':*({_param_list}value:?','):')')",VAR(_param_list) )\
	OBJ(zp_pair,void,"pair","pair desc","{_name}ident:'=':{_val}?ident:#','",VAR(_name) VAR(_val) )\
	OBJ(zp_value,void,"value",0,"(%whsp:'{':*{_child_list}obj:'}')|{_string_list}strlist|{_string}string|{_string}string_sq|{_obj}obj|{_string}ident",\
		VAR(_child_list) VAR(_string_list) VAR(_obj) VAR(_string)   )\
		OBJ(zp_feature,void,"feat",0,"{_name}ident:?({_subscript}':':{_subscript_id}ident)",VAR(_name) VAR(_subscript) VAR(_subscript_id)   )\
		/* TODO rename this */OBJ(void_feature,void,"feature",0,"%whsp:{_name}ident:'=':{_val}value",VAR(_name) VAR(_val)   )\
	OBJ(void_parse,void,"obj",0,"%whsp:{_name}ident:'<':*{_fet_list}feature:'>':*whsp",VAR(_name) VAR(_fet_list)  )\
	OBJ(void_generic,void,"generic","generic",0,NO_FTR)

//	OBJ(zp_pair,void,"pair","pair desc",ZPS("{_name}ident:'=':{_val}ident:#','"),VAR(_name),VAR(_val))
//void(zp_pair,void,"pair",,ZPV(_name),ZPV(_val));
#define Z_MODULE _Z_MODULE(parse)
#include "zipolib_parse/include/z_obj_macro.h"



#endif