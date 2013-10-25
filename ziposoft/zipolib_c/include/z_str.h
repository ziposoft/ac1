/*________________________________________________________________________

z_str.h

________________________________________________________________________*/

#include "zipolib_c/include/zipo.h"
#ifndef z_str_h
#define z_str_h
#ifdef	__cplusplus
extern "C" {
#endif

int vsnprintf(char *str, size_t size, ctext format, va_list ap);

char z_to_upper(char c);
int z_str_same(utf8 s1,utf8 s2);
int z_str_check3(utf8 id,utf8 s1,utf8 s2,utf8 s3);


I64 z_stringtoi(const char* p);
U64 z_stringtoue(const char* str, int base);
char* z_itobase(I64 value, char* buf, int base, int* buff_length, int _signed);
void z_itobase2(char* buf, I64 value, int base, int buff_length, char padchar, int _signed);



char * z_strstrcase (
        const char * str1,
        const char * str2
        );


#ifdef	__cplusplus
}
#endif
#endif

