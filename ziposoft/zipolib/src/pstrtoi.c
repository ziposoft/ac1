/*
*  Code originally from:
*
*  C++ Portable Types Library (PTypes)
*  Version 2.0.2  Released 17-May-2004
*
*  Copyright (C) 2001-2004 Hovik Melikyan
*
*  http://www.melikyan.com/ptypes/
*
*/

#include "zipo.h"



I64 z_stringtoi(const char* p)
{
	I64 r;
	I64 t;
	if (p == 0)
		return -1;
	if (*p == 0)
		return -1;

	r = 0;
	do 
	{
		char c = *p++;
		if (c < '0' || c > '9')
			return -1;              // invalid character
		t = r * 10;
		if (t < r)
			return -1;              // overflow
		t += c - '0';
		if (t < r)
			return -1;              // overflow
		r = t;
	} while (*p != 0);

	return r;
}




U64 z_stringtoue(const char* str, int base)
{
	U64 result = 0;
	U64 t;
	const char* p;
	if (str == 0)
	{
		//throw_conv(str);
		//Z_ERROR("conversion",0);  //TODO!!
		return 0;
	}
	if (*str == 0 )
	{
		return 0; //an empty string = 0
	}
	if ( base < 2 || base > 64)
	{
		//throw_conv(str);
		//Z_ERROR("conversion"); //TODO!!
		return 0;
	}

	p = str;

	do 
	{
		int c = *p++;

		if (c >= 'a')
		{
			// for the numeration bases that use '.', '/', digits and
			// uppercase letters the letter case is insignificant.
			if (base <= 38)
				c -= 'a' - '9' - 1;
			else  // others use both upper and lower case letters
				c -= ('a' - 'Z' - 1) + ('A' - '9' - 1);
		}
		else if (c > 'Z')
		{
			//throw_conv(str);
			//Z_ERROR("conversion");
			return 0;
		}
		else if (c >= 'A')
			c -= 'A' - '9' - 1;
		else if (c > '9')
		{
			//throw_conv(str);
			//Z_ERROR("conversion"); //TODO!!
			return 0;
		}

		c -= (base > 36) ? '.' : '0';
		if (c < 0 || c >= base)
		{
			//throw_conv(str);
			//Z_ERROR("conversion");
			return 0;
		}
		t = result * (U32)(base);
		if (t / base != result)
		{
			//throw_overflow(str);
			//Z_ERROR("conversion");
			return 0;
		}
		result = t;
		t = result + (U32)(c);
		if (t < result)
		{
			//throw_overflow(str);
			//Z_ERROR("conversion");
			return 0;
		}
		result = t;
	} while ((*p != 0)&&(*p != '_'));

	return result;
}


I64 z_stringtoie(const char* str)
{
	int neg ;
	U64 result;
	if (str == 0)
	{
		//throw_conv(str);
		//Z_ERROR("conversion"); //TODO!!
		return 0;
	}
	neg = (*str == '-'? 1:0);
	result = z_stringtoue(str + neg, 10);
	if (
		result > ((U64)(I64_MAX) + neg)
		)
	{
		//throw_overflow(str);
		//Z_ERROR("conversion"); //TODO!!
		return 0;
	}
	if (neg)
		return - (I64)(result);
	else
		return (I64)(result);
}

