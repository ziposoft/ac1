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


/*
This interface manipulates simple member variables 
*/
template <class VAR >  class zf_var_funcs  : public zf_var_funcs_base
{
public:
	virtual void get(z_string& s, void* v,int index=-1) const;
	virtual void set(ctext  s, void* v,int index=-1) const;
	virtual void clear(void* v) const;
	virtual void add(void* list,void* obj) const ;
	virtual void* get_item(void* list,size_t index) const;
	virtual size_t get_size(void* list) const;
	virtual void dump(z_file& s, void* v) const;
	virtual void* create_obj(void* var,z_factory* fact) const;
	virtual z_status set_from_value(zp_value* val, void* var,int index=-1) const ;
	virtual zf_feature_type get_type() const { return zf_ft_var; }

};	
class zf_funcs_obj_list_base  : public zf_var_funcs_base
{
public:
	virtual void clear(void* v) const;

	virtual z_factory* get_fact() const=0;
	virtual z_obj_vector_base* get_list(void * v) const=0;
	void dump(z_file& f, void* v) const;
	virtual void* get_ptr(void* v,int* iter ) const;
	virtual zf_feature_type get_type() const{ return zf_ft_obj_list; }
	virtual void* get_item(void* list,size_t index) const; 

};

/*
WARNING- overloaded funcs must match exactly! otherwise they will quietly not be called */
template <class TYPE >  class zp_var_list_funcs  : public zf_funcs_obj_list_base
{
public:
	virtual z_factory* get_fact()	const
	{
		return &z_factory_T<TYPE>::self;
	}

	virtual void* create_obj(void* v,z_factory* fact) const
	{
		z_obj_vector<TYPE>& list= *reinterpret_cast<z_obj_vector<TYPE>*>(v);
		TYPE* obj=reinterpret_cast<TYPE*>(fact->create_obj());
		list.add(obj);
		return obj;
	}

	virtual z_obj_vector_base* get_list(void* v ) const
	{
		z_obj_vector<TYPE>* list= reinterpret_cast<z_obj_vector<TYPE>*>(v);
		return list;
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


/*
This interface manipulates child objects 
*/
template <class CLASS >  class zp_child_obj_funcs  : public zf_var_funcs_base
{
public:
	virtual zf_feature_type get_type() const{ return zf_ft_obj; }

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
	virtual void* get_ptr(void* var,int* iter ) const
	{
		return var;
	}
	virtual void clear(void* v) const{
		z_factory* f=&z_factory_T<CLASS>::self;
		f->clear_all_vars(v);
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
template <class CLASS >  class zp_child_pobj_funcs  : public zf_var_funcs_base
{
public:

	virtual zf_feature_type get_type() const{ return zf_ft_obj; }
	virtual z_factory*  get_fact_from_obj(void* obj) const 
	{ 
		return &z_factory_T<CLASS>::self;
	}


	virtual void* get_ptr(void* var,int* iter ) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	}


	virtual void* create_obj(void* var /* pointer to obj pointer*/,z_factory* new_child_type) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 

		z_factory* f=&z_factory_T<CLASS>::self;
		if(new_child_type!=f)
		{
			Z_ERROR_MSG(zs_wrong_object_type,"Objects type does not match member variable");
			return 0;
		}

		*ppObj=f->create_obj();
		return *ppObj;
	}
	virtual void dump(z_file& file, void* v) const
	{
		void** ppObj=reinterpret_cast<void**>(v); 

		if(*ppObj == 0)
			file<< "NULL";
		else
		{
			z_factory* fact=get_fact_from_obj(*ppObj);
			Z_ASSERT(fact);
			if(!fact)
				return;//Z_ERROR
			file.indent_inc();
			file << "\n";
			fact->dump_obj(file,*ppObj);
			file.indent_dec();
		}
	}

	virtual void get(z_string& s, void* v,int index=-1) const
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
template <class VAR >  class zp_var_funcs_hex  : public zf_var_funcs_base
{
public:
	virtual void get(z_string& s, void* v,int index=0) const;
	virtual void set(ctext  s, void* v,int index=0) const;
};

template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(VAR& item)
{
	static const zf_var_funcs<VAR> f;
	return &f;
};
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(z_obj_vector<VAR>& list)
{
	static const zp_var_list_funcs<VAR> f;
	return &f;
};
template <class VAR >  const zf_var_funcs_base* zp_var_funcs_get(z_obj_vector_map<VAR>& list)
{
	static const zp_var_list_funcs<VAR> f;
	return &f;
};
#endif
