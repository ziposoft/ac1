/*________________________________________________________________________

 z_string_h

________________________________________________________________________*/


#ifndef z_string_h
#define z_string_h

#include "zipolib/include/zipo.h"
#include "zipolib/include/z_debug.h"
#include <string>
class z_file;
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

	
	const z_string & operator = (const wchar_t* wstr) ;
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


	void stringize(z_string &append_to);


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
void z_str_escape(ctext in,std::string& out);
void z_str_unescape(std::string& in,std::string& out);

extern const z_string z_string_null;





#endif

