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




char* z_itobase(I64 value, char* buf, int base, int* len, int _signed)
{
    // internal conversion routine: converts the value to a string 
    // at the end of the buffer and returns a pointer to the first
    // character. this is to get rid of copying the string to the 
    // beginning of the buffer, since finally the string is supposed 
    // to be copied to a dynamic string in itostring(). the buffer 
    // must be at least 65 bytes long.

    int i = *len;
	int in_len=*len;
    int neg = 0;
    static char* digits = 
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    U64 v = value;
    char* pdigits;
	pdigits = digits ;   // start from '0'
    
    buf[i] = 0;

    if (_signed && base == 10 && value < 0)
    {
        v = -value;
        // since we can't handle the lowest signed 64-bit value, we just
        // return a built-in string.
        if ( (I64)v < 0)   // the LLONG_MIN negated results in the same value
        {
            *len = 20;
            return "-9223372036854775808";
        }
        neg = 1;
    }

    do
    {
        buf[--i] = pdigits[ (U32)(v % base)];
        v /= base;
    } while ((v > 0)&&(i));

    if ((neg)&&(i))
        buf[--i] = '-';

    *len = in_len - i;
    return buf + i;
}
#define convert_buffer_size 32
char convert_buffer[convert_buffer_size];

void z_itobase2(char* result, I64 value, int base, int buff_length, char padchar, int _signed)
{
    int reslen;
	char* p;
	if(buff_length>=convert_buffer_size)
		buff_length=convert_buffer_size;

    reslen=buff_length;
    result[buff_length-1]=0;
	buff_length--;
    if (base < 2 || base > 62)
    {
		result="";
        return;
    }

    p = z_itobase(value, convert_buffer, base, &reslen, _signed);

    if (buff_length > reslen)
    {
		int neg;
        if (padchar == 0)
        {
            // default pad char
            if (base == 10)
                padchar = ' ';
            else
                padchar = '0';
        }

		neg = (*p == '-'? 1 :0);
        buff_length -= reslen;
        memset(result + neg, padchar, buff_length);
        memcpy(result + buff_length + neg, p + neg, reslen - neg);
        if (neg)
            *result = '-';
    }
    else 
        memcpy(result, p, reslen);
       


}


char * z_strstrcase (
        const char * str1,
        const char * str2
        )
{
        char *cp = (char *) str1;
        char *s1, *s2;
        while (*cp)
        {
                s1 = cp;
                s2 = (char *) str2;
                while ( *s1 && *s2 )
                {
                    int diff=(*s1-*s2);
                    if(!diff) goto NEXTCHAR;
                    if((diff==('A'-'a'))&&((*s1>='A')||(*s1<='Z')))goto NEXTCHAR;
                    if((diff==('a'-'A'))&&((*s1>='a')||(*s1<='z')))goto NEXTCHAR;
                    break;
                    
                NEXTCHAR:
                    s1++, s2++;        
                }
                if (!*s2) return(cp);
                cp++;
        }
        return(NULL);
}

