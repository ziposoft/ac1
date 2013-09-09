//________________________________________________________________________/////////////////////////
//
// z_utility_h
//
//________________________________________________________________________/////////////////////////
#include "zipolib_c/include/zipo.h"
#ifndef z_utility_h
#define z_utility_h
#ifdef	__cplusplus
extern "C" {
#endif

char z_to_upper(char c);
int z_str_same(ctext s1,ctext s2);
int z_str_check3(ctext id,ctext s1,ctext s2,ctext s3);
ctext z_get_filename_from_path(ctext fullpath);

U32 z_byte_swap32(U32 x);
U16 z_byte_swap16(U16 x);

char* z_temp_buffer_get();
int   z_temp_buffer_size();
void  z_temp_buffer_release();
I64 z_stringtoi(const char* p);
U64 z_stringtoue(const char* str, int base);
char* z_itobase(I64 value, char* buf, int base, int* buff_length, int _signed);
void z_itobase2(char* buf, I64 value, int base, int buff_length, char padchar, int _signed);



#define I64_MIN (LLCONST(-9223372036854775807)-1)
#define I64_MAX (LLCONST(9223372036854775807))
#define U64_MAX (LLCONST(18446744073709551615u))
char * z_strstrcase (
        const char * str1,
        const char * str2
        );


int z_ipower(int val,int power);
#ifdef	__cplusplus
}
#endif
#endif

