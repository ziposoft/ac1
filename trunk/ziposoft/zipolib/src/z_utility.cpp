#include "zipolib_cpp_pch.h"
#include "z_file.h"
#include "z_dbg.h"
void __cdecl z_debug_load_save_args(int* pargc, char*** pargv)
{
	int argc=*pargc;
	char** argv=*pargv;
	if((argc==2)&&(strcmp(argv[1],"debug")==0))
	{
		z_file file("debug.txt","rb");
		if (file.is_open())
		{
			z_string arg_temp;
			arg_temp << file;
			argc=arg_temp.GetDecVal();
			argv=(char**)malloc(argc*sizeof(char*));
			
			int i;
			for (i=0;i<argc;i++)
			{
				z_string arg_temp;
				arg_temp << file;
				argv[i]=
					(char*)malloc(arg_temp.size());
				strcpy(argv[i],arg_temp.c_str());
			}
		}
		*pargc=argc;
		*pargv=argv;

	}
	else
	{
		z_file file("debug.txt","wb");
        if (file.is_open())
        {
            file << argc << '\n';
			int i;
			for (i=0;i<argc;i++)
				file << argv[i] << '\n';
		}
	}
}




