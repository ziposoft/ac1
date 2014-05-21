/*________________________________________________________________________

 z_stl_vector_h

________________________________________________________________________*/

#ifndef z_stl_vector_h
#define z_stl_vector_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_dbg.h"

#include <vector>

class z_obj_vector_base
{
public:
	virtual void clear()=0;
	virtual size_t size() const=0;
	virtual void* get_void(size_t i) const=0;
	virtual void add_void(void* o)=0;

};

template <class ITEM_CLASS > class z_obj_vector  : public z_obj_vector_base
{
protected:
	std::vector<ITEM_CLASS*>  _vector;
public:
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
 	void* get_void(size_t i) const
	{
		return (void*)get(i);
	}

};
template <class ITEM_CLASS > class z_obj_vector_map  : public z_obj_vector<ITEM_CLASS>
{
public:
 
	ITEM_CLASS* get_by_name(ctext t)   const
	{
		size_t i=0;
		while(i<this->size())
		{
			ITEM_CLASS* obj= get(i);
			if(strcmp(obj->get_map_key(),t)==0)
				return obj;
			i++;
		}
		return 0;
	}
};
#endif
