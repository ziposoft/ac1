#include "zipo.h"
#include "z_temp_buff.h"

//TODO- replace these with a system to reuse buffers instead of reallocating them
// I won't make a performance difference, but allows me to get past this.
U8* z_temp_buffer_get(size_t size)
{
	return (U8*)malloc(size);

}


void  z_temp_buffer_release(char* ptr)
{

	free(ptr);
}

void  z_temp_buffer_cleanup()
{


}
