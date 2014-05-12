/*________________________________________________________________________

 z_list_h

________________________________________________________________________*/



#ifndef z_list_h
#define z_list_h
/*_______________________________________________________________________*\



\*_______________________________________________________________________*/
#include "zipolib/include/zipolib.h"
#include "zipolib/include/z_string.h"
#include "zipolib/include/z_stl_map.h"

class z_file;
class z_strmap : public z_stl_map<z_string,z_string>
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
	void dump(z_file &out);
	virtual z_strlist & operator << (z_string& s) { push_back(s); return *this; }
	virtual z_strlist & operator << (ctext s) { push_back(s); return *this; }

};
int z_parse_url_query(ctext buff,size_t len,z_strmap& vars);


#endif

