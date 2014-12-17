#include "zipolib_cpp_pch.h"

#include "zipolib/include/z_factory.h"
#include "zipolib/include/z_parse.h"
#include "zipolib/include/z_parse_text.h"

#define RECAST(_TYPE_,_NAME_) _TYPE_& _NAME_= *reinterpret_cast<_TYPE_*>(v);
#define VF template <> z_status zf_var_funcs
/*________________________________________________________________________

 zf_var_funcs_base
________________________________________________________________________*/
z_status zf_var_funcs_base::dump(z_file& file, void* v) const
{
	z_string s;
	z_status status=get(s,v,0);
	if(status)
		return status;
	file<<s;
	return zs_ok;
}
 z_status zf_var_funcs_base::assign(zp_text_parser &parser, void* v,zf_feature_flags oper) const
{
	z_status status=parser.test_char('=');
	if(status)
		return Z_ERROR_MSG(status,"Expected '=' ");
	return load( parser,v,oper);
}

 /*________________________________________________________________________

 zf_var_funcs_act
________________________________________________________________________*/

zf_feature* zf_var_funcs_act::create_feature(ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc) const
{

	zf_action* action=z_new	zf_action(id,name,*(z_memptr*)&offset,flags);
	return action;



}


/*________________________________________________________________________

 zf_var_funcs<TYPE> defaults
________________________________________________________________________*/

template <class V> z_status zf_var_funcs<V>::dump(z_file& file, void* v) const {	zf_var_funcs_base::dump( file,  v) ; return zs_ok;  }
template <class V> z_status zf_var_funcs<V>::add(void* list,void* obj) const {return Z_ERROR_NOT_IMPLEMENTED;}
template <class V> void* zf_var_funcs<V>::get_sub_obj(void* list,ctext key) const {	return 0;}
template <class V> size_t zf_var_funcs<V>::get_size(void* list) const{	return 0;}
template <class V> void* zf_var_funcs<V>::create_obj(void* list,z_factory* fact) const{	return 0;}
template <class V> z_status zf_var_funcs<V>::get(z_string& s, void* v,ctext format,int index)	const{return Z_ERROR_NOT_IMPLEMENTED;}
template <class V> z_status zf_var_funcs<V>::set(ctext s, void* v,ctext format,int index)	const{return Z_ERROR_NOT_IMPLEMENTED;}
template <class V> z_status zf_var_funcs<V>::clear( void* v)	const{return Z_ERROR_NOT_IMPLEMENTED;}
template <class V> z_status zf_var_funcs<V>::set_from_value(zp_value* val, void* var,int index)	const{  set(val->_string,var,0); return zs_ok;}
template <class V> z_status zf_var_funcs<V>::assign(zp_text_parser &parser, void* v,zf_feature_flags oper) const 
{
	z_status status=parser.test_char('=');
	if(status)
		return Z_ERROR_MSG(status,"Expected '=' ");
	return load( parser,v,oper);
}
template <class V> z_status zf_var_funcs<V>::evaluate1(zp_text_parser &parser, void* v) const {return Z_ERROR_NOT_IMPLEMENTED;}
template <class V> zf_feature* zf_var_funcs<V>::create_feature(ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc) const 
{
	zf_feature* feat=z_new	zf_prop(id,name,this,offset,flags,desc);
	return feat;


}



/*________________________________________________________________________

 zf_var_funcs<bool> 
________________________________________________________________________*/
VF<bool>::clear(void* v)            const {RECAST(bool,b); b=false;  return zs_ok;  }
VF<bool>::get(z_string& s, void* v,ctext format,int index) const {RECAST(bool,b); s=(b?"true":"false");return zs_ok;  }
VF<bool>::set(ctext s, void* v,ctext format,int index)     const {RECAST(bool,b); b=(strcmp(s,"true")==0);  return zs_ok;  }
VF<bool>::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const 
{
	z_status status=parser.test_any_identifier();
	if(status)
		return Z_ERROR_MSG(status,"Error loading bool type");
	z_string s;
	if(!(oper&ZFF_SKIP))
	{
		parser.get_match(s);
		set(s,v,0);
	}


	return zs_ok;
}

/*________________________________________________________________________

 zf_var_funcs_hex<int> 
________________________________________________________________________*/
template <>  z_status zf_var_funcs_hex<int>::get(z_string& s, void* v,ctext format,int index) const	
{RECAST(int,i); s.Format("0x%x",i); return zs_ok; }
template <> z_status  zf_var_funcs_hex<int> ::set(ctext s, void* v,ctext format,int index) const		
{
	RECAST(int,i);
	if(sscanf(s,"%x",&i)!=1)
	{
			i=0;
			return zs_bad_parameter;
	}
	return zs_ok;
}


/*________________________________________________________________________

 zf_var_funcs<int> 
________________________________________________________________________*/
VF<int>::clear(void* v) const			{RECAST(int,i); i=0;    return zs_ok;  }
VF<int>::get(z_string& s, void* v,ctext format,int index) const
{
	RECAST(int,i); s=i; 
	return zs_ok;
}
VF<int>::set(ctext s, void* v,ctext format,int index) const		
{
	RECAST(int,i);
	if(strncmp(s,"0x",2)==0)
	{
		if(sscanf(s,"%x",&i)!=1)
			i=0;
			return zs_bad_parameter;
	}
	else
		i=atoi(s);  
	return zs_ok;
}
template <> z_status zf_var_funcs<int>::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const 
{
	z_status status=parser.test_integer();
	if(status)
		return Z_ERROR_MSG(status,"Error loading int type");
	z_string s;
	if(!(oper&ZFF_SKIP))
	{
		parser.get_match(s);
		set(s,v,0);
	}
	return zs_ok;
}
/*________________________________________________________________________

zf_var_funcs<z_string> 
________________________________________________________________________*/
VF<z_string>::get(z_string& s, void* v,ctext format,int index) const{RECAST(z_string,str); s=str;   return zs_ok;   }
VF<z_string>::set(ctext s, void* v,ctext format,int index) const{RECAST(z_string,str); str=s;  return zs_ok;   }
VF<z_string>::clear(void* v) const{	RECAST(z_string,str); str=""; return zs_ok; }
VF<z_string>::dump(z_file& file, void* v) const{	RECAST(z_string,str);z_string out;z_str_escape(str,out);file <<out; return zs_ok; }
 template <> z_status zf_var_funcs<z_string>::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const 
{
	z_status status=parser.test_code_string();
	if(status)
		status=parser.test_single_quoted_string();

	if(status)
		return Z_ERROR_MSG(status,"Expecting quoted string");
	if(oper&ZFF_SKIP)
		return zs_ok; 
	z_string match,unesc;
	parser.get_match(match);
	z_str_unescape(match,unesc);

	set(unesc,v,0);
	return zs_ok;
}
/*________________________________________________________________________

zf_var_funcs<z_strlist> 
________________________________________________________________________*/
VF<z_strlist>::dump(z_file& file, void* v)	const{RECAST(z_strlist,list);
	size_t i;
	file<<'{';
	z_string s;
	for(i=0;i<list.size();i++)
	{
		if(i)
			file<<',';
		z_str_escape(list[i],s);
		file<<s;
	}

	file<<'}';
	return zs_ok;
}
 template <>  z_status zf_var_funcs<z_strlist>::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const
{
	RECAST(z_strlist,list);
	if(!(oper&ZFF_SKIP))
		list.clear();
	z_status status;
	parser.skip_ws();
	status=parser.test_char('{');
	if(status)
		return Z_ERROR_MSG(status,"Expected '{' ");
	z_string s;

	while(status==zs_ok)
	{
		parser.skip_ws();
		status=parser.test_code_string();
		if(status)
			status=parser.test_single_quoted_string();
		if(status)
			break;

		if(!(oper&ZFF_SKIP))
		{
			parser.get_match(s);

			z_string unesc;
			z_str_unescape(s,unesc);
			list<<unesc;
		}

 		status=parser.test_char(',');
		if(status)
			break;

	}
	parser.skip_ws();
	status=parser.test_char('}');
	if(status)
		return Z_ERROR_MSG(status,"Expected '}' ");

	return zs_ok;

}


VF<z_strlist>::get(z_string& s, void* v,ctext format,int index)	const
{	
	RECAST(z_strlist,list);	
	if(index==-1)
	{
		list.get_as_string(s);
		return zs_ok;
	}
	if(index<(int)list.size()) 
		s=list[index];    
	return zs_ok;
}
VF<z_strlist>::clear(void* v)				const{	RECAST(z_strlist,list);	list.clear(); return zs_ok; }
VF<z_strlist>::set(ctext s, void* v,ctext format,int index)		const
{	
	RECAST(z_strlist,list);
	if(index==-1)
	{
		list<<s; //this is important for multi-stage items in  the parser. 
		return zs_ok;
	}
	if(index<(int)list.size()) 
		list[index]=s;  
	return zs_ok;

}
template <> z_status zf_var_funcs<z_strlist>::set_from_value(zp_value* val, void* v,int index)		const{	RECAST(z_strlist,list);
	if(index==-1)
	{
		if(val->_string_list)
			list=val->_string_list->_list ;
		else
			list<<val->_string;//if we are passed a single string, then just append it.
		return zs_ok;
	}
	if(index<(int)list.size())
	{
		list[index]=val->_string;
		return zs_ok;
	}
	return zs_out_of_range;
	

}
/*________________________________________________________________________

zf_funcs_obj_base 
________________________________________________________________________*/

z_status zf_funcs_obj_base::dump(z_file& file, void* memvar) const
{
	void* pObj=get_sub_obj(memvar,0); 

	if(pObj == 0)
		file<< "NULL";
	else
	{
		z_factory* fact=get_fact_from_obj(pObj);
		Z_ASSERT(fact);
		if(!fact)
			return Z_ERROR(zs_not_found);//Z_ERROR
		file.indent_inc();
		file << "\n";
		fact->dump_obj_static(file,pObj);
		file.indent_dec();
	}
	return zs_ok;
}
z_status zf_funcs_obj_base::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const
{
	void* pObj=0;
	z_factory* factory=0;
	if(!(oper&ZFF_SKIP))
		pObj=get_sub_obj(v,0);

	return zf_create_obj_from_text_stream_dyn(parser, factory,pObj,oper);;

}

zf_feature* zf_funcs_obj_base::create_feature(ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc) const 
{
	zf_feature* feat=z_new	zf_child_obj(id,name,this,offset,flags,desc);
	return feat;


}


/*________________________________________________________________________

zf_var_funcs<zp_obj_vector> 
________________________________________________________________________*/
template <> void* zf_var_funcs<zp_obj_vector>::create_obj(void* v,z_factory* fact) const
{
	RECAST(zp_obj_vector,list);
	zf_obj obj;
	obj._obj=fact->create_default_obj();
	obj._fact=fact;
	list.push_back(obj);

	return obj._obj;
}
template <> size_t zf_var_funcs<zp_obj_vector>::get_size(void* v) const
{
	RECAST(zp_obj_vector,list);
	return list.size();
}

VF<zp_obj_vector>::dump(z_file& file, void* v) const
{
	RECAST(zp_obj_vector,list);
	size_t count=list.size();
	size_t i;
	for(i=0;i<count;i++)
	{
		list[i]._fact->dump_obj_static(file,list[i]._obj);
	}
	return zs_ok;
}
template <> z_status zf_var_funcs<zp_obj_vector>::set_from_value(zp_value* val, void* v,int index)		const{	
	RECAST(z_strlist,list);

	Z_ASSERT(0);//TODO
	return Z_ERROR_NOT_IMPLEMENTED;
}
template <> z_status zf_var_funcs<zp_obj_vector>::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const 
{
	return Z_ERROR_NOT_IMPLEMENTED;
}
/*________________________________________________________________________

zf_funcs_obj_list_base 
________________________________________________________________________*/
zf_feature* zf_funcs_obj_list_base::create_feature(ctext id,ctext name,z_memptr offset,zf_feature_flags flags,ctext desc) const 
{
	zf_feature* feat=z_new	zf_list(id,name,this,offset,flags,desc);
	return feat;
}

z_status zf_funcs_obj_list_base::clear(void* v) const 
{

	z_obj_list_base* plist=get_list(v);
	Z_ASSERT(plist);
	z_obj_list_iter i;
	void* p;
	while((p=plist->get_next(i)))
	{
		get_list_obj_fact()->delete_obj(p);
	}
	plist->clear();
	 return zs_ok; 

}

void* zf_funcs_obj_list_base::get_sub_obj(void* v,ctext key) const
{
	z_obj_list_base* plist=get_list(v);
	if(!plist)
	{
		Z_ERROR(zs_bad_parameter);
		return 0;
	}

	return plist->get_void_by_key(key);
}

void* zf_funcs_obj_list_base::get_ptr(void* v,z_obj_list_iter& iter ) const
{
	z_obj_list_base* plist=get_list(v);
	if(!plist)
	{
		//this will never happen because it is a static cast
		//Z_ERROR_MSG(zs_bad_parameter,"Objects type does not match member variable");
		return 0;
	}

	return plist->get_next(iter);
}
z_status zf_funcs_obj_list_base::dump(z_file& f, void* v) const 
{
	z_obj_list_base* plist=get_list(v);
	z_obj_list_iter iter;

	size_t count=plist->size();
	if(!count)
	{
		f << "{}";
		return zs_ok;
	}
	f << "{\n";
	f.indent_inc();
	void* p;
	while((p=plist->get_next(iter)))
	{
		z_factory* fact=get_fact_from_obj(p);
		if(!fact)
		{
			Z_ERROR_MSG(zs_error,"Could not get factory from object ");
		}
		else
		fact->dump_obj_static(f,p);
	}
	f.indent_dec();
	f.indent();
	f << "}";
	return zs_ok;
}
z_status zf_funcs_obj_list_base::load(zp_text_parser &parser, void* v,zf_feature_flags oper) const 
{
	z_obj_list_base* plist=0;
	if(v)
	{
		plist=get_list(v);
		plist->clear();
	}
	z_status status=zs_ok;
	parser.skip_ws();
	status=parser.test_char('{');
	if(status)
		return Z_ERROR_MSG(status,"Expected '{' ");
	z_string membervarname;


	while(status==zs_ok)
	{
		parser.skip_ws();
		void* pObj=0;
		z_factory* factory=0;
		status= zf_create_obj_from_text_stream_dyn(parser, factory,pObj,oper);;
 		if(status)
			break;
		if(!(oper&ZFF_SKIP))
			plist->add_void(pObj);
	}
	parser.skip_ws();
	status=parser.test_char('}');
	if(status)
		return Z_ERROR_MSG(status,"Expected '}' ");
	return zs_ok;
}
/*________________________________________________________________________

zf_var_funcs<z_obj_vector> 
________________________________________________________________________*/





template class zf_var_funcs<z_string>;
template class zf_var_funcs<int>;
template class zf_var_funcs<bool>;
template class zf_var_funcs<z_strlist>;
template class zf_var_funcs<zp_obj_vector>;

const zf_var_funcs_base* zp_act_funcs_get()
{
	static const zf_var_funcs_act f;
	return &f;
};