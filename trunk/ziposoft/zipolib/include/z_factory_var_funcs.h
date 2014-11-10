#ifndef z_factory_var_funcs_h
#define z_factory_var_funcs_h

class zp_str_list 
{
public:
	zp_str_list()
	{
	}
	z_strlist  _list;
};

class zp_cfg_obj ;
class zp_cfg_obj_list ;
class zp_value 
{
public:
	zp_value() 
	{
		_obj_list=0;//setting obj pointers to 0 is critical!!!
		_string_list=0;//setting obj pointers to 0 is critical!!!
		_obj=0;//setting obj pointers to 0 is critical!!!
	}
	//zp_cfg_obj* _child;
	zp_str_list* _string_list;
	int  _integer;
	z_string _string;
	zp_cfg_obj *_obj;
	zp_cfg_obj_list *_obj_list;
};

class zp_cfg_feature
{
public:
	zp_cfg_feature() 
	{
	}
	z_string _name;
	zp_value _val;
	ctext get_map_key () { return _name;}
};

class zp_cfg_obj
{
public:
	zp_cfg_obj() 
	{
	}
	ctext get_map_key () { return _obj_type;}
	z_string _obj_type;
	z_obj_vector_map<zp_cfg_feature> _features;
	z_status createobj(zf_obj& o);
	z_status load_obj(void* obj, z_factory* f);
};
class zp_cfg_obj_list
{
public:
	zp_cfg_obj_list() 
	{
	}
	z_obj_vector_map<zp_cfg_obj> _list;

};

class zf_var_funcs_act : public zf_var_funcs_base
{
public:
	virtual zf_feature_type get_type()const { return zf_ft_act; } 
	virtual zf_feature* create_feature(ctext name,z_memptr offset,ctext desc,U32 flags) const;

};
/*
This interface manipulates simple member variables 
*/
template <class VAR >  class zf_var_funcs  : public zf_var_funcs_base
{
public:
	virtual zf_feature_type get_type() const { return zf_ft_var; }
	virtual zf_feature* create_feature(ctext name,z_memptr offset,ctext desc,U32 flags) const;
	virtual void get(z_string& s, void* v,ctext format,int index=-1) const;
	virtual void set(ctext s, void* v,ctext format,int index=-1) const;
	virtual void clear(void* v) const;
	virtual void add(void* list,void* obj) const ;
	virtual void* get_sub_obj(void* list,ctext key) const;
	virtual size_t get_size(void* list) const;
	virtual void dump(z_file& s, void* v) const;
	virtual void* create_obj(void* var,z_factory* fact) const;
	virtual z_status set_from_value(zp_value* val, void* var,int index=-1) const ;

 	virtual z_status load(zp_text_parser &parser, void* v) const ;
 	virtual z_status assign(zp_text_parser &parser, void* v) const ;
 	virtual z_status evaluate1(zp_text_parser &parser, void* v) const ;

};	
class zf_funcs_obj_list_base  : public zf_var_funcs_base
{
public:
	virtual zf_feature* create_feature(ctext name,z_memptr offset,ctext desc,U32 flags) const;
	virtual zf_feature_type get_type() const{ return zf_ft_obj_list; }

	virtual void clear(void* v) const;

	virtual z_factory* get_list_obj_fact() const=0;
	virtual z_obj_list_base* get_list(void * v) const=0;
	void dump(z_file& f, void* v) const;
	virtual void* get_ptr(void* v,z_obj_list_iter& iter ) const;
	virtual void* get_sub_obj(void* list,ctext key) const; 
  	virtual z_status load(zp_text_parser &parser, void* v) const ;

};

/*
WARNING- overloaded funcs must match exactly! otherwise they will quietly not be called */
template <class TYPE >  class zf_funcs_obj_list_base_t  : public zf_funcs_obj_list_base
{
public:
	virtual z_factory* get_list_obj_fact()	const
	{
		return &z_factory_T<TYPE>::self;
	}

	virtual void* create_obj(void* v,z_factory* fact) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		TYPE* obj=reinterpret_cast<TYPE*>(fact->create_default_obj());
		list.add(obj);
		return obj;
	}


	virtual z_status set_from_value(zp_value* value, void* var,int index=-1) const 
	{
		//TODO - change all these to return status
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(var);
		zf_obj o;
		z_status status;
		if(value->_obj)
		{
			status=value->_obj->createobj(o);
			if(status)
				return status;
			TYPE* obj=reinterpret_cast<TYPE*>(o._obj);
			list.add(obj);
		}
		// HOLY CRAP THIS IS UGLY. TODO-FIX THIS!!
		if(value->_obj_list)
		{
			size_t i;
			for(i=0;i<value->_obj_list->_list.size();i++)
			{
				status=value->_obj_list->_list[i]->createobj(o);
				if(status)
					return status;
				TYPE* obj=reinterpret_cast<TYPE*>(o._obj);
				list.add(obj);
			}
		}

		return zs_ok;
	}
	virtual z_factory*  get_fact_from_obj(void* vobj) const 
	{ 
		TYPE* pObj=reinterpret_cast<TYPE*>(vobj); 
		if(!pObj) 
		{
			Z_ERROR(zs_bad_parameter);
			return 0;
		}
		ctext typetext=Z_TYPEINFO_O(*pObj);
		return zf_get_factory_by_type(typetext);
	}
};
template <class TYPE >  class zp_var_funcs_list_vect  : public zf_funcs_obj_list_base_t<TYPE>
{
	virtual z_obj_list_base* get_list(void* v ) const
	{
		z_obj_vector<TYPE>* list= reinterpret_cast<z_obj_vector<TYPE>*>(v);
		return list;
	}
};
template <class TYPE >  class zp_var_funcs_list_map  : public zf_funcs_obj_list_base_t<TYPE>
{
	virtual z_obj_list_base* get_list(void* v ) const
	{
		z_obj_map<TYPE>* list= reinterpret_cast<z_obj_map<TYPE>*>(v);
		return list;
	}
};
class zf_funcs_obj_base  : public zf_var_funcs_base
{
public:
	virtual zf_feature_type get_type() const{ return zf_ft_obj; }
  	virtual z_status load(zp_text_parser &parser, void* v) const ;
	virtual void dump(z_file& file, void* memvar) const;
	virtual zf_feature* create_feature(ctext name,z_memptr offset,ctext desc,U32 flags) const;

};


/*
This interface manipulates child objects 
*/
template <class CLASS >  class zp_child_obj_funcs  : public zf_funcs_obj_base
{
public:

	virtual void* create_obj(void* var /* pointer to obj */,z_factory* new_child_type) const
	{
		//OBJ instance is part of parent, so it is already created.
		//Just reset it and return a pointer to it.
		z_factory* f=&z_factory_T<CLASS>::self;
		if(new_child_type!=f)
		{
			Z_ERROR_MSG(zs_wrong_object_type,"Objects type does not match member variable");
			return 0;
		}
		f->clear_all_vars(var); 
		return var;
	}
	virtual void* get_sub_obj(void* var,ctext key) const { return  var;} 
	virtual void* get_ptr(void* var,z_obj_list_iter& iter) const
	{
		return var;
	}

	virtual z_factory*  get_fact_from_obj(void* obj) const 
	{ 
		return &z_factory_T<CLASS>::self;
	}

	virtual void dump(z_file& file, void* v) const
	{
		file.indent_inc();
		file << "\n";
		z_factory_T<CLASS>::self.dump_obj(file,v);
		file.indent_dec();
	}
	virtual z_status set_from_value(zp_value* val, void* var,int index=-1) const 
	{
		z_status status;
		//TODO - change all these to return status
		z_factory* f=&z_factory_T<CLASS>::self;
		if(!val->_obj)
			return Z_ERROR_MSG(zs_bad_parameter,"No object specified");
		status=val->_obj->load_obj(var,f);
		Z_ASSERT(status==zs_ok);
		return status;
	}


};
template <class CLASS >  const zf_var_funcs_base* zp_child_obj_funcs_get(CLASS& obj)
{
	static const zp_child_obj_funcs<CLASS> f;
	return &f;
};
/*
This interface manipulates child object pointers 
*/
template <class CLASS >  class zp_child_pobj_funcs  : public zf_funcs_obj_base
{
public:

	virtual zf_feature_type get_type() const{ return zf_ft_obj; }
	virtual z_factory*  get_fact_from_obj(void* pobj) const 
	{ 
		return &z_factory_T<CLASS>::self;
	}


	virtual void* get_ptr(void* var,z_obj_list_iter& iter ) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	}
	virtual void* get_sub_obj(void* var,ctext key) const 
	{ 
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	} 


	virtual void* create_obj(void* ppmvar /* pointer to obj pointer*/,z_factory* new_child_type) const
	{
		void** ppObj=reinterpret_cast<void**>(ppmvar); 
		//TODO - we could do a dynamic cast here to check that the new child type is valid for the memvar pointer.
		*ppObj=new_child_type->create_default_obj();
		return *ppObj;
	}

	virtual void get(z_string& s, void* v,ctext format,int index=-1) const
	{
		s="???";
	}
	virtual void clear(void* v) const{
		CLASS** ppObj=reinterpret_cast<CLASS**>(v); 
		if(*ppObj )
			delete *ppObj;
		*ppObj=0;
	}
	virtual z_status set_from_value(zp_value* val, void* var,int index=-1) const 
	{
		//TODO - change all these to return status
		void** ppObj=reinterpret_cast<void**>(var); 
		if(!val->_obj)
		{
			return Z_ERROR_MSG(zs_bad_parameter,"No object specified");
		}
		zf_obj o;
		z_status status=val->_obj->createobj(o);
		if(status)
			return status;
		*ppObj=o._obj;
		return zs_ok;
	}
};
template <class CLASS >  class zp_child_vobj_funcs  : public zp_child_pobj_funcs<CLASS>
{
public:
	virtual z_factory*  get_fact_from_obj(void* vobj) const 
	{ 
		CLASS* pObj=reinterpret_cast<CLASS*>(vobj); 
		if(!pObj) 
		{
			Z_ERROR(zs_bad_parameter);
			return 0;
		}
		ctext typetext=Z_TYPEINFO_O(*pObj);
		return zf_get_factory_by_type(typetext);
	}

};

template <class CLASS >  const zf_var_funcs_base* zp_child_vobj_funcs_get(CLASS*& obj)
{
	static const zp_child_vobj_funcs<CLASS> f;
	return &f;
};
template <class CLASS >  const zf_var_funcs_base* zp_child_pobj_funcs_get(CLASS*& obj)
{
	static const zp_child_pobj_funcs<CLASS> f;
	return &f;
};
/*
This is custom HEX interface 
*/
template <class VAR >  class zf_var_funcs_hex  : public zf_var_funcs<VAR>
{
public:
	virtual void get(z_string& s, void* v,ctext format,int index=0) const;
	virtual void set(ctext s, void* v,ctext format,int index=0) const;
};
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_hex_get(VAR& item)
{
	static const zf_var_funcs_hex<VAR> f;
	return &f;
};
const zf_var_funcs_base* zp_act_funcs_get();
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(VAR& item)
{
	static const zf_var_funcs<VAR> f;
	return &f;
};
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(z_obj_vector<VAR>& list)
{
	static const zp_var_funcs_list_vect<VAR> f;
	return &f;
};
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(z_obj_vector_map<VAR>& list)
{
	static const zp_var_funcs_list_vect<VAR> f;
	return &f;
};
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(z_obj_map<VAR>& list)
{
	static const zp_var_funcs_list_map<VAR> f;
	return &f;
};
#endif
