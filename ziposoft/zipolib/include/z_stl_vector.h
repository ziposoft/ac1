/*________________________________________________________________________

 z_stl_vector_h

________________________________________________________________________*/

#ifndef z_stl_vector_h
#define z_stl_vector_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_dbg.h"

#include <vector>



template <class ITEM_CLASS > class z_obj_vector 
{
protected:
	std::vector<ITEM_CLASS*>  _vector;
public:

	void add(ITEM_CLASS* item)
	{
		_vector.push_back(item);
		return ;
	}
	void clear()
	{ 
		_vector.clear(); 
	}

 	size_t size()
	{ 
		return _vector.size(); 
	}
	
	ITEM_CLASS* operator [](size_t i)
	{
		if(i>=size())
			return 0;
		return _vector[i];
	}
	ITEM_CLASS* get(size_t i)
	{
		if(i>=size())
			return 0;
		return _vector[i];
	}


};

#endif
