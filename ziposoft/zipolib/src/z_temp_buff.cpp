#include "zipolib_cpp_pch.h"
#include "z_temp_buff.h"
#include "z_dbg.h"

//TODO- replace these with a system to reuse buffers instead of reallocating them
// It won't make a performance difference, but allows me to get past this.

#define TEMP_BUFF_SIZE 0x2000
#define TEMP_BUFF_COUNT 8


#if 0  //TODO - performance
static char buff[TEMP_BUFF_COUNT][TEMP_BUFF_SIZE];
#endif

int gz_temp_buff_outstanding=0;
int gz_temp_buff_count=0;

char* z_temp_buffer_get(size_t size)
{
	gz_temp_buff_outstanding++;
	gz_temp_buff_count++;
	return (char*)z_new char[size];
#if 0  //TODO - performance
	index++;
	if(index>TEMP_BUFF_COUNT)
		index=0;
	count++;
	if(count>3)
	{
		Z_DEBUG_BREAK;
	}
	if(count>TEMP_BUFF_COUNT)
	{
		//DO NOT USE ASSERT HERE. It would recursive
		Z_DEBUG_BREAK;
		printf("ERROR!!! %d tempbuff count!!\n",count);

	}	
	if(size< TEMP_BUFF_SIZE)
		return 	&buff[index][0];
	Z_DEBUG_BREAK(0);
	printf("ERROR!!! %d Exceeds tempbuff size!!\n",size);
	return 0;
#endif
}


int  z_temp_buffer_release(char* ptr)
{
	gz_temp_buff_outstanding--;
	delete ptr;
#if 0  //TODO - performance
	if(count<0)
	{
		Z_DEBUG_BREAK;
		printf("ERROR!!! %d tempbuff count!!\n",count);

	}
#endif
	return 0;
}

void  z_temp_buffer_cleanup()
{


}
size_t  z_temp_buffer_size()
{
	return TEMP_BUFF_SIZE;

}
