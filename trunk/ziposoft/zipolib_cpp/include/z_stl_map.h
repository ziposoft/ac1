/*________________________________________________________________________

 z_stl_map_h

________________________________________________________________________*/

#ifndef z_stl_map_h
#define z_stl_map_h

#include "zipolib_c/include/zipo.h"
#include "zipolib_c/include/z_debug.h"

#include <map>



template <class KEY, class DATA > class z_stl_map : public std::map<KEY,DATA> 
{
public:
	typedef typename std::map<KEY,DATA>::iterator iter;
	typedef typename std::map<KEY,DATA> m;
	bool get_next(iter& i,KEY& key_out,DATA& data_out)
	{
		if(i==m::end()) return false;
		data_out=i->second;
		key_out=i->first;
		i++;
		return true;
	}
	bool get_next(iter& i,DATA& data_out)
	{
		if(i==m::end()) return false;
		data_out=i->second;
		i++;
		return true;
	}
	bool exists(const KEY& key)
	{
		iter i=find(key);
		if(i==m::end()) return false;
		return true;
	}
	bool get(const KEY& key,DATA& data_out)
	{
		iter i=find(key);
		if(i==m::end()) return false;
		data_out=i->second;
		return true;
	}
	bool pop(const KEY& key,DATA& data_out)
	{
		iter i=find(key);
		if(i==m::end()) return false;
		data_out=i->second;
		erase(i);
		return true;
	}
	bool del(const KEY& key)
	{
		iter i=find(key);
		if(i==m::end()) return false;
		erase(i);
		return true;
	}
	bool pop_first( KEY& key,DATA& data_out)
	{
		iter i=m::begin();
		if(i==m::end()) return false;
		key=i->first;
		data_out=i->second;
		erase(i);
		
		return true;
	}
};

struct ctext_less_than
	: public std::binary_function<ctext, ctext, bool>
{	// functor for operator>
bool operator()(const ctext& _Left, const ctext& _Right) const
	{	// apply operator> to operands
	return (strcmp(_Left,_Right)<0);
	}
};

template <class ITEM_CLASS > class z_stl_obj_map 
: public std::map<ctext,ITEM_CLASS,ctext_less_than> 
{
public:
	typedef typename std::map<ctext,ITEM_CLASS,ctext_less_than>::iterator iter;
	typedef typename std::map<ctext,ITEM_CLASS,ctext_less_than> m;
	bool get_next(ctext& key_out,ITEM_CLASS& data_out,iter& i)
	{
		if(i==m::end()) return false;
		data_out=i->second;
		key_out=i->first;
		i++;
		return true;
	}
	bool get_next(iter& i,ITEM_CLASS& data_out)
	{
		if(i==m::end()) return false;
		data_out=i->second;
		i++;
		return true;
	}
	bool get(ctext key,ITEM_CLASS& data_out)
	{
		iter i=m::find(key);
		if(i==m::end()) return false;
		data_out=i->second;
		return true;
	}
	bool del(ctext key)
	{
		iter i=m::find(key);
		if(i==m::end()) return false;
		m::erase(i);
		return true;
	}
};

typedef  std::map<ctext,void*,ctext_less_than>::iterator z_map_iterator;
class z_map_iter
{
public:
	z_map_iter()
	{
		key=0;
	};
	void reset()
	{
		key=0;
	};
	z_map_iterator i;
	ctext key;
};

template <class ITEM_CLASS > class z_map 
: public std::map<ctext,void*,ctext_less_than> 
{
public:
	typedef typename std::map<ctext,void*,ctext_less_than> m;
	
    virtual z_map<ITEM_CLASS> & operator << (ITEM_CLASS *x)
    {
		Z_ASSERT(x);
        add(x->get_key(),x);
        return *this;
    };

	bool add(ctext key_in,ITEM_CLASS* item)
	{
		size_t len=strlen(key_in);
		char* s=new char[len+1];
		
		strncpy(s,key_in,len);
		s[len]=0;
		(*this)[s]=item;
		
		return true;
	}
	void clear_all()
	{ 
		m::clear(); 
	}
	ITEM_CLASS* get_current(z_map_iter& iter)
	{

		if(iter.key==0)
			iter.i=m::begin();
		if(iter.i==m::end()) return 0;
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
	ITEM_CLASS* get(ctext key)
	{
		z_map_iterator i=m::find(key);
		if(i==m::end()) return 0;
		void* data_out=i->second;
		return (ITEM_CLASS*)data_out;
	}

	bool del(ctext key)
	{
		z_map_iterator i=m::find(key);
		if(i==m::end()) return false;
		m::erase(i);
		return true;
	}
};



/*
WARNING - this is only for static const char*. hard coded strings.
*/

template <class ITEM_CLASS > class z_map_ctext 
: public std::map<ctext,void*,ctext_less_than> 
{
public:
	typedef typename std::map<ctext,void*,ctext_less_than> m;
	z_map_iter _internal_iter; //warning! only use for simple loops!
	bool add(ctext key_in,ITEM_CLASS* item)
	{
		(*this)[key_in]=(void*)item;
		return true;
	}

	ITEM_CLASS* get_next(z_map_iter& iter)
	{

		if(iter.key==0)
			iter.i=m::begin();
		if(iter.i==m::end()) return 0;
		void* data_out=iter.i->second;
		iter.key=iter.i->first;
		iter.i++;
		return (ITEM_CLASS*)data_out;
	}
	ITEM_CLASS* get(ctext key)
	{
		z_map_iterator i=m::find(key);
		if(i==m::end()) return 0;
		void* data_out=i->second;
		return (ITEM_CLASS*)data_out;
	}
	void reset_iter()
	{
		_internal_iter.reset();
	}
	ITEM_CLASS* get_next(ctext& key)
	{
		ITEM_CLASS* i= get_next(_internal_iter);
		key=_internal_iter.key;
		return i;

	}
	ITEM_CLASS* get_next()
	{
		return get_next(_internal_iter);
	}
	bool del(ctext key)
	{
		z_map_iterator i=m::find(key);
		if(i==m::end()) return false;
		m::erase(i);
		return true;
	}
};
#endif
