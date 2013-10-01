#include "zipolib_c/include/zipo.h"
#ifndef z_temp_buff_h
#define z_temp_buff_h
#ifdef	__cplusplus
extern "C" {
#endif


char* z_temp_buffer_get(size_t size);
int  z_temp_buffer_release(char* ptr);
void  z_temp_buffer_cleanup();


#ifdef	__cplusplus
}
#endif
#endif

