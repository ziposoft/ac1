/*________________________________________________________________________

 z_obj_list

________________________________________________________________________*/

#ifndef z_obj_list_h
#define z_obj_list_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_dbg.h"
#include "zipolib/include/z_stl_map.h"

#include <vector>


class z_obj_list_iter
{
public:
	z_obj_list_iter()
	{
		vect=0;
	}
	void reset()
	{
		map.reset();
		vect=0;
	}
	z_map_iter map;
	size_t vect;

};

class z_obj_list_base
{
public:
	z_obj_list_base();
	virtual void clear()=0;
	virtual size_t size() const=0;
	
	virtual void* get_void(size_t i) =0;
	virtual void* get_void_by_key(ctext key) =0;
	
	virtual void add_void(void* o)=0;
	virtual void* get_next(z_obj_list_iter& i)=0;
	virtual z_status get_next_key(z_obj_list_iter& i,z_string &s)=0;
	
	z_status act_clear()
	{
		clear();
		return zs_ok;
	}	
	z_status act_test()
	{
		printf("hello?\n");
		return zs_ok;
	}

};

template <class ITEM_CLASS > class z_obj_vector  : public z_obj_list_base
{
protected:
	std::vector<ITEM_CLASS*>  _vector;
public:
	virtual ~z_obj_vector()
	{
		//destroy();
	}
	virtual void* get_next(z_obj_list_iter &i)
	{
		void* v=get(i.vect);
		i.vect++;
		return v;
	}
	virtual z_status remove(size_t i)
	{
		_vector.erase(_vector.begin()+i);
		return zs_ok;
	}
	virtual z_status get_next_key(z_obj_list_iter& iter,z_string &s)
	{
		
		if(iter.vect>=size())
			return zs_end_of_list;
		s=iter.vect;
		iter.vect++;
		return zs_ok;

	}
 	void add_void(void* v)
	{
		ITEM_CLASS* i=reinterpret_cast<ITEM_CLASS*>(v);
		add(i);
	}
	void add(ITEM_CLASS* item)
	{
		_vector.push_back(item);
		return ;
	}
	void destroy()
	{ 
		size_t i;
		for(i=0;i<size();i++)
		{
			ITEM_CLASS* item=get(i);
			if(item)
				delete item;
		}
		_vector.clear(); 
	}

	void clear()
	{ 
		_vector.clear(); 
	}

 	size_t size()	const
	{ 
		return _vector.size(); 
	}
	
	ITEM_CLASS* operator [](size_t i)  	const
	{
		if(i>=size())
			return 0;
		return _vector[i];
	}
	ITEM_CLASS* get(size_t i)  const
	{
		if(i>=size())
			return 0;
		return _vector[i];
	}
	void* get_void_by_key(ctext key)  
	{ 
		size_t i=atoi(key);
		return get_void(i); 
	}

 	void* get_void(size_t i) 
	{
		return (void*)get(i);
	}
    virtual z_obj_vector<ITEM_CLASS> & operator << (ITEM_CLASS *x)
    {
		_vector.push_back(x);
		return *this;
    };


};
template <class ITEM_CLASS > class z_obj_vector_map  : public z_obj_vector<ITEM_CLASS>
{
public:
 
	ITEM_CLASS* get_by_key(ctext t)   const
	{
		size_t i=0;
		while(i<this->size())
		{
			ITEM_CLASS* obj= this->get(i);
			if(strcmp(obj->get_map_key(),t)==0)
				return obj;
			i++;
		}
		return 0;
	}
};

template <class ITEM_CLASS > class z_obj_map  : public z_obj_list_base
{
protected:
	std::map<ctext,void*,ctext_less_than>  _map;
public:
	/*
    virtual z_map<ITEM_CLASS> & operator << (ITEM_CLASS *x)
    {
		Z_ASSERT(x);
        add(x->get_map_key(),x);
        return *this;
    };
	*/
	virtual ~z_obj_map()
	{
		//destroy();
	}
	virtual void* get_next(z_obj_list_iter& i)
	{
		return get_next(i.map);
	}

 	void add_void(void* v)
	{
		ITEM_CLASS* i=reinterpret_cast<ITEM_CLASS*>(v);
		add(i->get_map_key(),i);
	}
	bool add(ctext key_in,ITEM_CLASS* item)
	{
		size_t len=strlen(key_in);
		char* s=z_new char[len+1];
		
		strncpy(s,key_in,len);
		s[len]=0;
		_map[s]=item;
		
		return true;
	}
	void destroy()
	{ 
		z_map_iter iter;
		ITEM_CLASS* item;
		while(1)
		{
			item=get_next(iter);
			if(item)
				delete item;
			else
				break;

		}
		_map.clear(); 
	}
	void clear()
	{ 
		_map.clear(); 
	}

 	size_t size() const
	{ 
		return _map.size(); 
	}
	ITEM_CLASS* get_current(z_map_iter& iter)
	{

		if(iter.key==0)
			iter.i=_map.begin();
		if(iter.i==_map.end()) return 0;
		void* data_out=iter.i->second;
		iter.key=iter.i->first;
		return (ITEM_CLASS*)data_out;
	}
	ITEM_CLASS* get_next(z_map_iter& iter)
	{
		void* data_out=get_current(iter);
		if(data_out)
			iter.i++;
		return (ITEM_CLASS*)data_out;
	}
	virtual z_status get_next_key(z_obj_list_iter& iter,z_string &s)
	{
		if(iter.map.key==0)
			iter.map.i=_map.begin();
		if(iter.map.i==_map.end())
			return zs_end_of_list;
		iter.map.key=iter.map.i->first;
		s=iter.map.key;
		iter.map.i++;
		return zs_ok;

	}
	ITEM_CLASS* operator [](ctext key)
	{
		z_map_iterator i=_map.find(key);
		if(i==_map.end()) return 0;
		void* data_out=i->second;
		return (ITEM_CLASS*)data_out;
	}
	ITEM_CLASS* get(ctext key) 
	{
		z_map_iterator i=_map.find(key);
		if(i==_map.end()) return 0;
		void* data_out=i->second;
		return (ITEM_CLASS*)data_out;
	}
	virtual void* get_void(size_t i)  { return 0; }
	virtual void* get_void_by_key(ctext key)  { return get(key); }

	bool del(ctext key)
	{
		z_map_iterator i=_map.find(key);
		if(i==_map.end()) return false;
		_map.erase(i);
		return true;
	}

	virtual void init_child(ITEM_CLASS *obj)
	{

	}
	virtual z_obj_map<ITEM_CLASS> & operator , (ITEM_CLASS *obj)
	{
 		Z_ASSERT(obj);
		this->add(obj->get_map_key(),obj); 
		init_child(obj);
		return *this;
	}
	virtual z_obj_map<ITEM_CLASS> & operator << (ITEM_CLASS *obj)
	{
 		Z_ASSERT(obj);
		this->add(obj->get_map_key(),obj); 
		init_child(obj);
		return *this;
	}


};




#endif
