
#include "params.h"

int param_help(ctext exe_name)
{
	int i;
	printf("Usage:\n\t%s [operation] [option[=value]]\n\n",exe_name);

	printf("Valid operations:\n");
	for(i=0;i<list_param_funcs_count;i++)
	{
		st_param_func* f=&list_param_funcs[i];
		printf("\t%s - %s\n",f->_name,f->_desc);

	}
	return 0;
}
int param_exec_operation(ctext operation)
{
	int i;
	for(i=0;i<list_param_funcs_count;i++)
	{
		st_param_func* f=&list_param_funcs[i];

		if(strcmp(f->_name,operation)==0)
		{
			param_func fp=f->_func_ptr;
			if(fp)
				(*fp)();
			return 0;
		}
	}
	return -1;
}
int param_process(int argc,char* argv[])
{

	int i;

	if(argc<2)
		return param_help(argv[0]);

	

	for (i=1;i<argc;i++)
	{
		

	}
	if(param_exec_operation(argv[1]))
	{
		printf("Unknown operation: \"%s\"\n",argv[1]);
		param_help(argv[0]);
		return -1;
	}
	return 0;

}
