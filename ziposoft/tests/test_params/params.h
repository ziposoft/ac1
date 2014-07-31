

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef const char* ctext;
typedef int (*param_func)();



typedef struct _st_param_func 
{
	param_func _func_ptr;
	ctext	_name;
	ctext	_desc;
} st_param_func;
extern  st_param_func list_param_funcs[];
extern int list_param_funcs_count;

int param_process(int argc,char* argv[]);
