//________________________________________________________________________/////////////////////////
//
// z_string
//
//________________________________________________________________________/////////////////////////
#ifndef z_string_h
#define z_string_h
//________________________________________________________________________/////////////////////////
//
// Basic defines 
//
//________________________________________________________________________/////////////////////////
#include "zipolib/include/zipo.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#ifdef UNIX
#include <stdarg.h>
#define stricmp strcasecmp
#endif
#include "zipolib/include/z_objlist.h"

class z_file;
//TODO BUG BUG
ctext STR(U32 i);
static const char* empty_cstr="";
class z_string : public std::string
{
public:
    z_string(ctext x)
    {
        if (x) assign(x);
    }
    z_string()
    {
		assign(empty_cstr);
    }	
	z_string(U32 i) ;
	void operator <<  (z_file &stream); 

	z_string(int i) ;	
    U32 GetDecVal() const;
	const z_string & operator = (const U32 i) ;
	const z_string & operator = (const int i) ;
	const z_string & operator = (std::string s) ;
	bool operator ! () 
	{ 
		return (*this == "");    
	};
	operator bool () const
	{
		return (*this != "");    
	}
	bool hex(const int i) {   return Format("%x",i);      }
	z_string & operator += (const z_string &s) { append(s);return *this; }
	z_string & operator += (const int i)  { return append_int(i); }
	z_string & operator += (const U32 i)  { return append_int(i); }
	z_string & operator += (const char i)  { append(1,i);return *this; }
	z_string & operator += (const U8 i)  { append(1,(char)i);return *this; }

    friend z_string operator+ (const z_string&, const z_string&);
    friend z_string operator+ (const z_string&, const char*);
    friend z_string operator+ (const char*, const z_string&);

	bool Format(ctext pFormat,...) ;
    bool FormatV(ctext pFormat,va_list ArgList) ;
    operator ctext() const 
    {
        return this->c_str();
    }

    char &operator  [] (int i)  
    {
        return at(i);
    }

    ~z_string()
    {

    }
	void increment_name();
	void append_xml_encoded(ctext x);
	void xml_decode(ctext x);
	
    z_string & operator << (char x)
    {
        append(1,x);
        return *this;
    }
    z_string & operator << (ctext x)
    {
        if (x) append(x);
        return *this;
    }
    z_string & operator << (const z_string& x)  { append(x);  return *this;  }
    z_string & append_int (const int i);
    z_string & operator << (const int i);    
	z_string & operator << (U32 i);
    z_string & operator = (ctext x)
    {
        if (x) assign(x);
        return *this;
    }
};
extern const z_string z_string_null;
class z_str_map : public std::map<z_string,z_string>
{
public:
   

};
class z_strlist : public std::vector<z_string>
{
public:
    z_strlist()
    {

    }
    virtual ~z_strlist()
    {

    }
    int del(int i)
    {
        erase(begin()+i);
        return 0;
    }
    int find(const char* str)
    {
        iterator i;
        i=std::find(begin(),end(),str);
        if(i==end()) return -1;
        return (int)(i-begin());

    }
    int find(const z_string &str)
    {
        return find(str.c_str());
    }
	int break_string(ctext s,char c);
	virtual z_strlist & operator << (z_string& s) { push_back(s); return *this; }
	virtual z_strlist & operator << (ctext s) { push_back(s); return *this; }

};
/*
template <class ITEM_CLASS,bool OWNER=true> class z_named_list : public z_objmap<ITEM_CLASS,z_string,OWNER>
{
public:
	ITEM_CLASS*  operator [] (const char* id) { return z_objmap<ITEM_CLASS,z_string,OWNER>::objFind(id); }
       
	ITEM_CLASS*  operator [] (int i) {  return z_objmap<ITEM_CLASS,z_string,OWNER>::GetItemByIndex(i); }

};
*/

int z_parse_url_query(ctext buff,size_t len,z_str_map& vars);

#endif

