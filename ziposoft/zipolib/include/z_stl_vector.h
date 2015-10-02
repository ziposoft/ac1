/*________________________________________________________________________

 z_stl_vector_h

________________________________________________________________________*/

#ifndef z_stl_vector_h
#define z_stl_vector_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_dbg.h"

#include <vector>



template <class ITEM_CLASS > class z_vector_obj
{
protected:
	std::vector<ITEM_CLASS*>  _vector;
public:
	virtual ~z_vector_obj()
	{
		destroy();
	}
	virtual void* get_next(size_t &i)
	{
		void* v=get(i);
		i++;
		return v;
	}
	virtual z_status remove(size_t i)
	{
		_vector.erase(_vector.begin()+i);
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


 	void* get_void(size_t i) 
	{
		return (void*)get(i);
	}
    virtual z_vector_obj<ITEM_CLASS> & operator << (ITEM_CLASS *x)
    {
		_vector.push_back(x);
		return *this;
    };


};


#endif
