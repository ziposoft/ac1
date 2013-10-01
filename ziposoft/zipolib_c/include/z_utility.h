/*________________________________________________________________________

z_utility.h

________________________________________________________________________*/


#include "zipolib_c/include/zipo.h"
#ifndef z_utility_h
#define z_utility_h
#ifdef	__cplusplus
extern "C" {
#endif


U32 z_byte_swap32(U32 x);
U16 z_byte_swap16(U16 x);



int z_ipower(int val,int power);
#ifdef	__cplusplus
}
#endif
#endif

