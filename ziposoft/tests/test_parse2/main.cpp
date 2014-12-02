#include "test_parse.h"
#include "zipolib/include/z_dbg.h"
#include "zipolib/include/z_parse_text.h"


z_string g_file_buffer;


class foo_base
{
public:
	int x;
};
class foo_1 : public foo_base
{
public:
	int y;
};
int func1(foo_base* x)
{
	x->x=1;
	return 0;
}

int func2()
{
	foo_1 y;
	foo_base* py=&y;
	func1(&y);
	return 0;
}


int main(int argc, char** argv)
{
	ZT_ENABLE();





	//z_trace_enable();
	//___________________________________________________________
	//
	//   Load/Save debug arguments
	//___________________________________________________________

	z_debug_load_save_args(&argc,&argv);

	//___________________________________________________________
	//
	//assign args 
	//___________________________________________________________
	int i;
	for (i=1;i<argc;i++)
	{
		size_t j;
		z_string name;
		z_string val;
		zp_text_parser argp(argv[i]);
		if(argp.test_any_identifier()==zs_matched)
		{
			argp.get_match(name);
			if(argp.test_char('=')==zs_matched)
			{
				argp.test_to_eob();
				argp.get_match(val);
			}
			for(j=0;j<arg_list_count;j++)
			{
				argument& a= arg_list[j];
				if(name==a.name)
				{
					if(a.num_valid_options)
					{
						option* opt=get_option(a,val);
						if(!opt)
						{
							zout<<"\nERROR! Invalid option \"" <<val<<"\" for argument \""<<name<<"\"\n\n";
							show_valid_options(a);
							return -1;
							break;
						}
					}
					a.ref_variable=val;
					break;
				}
			}
			if(j==arg_list_count)
			{
				zout<<"\nERROR! Invalid argument \"" <<name<<"\"\n\n";
				run_help();
				exit(0);
				
			}
		}
	}

	//___________________________________________________________
	//
	//Check Data file
	//___________________________________________________________

	if(g_arg_file_input_data)
	{
		z_file f;
		if(f.open(g_arg_file_input_data,"rb")==-1)
		{
			zout<<"\nERROR! Cannot open \"" <<g_arg_file_input_data<<"\"\n\n";
			return -1;
		}
		f.read_all(g_file_buffer);
		g_arg_data_in=g_file_buffer.c_str();

	}
	//___________________________________________________________
	//
	//Check Class type
	//___________________________________________________________

	if(g_arg_obj_type)
	{
		g_parse_fact=zf_get_factory(g_arg_obj_type);
		g_arg_operation="obj";
		if(!g_parse_fact)
		{
			printf("ERROR! Class \"%s\" not found.\n",g_arg_obj_type.c_str());
			return -1;
		}

	}
	//___________________________________________________________
	//
	//Check Test Type
	//___________________________________________________________

	option* opt=get_option(arg_list[2],g_arg_test_type);
	if(!opt)
	{
		zout << "Invalid test type  \""<<g_arg_test_type <<"\"\n";
		return -1;
	}
	g_test_type_function_to_run=opt->pfunc;
	if(!g_test_type_function_to_run)
	{
		zout << "No function for  \""<<opt->name <<"\"\n";
		return -1;
	}
	//___________________________________________________________
	//
	//execute operation 
	//___________________________________________________________

	opt=get_option(arg_list[0],g_arg_operation);
	if(!opt)
	{
		return run_help();
	}
	opt_func fp=opt->pfunc;
	if(fp)
		return (*fp)();
	zout << "No action for "<< g_arg_operation << "\n";
	return 0;

}
