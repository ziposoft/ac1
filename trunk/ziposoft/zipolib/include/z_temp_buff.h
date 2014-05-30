#include "zipolib/include/zipo.h"
#ifndef z_temp_buff_h
#define z_temp_buff_h
#ifdef	__cplusplus
extern "C" {
#endif


char* z_temp_buffer_get(size_t size);
size_t  z_temp_buffer_size();
int  z_temp_buffer_release(char* ptr);
void  z_temp_buffer_cleanup();


#ifdef	__cplusplus
}
#endif
#endif

