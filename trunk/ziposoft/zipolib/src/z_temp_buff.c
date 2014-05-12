#include "zipo.h"
#include "z_temp_buff.h"

//TODO- replace these with a system to reuse buffers instead of reallocating them
// It won't make a performance difference, but allows me to get past this.
char* z_temp_buffer_get(size_t size)
{
	return (char*)malloc(size);

}


int  z_temp_buffer_release(char* ptr)
{

	free(ptr);
	return 0;
}

void  z_temp_buffer_cleanup()
{


}
