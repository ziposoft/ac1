/*________________________________________________________________________

 z_stl_list_h

________________________________________________________________________*/

#ifndef z_stl_list_h
#define z_stl_list_h
#include "zipolib_c/include/zipo.h"


#include <list>
#include <vector>


template <class ITEM_CLASS> class z_stl_list : public std::list<ITEM_CLASS*>
{
public:
	typedef typename std::list<ITEM_CLASS*>::iterator iter; //FREE BSD needs this
	typedef typename std::list<ITEM_CLASS*> l;; //FREE BSD needs this
    template <class ID> ITEM_CLASS* obj_find(const ID & id) 
    {
        iter i;
		for(i=l::begin();i!=l::end();i++)
		{
			
			if((*i)->compare_id(id)==0) 
				return *i;
		}
		return 0;
	};	

};

template <class ITEM_CLASS> class z_stl_obj_vector : public std::vector<ITEM_CLASS*>
{
public:
	typedef typename std::vector<ITEM_CLASS*>::iterator iter;
	typedef typename std::vector<ITEM_CLASS*> l;; //FREE BSD needs this

    virtual z_stl_obj_vector<ITEM_CLASS> & operator << (ITEM_CLASS *x)
    {
        push_back(x);
        return *this;
    };
    template <class ID> ITEM_CLASS* get_next_obj(const ID & id,size_t &  start) 
    {
        size_t i;
		for(i=start;i<std::vector<ITEM_CLASS*>::size();i++)
		{
			ITEM_CLASS* item=(*this)[i];
			if(item)
			{
				if(item->compare_id(id)) 
				{
					start=i+1;
					return item;
				}
			}
		}
		return 0;
	};	
    template <class ID,class ITEM_SUB_CLASS> ITEM_SUB_CLASS* get_next_obj_type(const ID & id,size_t &  start) 
    {
        size_t i;
		for(i=start;i<std::vector<ITEM_CLASS*>::size();i++)
		{
			ITEM_CLASS* item=(*this)[i];
			if(item)
			{
				ITEM_SUB_CLASS* sc_item=dynamic_cast<ITEM_SUB_CLASS*>(item);
				if(sc_item)
					if(sc_item->compare_id(id)) 
					{
						start=i+1;
						return sc_item;
					}
			}
		}
		return 0;
	};	
};


#endif 
