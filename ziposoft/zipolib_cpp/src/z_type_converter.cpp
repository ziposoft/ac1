#include "zipolib_cpp.h"
#include "zipolib_cpp/include/z_string.h"
#include "zipolib_cpp/include/z_type_converter.h"
#include "zipolib_c/include/z_utility.h"
#include "zipolib_c/include/z_str.h"
#include <stdlib.h>
#ifdef UNIX
#include <stdio.h>
#endif


const ctext format_bool_yes_no="y";
const ctext format_bool_true_false="t";
const ctext format_bool_0_1="0";
const ctext format_u32_dec="%d";


z_string nullstring;
#ifndef WIN32
char *_gcvt(double number, size_t ndigit, char *buf)
{
     sprintf(buf, "%f", number); //TODO!!! ERRORRR!
     return buf;
}

/*
 *  itoa - convert integer (decimal) to ascii string for positive numbers
 *  	   only (we don't bother with negative numbers since we know we
 *	   don't use them).
 */

				/*
				 * How do we know that 16 will suffice?
				 * Because the biggest number that we will
				 * ever convert will be 2^32-1, which is 10
				 * digits.
				 */

int itoa(int val,char* buffer,int radix)
{
	return sprintf(buffer,"%d",val);
}

#endif
#if 1

///////////////////////////////////////////////////////////////////////
//////////////////////////// z_string ///////////////////////////////////

bool	        z_convert(const ctext& from,z_string& to,ctext format) 
{ 
	to=from;
	return true;
}
bool z_convert(const  ctext &from, char* to,int size,ctext format) 
{
	strncpy(to,from,size);
	return true;
}
bool z_convert(const char &from, char* to,int size,ctext format) 
{
	if(size)
	{
		*to=from;
		*(to+1)=0;
	}
	return true;
}
bool z_convert(const z_string &from, char* to,int size,ctext format) 
{
	strncpy(to,from.c_str(),size);
	return true;

}

///////////////////////////////////////////////////////////////////////
//////////////////////////// bool ///////////////////////////////////
///////////////////////////////////////////////////////////////////////
bool		z_convert 	(const ctext&  s,bool& b,ctext format) 
{ 
	b= ((strcmp (s,"true")==0)||(strcmp (s,"X")==0)||(strcmp (s,"on")==0)||(strcmp (s,"1")==0));//TODO case insensitive
	return true;
}
bool z_convert(const bool &val,z_string & s,ctext format)
{
	if(format== format_bool_yes_no)
	{
		if (val) s="yes"; else s="no";
		return true;
	}
	if (format== format_bool_true_false)
	{
		if (val) s="true"; else s="false";
		return true;
	}
	if (val) s="1"; else s="0";
	return true;
}
template <class TYPE>  bool convert(const bool &from, char* to,int size,ctext format) 
{
	if(size>5) 
	{
		if(from) strcpy(to,"true");
		else     strcpy(to,"false");
		return true;
	}
	return false;
}

bool		z_convert 	(const z_string&  s,bool& b,ctext format) 
{ 
	return z_convert(s.c_str(),b,format);
}
///////////////////////////////////////////////////////////////////////
//////////////////////////// float ///////////////////////////////////
///////////////////////////////////////////////////////////////////////
//float		z_get_def<float>() { return 0;}
bool		z_convert	(const ctext&  s,float& val,ctext format) 
{ 
	return sscanf((ctext)s,"%f",&val)==1;
}
bool z_convert(const float &val,z_string& t,ctext format)
{
    char buff[20];
    _gcvt(val,4,buff);
	t=buff;
	return true;
}
bool z_convert(const float &from,char* to,int size,ctext format) 
{
    if(size>14) //leave room for extra: -3.32e+211
		_gcvt(from,4,to);
	return true;
}
///////////////////////////////////////////////////////////////////////
//////////////////////////// double ///////////////////////////////////
///////////////////////////////////////////////////////////////////////
//double		z_get_def<double>() { return 0;}
bool z_convert(const ctext& s,double& val,ctext format) 
{ 
	return (sscanf((ctext)s,"%lf",&val)==1);
}
bool z_convert(const double &val,z_string &t,ctext format)
{
    char buff[20];
    _gcvt(val,4,buff);
	t=buff;
	return true;
}
bool z_convert(const double &from,char* to,int size,ctext format) 
{
    if(size>14) //leave room for extra: -3.32e+211
		_gcvt(from,4,to);
    //_gcvt_s(to,size,from,4);
	return true;
}
///////////////////////////////////////////////////////////////////////
//////////////////////////// unsigned long   ////////////////////////////
///////////////////////////////////////////////////////////////////////
bool z_convert(const ctext& s,unsigned long  &i,ctext format)
{ 
	i=(unsigned long )atoi(s);return true;
}
bool z_convert(const z_string& s,unsigned long &i,ctext  format)
{ 
	return z_convert(s.c_str(),i,format);
}

bool z_convert(const unsigned long &val, char* to,int size,ctext format)
{

	z_itobase2(to,val,10,size,' ',0);
	return true;  
}
bool z_convert(const unsigned long  &val,z_string &s,ctext format)
{
    char buff[20];
	z_itobase2(buff,val,10,19,' ',0);
    s=buff;
    return true;
}
///////////////////////////////////////////////////////////////////////
//////////////////////////// Unisgned int  ////////////////////////////
///////////////////////////////////////////////////////////////////////
bool z_convert(const ctext& s,U32 &i,ctext format)
{ 
	i=(U32)atoi(s);return true;
}
bool z_convert(const z_string& s,U32 &i,ctext format)
{ 
	return z_convert(s.c_str(),i,format);
}

bool z_convert(const U32 &val, char* to,int size,ctext format)
{

	z_itobase2(to,val,10,size,' ',0);
	return true;  
}
bool z_convert(const U32 &val,z_string &s,ctext format)
{
    char buff[20];
	z_itobase2(buff,val,10,19,' ',0);
    s=buff;
    return true;
}
///////////////////////////////////////////////////////////////////
//////////////////////////// int //////////////////////////////////
///////////////////////////////////////////////////////////////////
bool convert_ctext_to_int(const ctext& s,int& val,ctext format)
{
	if(!format) format="%d";
	if(s[0]=='0')
		if(s[1]=='x')
			return sscanf((ctext)s,"%x",&val)==1;
	return sscanf((ctext)s,format,&val)==1;
}
bool z_convert(const ctext& s,int& val,ctext format) 
{ 
	return convert_ctext_to_int(s,val,format);
}
bool z_convert(const z_string& s,int& val,ctext format) 
{ 
	return convert_ctext_to_int((ctext)s,val,format);
}
bool z_convert(const  int &val, char* to,int size,ctext format)
{
#ifdef BUILD_VSTUDIO
	_itoa_s(val,to,size,10);
#else
	itoa(val,to,10);
#endif
	return true;
}
bool z_convert(const int &val,z_string &s,ctext format)
{
    char buff[20];
#ifdef BUILD_VSTUDIO
	_itoa_s(val,buff,19,10);
#else
	itoa(val,buff,10);
#endif
    s=buff;
    return true;
}
bool z_convert(const bool& s,int& val,ctext format) 
{ 
	val=(s? 1:0);
	return true;
}

bool z_convert(const int &val,bool &s,ctext format)
{
	s=(val!=0);return true;
}
bool z_convert(const bool& s,U32& val,ctext format) 
{ 
	val=(s? 1:0);
	return true;
}

bool z_convert(const U32 &val,bool &s,ctext format)
{
	s=(val!=0);return true;
}


bool z_convert(const int &val,U32 &s,ctext format)
{
	s=val;return true;
}
///////////////////////////////////////////////////////////////////////
//////////////////////////// U64  ////////////////////////////
///////////////////////////////////////////////////////////////////////

bool z_convert(const z_string& s,U64 &i,ctext format)
{ 
	i=z_stringtoue(s.c_str(),10);return true;
}
bool z_convert(const ctext& s,U64 &i,ctext format)
{ 
	i=z_stringtoue(s,10);return true;
}
bool z_convert(const U64 &val, char* to,int size,ctext format)
{
	z_itobase2(to,val,10,size,' ',0);
	return true;  
}
bool z_convert(const U64 &val,z_string &s,ctext format)
{
    static char buff[24];
	int len=23;
	s=z_itobase(val,buff,10,&len,0);
    return true;
}
#endif

