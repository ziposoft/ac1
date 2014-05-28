#ifndef z_factory_var_funcs_h
#define z_factory_var_funcs_h



/*
This interface manipulates simple member variables 
*/
template <class VAR >  class zf_var_funcs  : public zf_var_funcs_base
{
public:
	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
	virtual void clear(void* v) const;
	virtual void add(void* list,void* obj) const ;
	virtual void* get_item(void* list,size_t index) const;
	virtual size_t get_size(void* list) const;
	virtual void dump(z_file& s, void* v) const;
	virtual void* create_obj(void* var,const z_factory* fact) const;
};	
class zp_var_list_funcs_base  : public zf_var_funcs_base
{
public:
	virtual void clear(void* v) const;

	virtual const z_factory* get_fact() const=0;
	virtual z_obj_vector_base* get_list(void * v) const=0;
	void dump(z_file& f, void* v) const;
	virtual void* get_ptr(void* v,int* iter ) const;
};

/*
WARNING- overloaded funcs must match exactly! otherwise they will quietly not be called */
template <class TYPE >  class zp_var_list_funcs  : public zp_var_list_funcs_base
{
public:
	virtual const z_factory* get_fact()	const
	{
		return &z_factory_T<TYPE>::self;
	}

	virtual void* create_obj(void* v,const z_factory* fact) const
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
};


/*
This interface manipulates child objects 
*/
template <class CLASS >  class zp_child_obj_funcs  : public zf_var_funcs_base
{
public:
	virtual void* create_obj(void* var /* pointer to obj */,const z_factory* new_child_type) const
	{
		//OBJ instance is part of parent, so it is already created.
		//Just reset it and return a pointer to it.
		const z_factory* f=&z_factory_T<CLASS>::self;
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
		const z_factory* f=&z_factory_T<CLASS>::self;
		f->clear_all_vars(v);
	}
	virtual const  z_factory*  get_child_obj_fact() const 
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
	virtual const  z_factory*  get_child_obj_fact() const 
	{ 
		return &z_factory_T<CLASS>::self;
	}

	virtual void* get_ptr(void* var,int* iter ) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 
		return *ppObj;
	}


	virtual void* create_obj(void* var /* pointer to obj pointer*/,const z_factory* new_child_type) const
	{
		void** ppObj=reinterpret_cast<void**>(var); 

		const z_factory* f=&z_factory_T<CLASS>::self;
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
	 		file.indent_inc();
			file << "\n";
			z_factory_T<CLASS>::self.dump_obj(file,*ppObj);
			file.indent_dec();
		}
	}

	virtual void get(z_string& s, void* v) const
	{
		s="???";
	}
	virtual void clear(void* v) const{
		CLASS** ppObj=reinterpret_cast<CLASS**>(v); 
		if(*ppObj )
			delete *ppObj;
		*ppObj=0;
	}


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
	virtual void get(z_string& s, void* v) const;
	virtual void set(ctext  s, void* v) const;
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
