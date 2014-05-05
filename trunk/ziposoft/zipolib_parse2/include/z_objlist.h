// z_objlist.h: interface for the z_objlist class.
//
//////////////////////////////////////////////////////////////////////

#ifndef z_objlist_h
#define z_objlist_h
#include <stddef.h>



template <class ITEM_CLASS> class z_list_entry
{
public:
    ITEM_CLASS *_item;
    z_list_entry<ITEM_CLASS> *_InFront;
    z_list_entry<ITEM_CLASS> *_Behind;
    z_list_entry(ITEM_CLASS *item) {
        _InFront=0;
        _Behind=0;
        _item=item;
    };
};
static size_t dummy_cookie;
//________________________________________________________________________
//
//                  z_objlist
//
//________________________________________________________________________
template <class ITEM_CLASS,bool OWNER=false,class ENTRY=z_list_entry<ITEM_CLASS>  > class z_objlist 
{
public:
    typedef void (ITEM_CLASS::*EntryClassMemberFunc)();
    //typedef ITEM_CLASS OBJ;
    typedef ENTRY* ITER;
private:

    ENTRY* _FrontOfLine;
    ENTRY* _BackOfLine;
    size_t _count;
 public:
    bool _bOwner;
    z_objlist()
    {
#ifdef DEBUG
		_safetycounter=0;
#endif
		_FrontOfLine=0;
        _count=0;
        _BackOfLine=0;
        _bOwner=OWNER;;
    }
    /*****************************************************************/
    virtual ~z_objlist()
    {
    #ifndef _CGI
        if(_bOwner)
        DeleteAll();
    #endif
    }
    /*****************************************************************/
    ITEM_CLASS* PullItem(ITEM_CLASS* x)
    {
        ENTRY* it=FindEntryByItem(x);
        if (it) return PullEntry(it);
        return 0;
    }
    /*****************************************************************/
    void DeleteObject(ITEM_CLASS* x)
    {
        ENTRY* it=FindEntryByItem(x);
        if (it)
        {
            ITEM_CLASS* item=PullEntry(it);
            if(_bOwner)
                if (item) delete item;
        }
    }
    /*****************************************************************/
    void ForEach0(EntryClassMemberFunc func) 
    {
        size_t iter=0;
        ITEM_CLASS* item;
        while((item=GetNext(iter)))
        {
            (item->*func)();
        }
    }
    /*****************************************************************/
    template <class PARAM1> void ForEach1(void (ITEM_CLASS::*func)(PARAM1 p ),PARAM1 param1) 
    {
        size_t iter=0;
        ITEM_CLASS* item;
        while((item=GetNext(iter)))
        {
            (item->*func)(param1);
        }
    }
    /*****************************************************************/
    template <class PARAM1,class PARAM2> 
		void ForEach2(
			void (ITEM_CLASS::*func)(PARAM1 p1,PARAM2 p2 ),
			PARAM1 param1,PARAM2 param2) 
    {
        size_t iter=0;
        ITEM_CLASS* item;
        while((item=GetNext(iter)))
        {
            (item->*func)(param1,param2);
        }
    }
    /*****************************************************************/
    void DeleteAll()     {
        ITEM_CLASS* item;
        while (_FrontOfLine)           {
            item=PullEntry(_FrontOfLine);
            if(_bOwner) delete item;
        }
    }
    /*****rfoo************************************************************/
    ITEM_CLASS* GetLast()     
	{ 
		if(_BackOfLine) return _BackOfLine->_item;
		else return 0;
    }
    /*****************************************************************/
    template <class TYPE> TYPE* FindNext_t(size_t &cookie=dummy_cookie)    
	{
		ITEM_CLASS* item_base;
		TYPE* item_t;
		dummy_cookie=0;
		
		while((item_base=GetNext(cookie)))
		{
			item_t=dynamic_cast<TYPE*>(item_base);
			if(item_t) return item_t;
		}
		return 0;
    }
    /*****************************************************************/
    ITEM_CLASS* GetNext(size_t &cookie=dummy_cookie)    
	{
        ENTRY* i=(ENTRY*)cookie;
		dummy_cookie=0;//if someone iterates with the dummycookie it is an infinite loop

#ifdef DEBUG
        if(i)
		{
			_safetycounter++;
			if (_safetycounter>10000) 
			{
				i=0;
				Z_ASSERT(0);
				return 0;
			}
		}
		else _safetycounter=0;

#endif
        if (i==0) 
		{	
			i=_FrontOfLine;

		}
		else 
		{
			i=i->_Behind;
		}
		cookie=(size_t)i;
        if (!i) return 0;
        return i->_item;
    }
    /*****************************************************************/
    //virtual z_objlist & InsertAfter (ENTRY* left,ITEM_CLASS *x);
    /*****************************************************************/
    ITEM_CLASS* PullEntry(ENTRY* x)
    {
        if (x->_InFront)  
            (x->_InFront)->_Behind=x->_Behind;
        else
            _FrontOfLine=x->_Behind;
        if (x->_Behind) 
            (x->_Behind)->_InFront=x->_InFront;
        else
            _BackOfLine=x->_InFront;
        _count--;
        if (!_count) _FrontOfLine=0;
        ITEM_CLASS *object=x->_item;
        delete x;
        return object;
    }
    /*****************************************************************/
    ITEM_CLASS* PopTop() { if (_FrontOfLine) return PullEntry(_FrontOfLine); else return 0; };
    /*****************************************************************/
    ITEM_CLASS* PopEnd() { if (_BackOfLine) return PullEntry(_BackOfLine); else return 0; };
    /*****************************************************************/
    void Clean(){ 	while (_FrontOfLine) PullEntry(_FrontOfLine); }
    /*****************************************************************/
    int get_count()  { 	return (int)_count; }
    /*****************************************************************/
    /*
	ITEM_CLASS* PullEntryByIndex(size_t index)
    {
        ITEM_CLASS *x=GetEntry(index);
        if (!x) return 0;
        return PullEntryByItem(x);
    }*/
    /*****************************************************************/
    void AddToEnd(ITEM_CLASS *x) 
    { 
        if (x==0) return;
        #if 0
        if (FindEntryByItem(x)) 
        {
             _asm int 3;
        }
        #endif
        ENTRY* newEntry=znew z_list_entry<ITEM_CLASS>(x);
        if (!_FrontOfLine) 
        {
            _FrontOfLine=newEntry;
            _BackOfLine=newEntry;
        }
        else 	
        {
            _BackOfLine->_Behind=newEntry;
            newEntry->_InFront=_BackOfLine;
            _BackOfLine=newEntry;
        }
        _count++;
    }
    /*****************************************************************/
    void AddToStart(ITEM_CLASS *x) 
    { 
        if (x==0) return;

        ENTRY* newEntry=znew z_list_entry<ITEM_CLASS>(x);
        if (!_BackOfLine) 
        {
            _FrontOfLine=newEntry;
            _BackOfLine=newEntry;
        }
        else 	
        {
            _FrontOfLine->_InFront=newEntry;
            newEntry->_Behind=_FrontOfLine;
            _FrontOfLine=newEntry;
        }
        _count++;
    }
    /*****************************************************************/
    virtual z_objlist<ITEM_CLASS,OWNER> & operator << (ITEM_CLASS *x)
    {
        AddToEnd(x);
        return *this;
    };
    /*****************************************************************/
    virtual z_objlist<ITEM_CLASS,OWNER> & operator , (ITEM_CLASS *x)
    {
        AddToEnd(x);
        return *this;
    };
    /*****************************************************************/
    ITEM_CLASS*  operator [] (int i)
    {
        return GetItemByIndex(i);
    };

    /*****************************************************************/
    ITEM_CLASS* GetItemByIndex(size_t index)
    {
        ENTRY* entry=_FrontOfLine;	
        while(index)
        {
            if (!entry) break;
            entry=entry->_Behind;
            index--;
        }
		if (entry) return entry->_item;
        return 0;
    }

#if 0 //BLOAT
    /*****************************************************************/
    /*****************************************************************/
    //  dont know if this works
    z_objlist<ITEM_CLASS> & InsertBefore(ENTRY* GuyOnRight,ITEM_CLASS *x)
    {
        ENTRY* GuyOnLeft,*newEntry=znew z_list_entry<ITEM_CLASS>(x);
        if (!GuyOnRight) 
        {
            GuyOnLeft=_BackOfLine;
            _BackOfLine=newEntry;
            _BackOfLine->_InFront=GuyOnLeft;
            if (GuyOnLeft)
                GuyOnLeft->_Behind=newEntry;
            else
                _FrontOfLine=newEntry;
        }
        else 	
        {
            GuyOnLeft=GuyOnRight->_InFront;
            GuyOnRight->_InFront=newEntry;
            newEntry->_InFront=GuyOnLeft;
            newEntry->_Behind=GuyOnRight;
            if (GuyOnLeft)
            {
                GuyOnLeft->_Behind=newEntry;
            }
            else
                _FrontOfLine=newEntry;
        }
        _count++;
        return *this;
    }
    ENTRY* FindEntryByItem(ITEM_CLASS *x)
    {
        ENTRY* it=0;
        ResetIndex(it);
        while(GetIndex(it))
        {
            if (it->_item==x) return it;
            GetNext(it);
        }
        return 0;
    }
	
    //  dont know if this works
    z_objlist<ITEM_CLASS> & Insert(ITEM_CLASS *x,size_t index)
    {
        ENTRY* GuyOnLeft,*newEntry=znew z_list_entry<ITEM_CLASS>(x);
        if (!GuyOnRight) 
        {
            GuyOnLeft=_BackOfLine;
            _BackOfLine=newEntry;
            _BackOfLine->_InFront=GuyOnLeft;
            if (GuyOnLeft)
                GuyOnLeft->_Behind=newEntry;
            else
                _FrontOfLine=newEntry;
        }
        else 	
        {
            GuyOnLeft=GuyOnRight->_InFront;
            GuyOnRight->_InFront=newEntry;
            newEntry->_InFront=GuyOnLeft;
            newEntry->_Behind=GuyOnRight;
            if (GuyOnLeft)
            {
                GuyOnLeft->_Behind=newEntry;
            }
            else
                _FrontOfLine=newEntry;
        }
        _count++;
        return *this;
    }
#endif
#ifdef DEBUG
    size_t _safetycounter;		
#endif
};
#if 0
//________________________________________________________________________
//
//                  objMap
//
// item class must define compare_id() function
//
//________________________________________________________________________
template <class ITEM_CLASS,class ID_CLASS,bool OWNER=true> class z_objmap : public z_objlist<ITEM_CLASS, OWNER>
{
public:
    typedef z_list_entry<ITEM_CLASS> ENTRY;

    ENTRY* objFindEntry(const ID_CLASS & id) 
    {
        size_t iter=0;
        ITEM_CLASS* item;
        while((item=GetNext(iter)))
        {
            (item->*func)(param1);
        }
		ENTRY* it=0;
        ITEM_CLASS* item=ResetIndex(it);
        while(item)
        {
            if(item->compare_id(id)==0) return it;
            item=GetNext(it);
        }
        return 0;
    };
    ITEM_CLASS* objFind(const ID_CLASS & id) 
    {
        ENTRY* it=objFindEntry(id);
        if(it) return it->_item;
        return 0;
    };
    ITEM_CLASS* objDel(const ID_CLASS & id) 
    {
        ENTRY* it=objFindEntry(id);
        if(it) return PullEntry(it);
        return 0;
    };

    ITEM_CLASS*  operator [] (const ID_CLASS & id) { return objFind(id); }

};

//________________________________________________________________________
//
//                  objFactory
//
//________________________________________________________________________
#define PTR &

template <class BASE,class ID_CLASS> class objCreatorBase
{
public:
    virtual BASE* createInstance()=0;
    virtual bool compare_id(const ID_CLASS& id)=0;
};


template <class BASE,class ID_CLASS> class objFactory : public z_objmap< objCreatorBase<BASE,ID_CLASS>,ID_CLASS >
{
    objFactory(){};
public:
    static objFactory &instance()
    {
        // Note that this is not thread-safe!
        static objFactory theInstance;
        return theInstance;
    };
    BASE* create(const ID_CLASS & ClassId) 
    {
        BASE* ret=0;
        objCreatorBase<BASE,ID_CLASS> *creator=objFind(ClassId);
        if(creator) return creator->createInstance();
        return 0;
    }
    BASE* create(ID_CLASS & ClassId,ID_CLASS& Default) 
    {
        BASE* ret=create(ClassId);
        if (!ret) ret=create(Default);
        return ret;
    }
};
template <class BASE,class TYPE,class ID_CLASS> class objCreator : public objCreatorBase<BASE,ID_CLASS>
{
public:
    virtual BASE* createInstance() {   return znew TYPE();  }
    objCreator()     {   objFactory< BASE,ID_CLASS >::instance() << this;   }
    bool compare_id(const ID_CLASS& id)  { return TYPE::StaticCompareId(id); } 

};

/*

// dont know if this works
template <class ITEM_CLASS> 
z_objlist<ITEM_CLASS> & z_objlist< ITEM_CLASS >::InsertAfter(ENTRY* GuyOnLeft,ITEM_CLASS *x)
{
    ENTRY* GuyOnRight;
    ENTRY* newEntry=znew z_list_entry<ITEM_CLASS>(x);


    if (!GuyOnLeft) 
    {
        GuyOnRight=_FrontOfLine;
        _FrontOfLine=newEntry;
        _FrontOfLine->_Behind=GuyOnRight;
        GuyOnRight->_InFront=newEntry;
        if (GuyOnRight)
        {
            GuyOnRight->_InFront=newEntry;
        }
        else
            _BackOfLine=newEntry;
    }
    else 	
    {
        GuyOnRight=GuyOnLeft->_Behind;
        GuyOnLeft->_Behind=newEntry;
        newEntry->_InFront=GuyOnLeft;
        newEntry->_Behind=GuyOnRight;
        if (GuyOnRight)
        {
            GuyOnRight->_InFront=newEntry;
        }
        else
            _BackOfLine=newEntry;
    }
    _count++;
    return *this;
}

*/



#endif 
#endif 
