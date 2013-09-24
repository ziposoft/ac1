//________________________________________________________________________/////////////////////////
//
// z_type_converter
//
//________________________________________________________________________/////////////////////////
#ifndef z_type_converter_h
#define z_type_converter_h

#include "zipolib_cpp/include/z_string.h"

//TODO - this is terrible, get rid of this!

/////////////////////////////////////////////////////////////
//////////////////////////// z_type_converter ////////////////




bool  z_convert(const double &from, char* to,int size,ctext format=0); 
bool  z_convert(const int &from, char* to,int size,ctext format=0); 
bool  z_convert(const unsigned int &from, char* to,int size,ctext format=0); 
bool  z_convert(const  unsigned long &from, char* to,int size,ctext format=0);
bool  z_convert(const z_string &from,unsigned int& to,ctext format=0);

#endif

