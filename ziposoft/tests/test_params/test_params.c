#include "params.h"

#define PARAM_LIST \
	PARAM(int,g_sample_int,99,"sample integer params")\
	PARAM(ctext ,g_sample_string,"default","description")\
	OPER(function1,"") \
	OPER(function2,"")

#include "params.inc"


int main(int argc,char* argv[])
{
	return param_process(argc,argv);	
}

int function1()
{
	printf("function1\n");
	return 0;
}
int function2()
{
	printf("function2\n");
	return 0;
}
