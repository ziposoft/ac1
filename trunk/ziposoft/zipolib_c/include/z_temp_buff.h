#include "zipolib_c/include/zipo.h"
#ifndef z_temp_buff_h
#define z_temp_buff_h
#ifdef	__cplusplus
extern "C" {
#endif


U8* z_temp_buffer_get(size_t size);
void  z_temp_buffer_release(char* ptr);
void  z_temp_buffer_cleanup();


#ifdef	__cplusplus
}
#endif
#endif

