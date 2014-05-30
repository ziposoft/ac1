#include "zipo.h"
#include "z_temp_buff.h"

//TODO- replace these with a system to reuse buffers instead of reallocating them
// It won't make a performance difference, but allows me to get past this.

#define TEMP_BUFF_SIZE 0x100



static char buff[TEMP_BUFF_SIZE];

#ifdef BUILD_VSTUDIO  

#define Z_DEBUG_BREAK  __debugbreak();
#endif

char* z_temp_buffer_get(size_t size)
{
	if(size< TEMP_BUFF_SIZE)
		return 	&buff[0];
#ifdef BUILD_VSTUDIO  
	__debugbreak();
#endif
	printf("ERROR!!! %d Exceeds tempbuff size!!\n");
	return 0;
}


int  z_temp_buffer_release(char* ptr)
{

	//free(ptr);
	return 0;
}

void  z_temp_buffer_cleanup()
{


}
size_t  z_temp_buffer_size()
{
	return TEMP_BUFF_SIZE;

}
