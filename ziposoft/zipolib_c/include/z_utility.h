/*________________________________________________________________________

z_utility.h

________________________________________________________________________*/


#include "zipolib_c/include/zipo.h"
#ifndef z_utility_h
#define z_utility_h
#ifdef	__cplusplus
extern "C" {
#endif




char z_to_upper(char c);
int z_str_same(utf8 s1,utf8 s2);
int z_str_check3(utf8 id,utf8 s1,utf8 s2,utf8 s3);
utf8 z_get_filename_from_path(utf8 fullpath);

U32 z_byte_swap32(U32 x);
U16 z_byte_swap16(U16 x);

I64 z_stringtoi(const char* p);
U64 z_stringtoue(const char* str, int base);
char* z_itobase(I64 value, char* buf, int base, int* buff_length, int _signed);
void z_itobase2(char* buf, I64 value, int base, int buff_length, char padchar, int _signed);



char * z_strstrcase (
        const char * str1,
        const char * str2
        );


int z_ipower(int val,int power);
#ifdef	__cplusplus
}
#endif
#endif

