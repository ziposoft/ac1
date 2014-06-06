#include "test_parse.h"


//GLOBALS
void * g_parse_obj=0;
z_factory * g_parse_fact=0;
z_parser p;
ctext g_template=0;
ctext g_expected_output=0;
ctext g_expected_def_output=0;
z_status g_expected_result=zs_ok;
opt_func g_test_type_function_to_run=0;

/*--------------------------------------------------------------

	Test Types

--------------------------------------------------------------*/

int  run_custom()
{

	z_status status=zs_no_match;
	zp_value obj;


	status=p.parse_obj(&obj,g_arg_data_in);



	if(status!=g_expected_result)
	{
		printf("TEST FAILED!\n");
		printf("result=%s\n",zs_get_status_text(status));
		printf("expected result=%s\n",zs_get_status_text(g_expected_result));
		p.report_error();
		return -1;
	}
	printf("TEST PASSED\n");
	zf_dump_obj(&obj);

	return 0;
}
int  run_test_only()
{
	if(g_parse_obj)
	{
		delete g_parse_obj;
		g_parse_obj=0;
	}
	z_status status=zs_no_match;


	if(g_template)
		status=p.parse_template(g_template,g_arg_data_in);
	else
	{
		if(g_arg_obj_type)
		{
		
			g_parse_obj= zfs_create_obj_by_type(g_arg_obj_type);
 			if(!g_parse_obj)
			{
				printf("ERROR! Cannot create object of class \"%s\" .\n",g_arg_obj_type.c_str());
				return -1;
			}
			status=p.parse_obj_f(g_parse_obj,g_parse_fact,g_arg_data_in);

		}
		else
		{
			printf("ERROR! no object or template specified.\n");
			return -1;
		}
	}



	if(status!=g_expected_result)
	{
		printf("TEST FAILED!\n");
		printf("result=%s\n",zs_get_status_text(status));
		printf("expected result=%s\n",zs_get_status_text(g_expected_result));
		p.report_error();
		return -1;
	}
	printf("TEST PASSED\n");
	if(g_arg_dump=="on")
		if(g_parse_obj)
		{
			g_parse_fact->dump_obj(gz_out,g_parse_obj);
		}

	return 0;
}

int run_dump_all()
{
	zo_factory_list_dump();
	return 0;
}
int run_test_create()
{
	return run_test_only();
}
int run_test_all()
{
	int val=run_test_create_output();
	if(val==0)
		val=run_test_output_def();
	return val;
}
int run_test_create_output()
{
	int val=run_test_create();
	if(val==-1)
		return val;
	return run_test_output();
	
}
int run_test_output()
{
	z_status status=zs_no_match;
	z_file_string_buffer fp_output_buffer;
	if(g_parse_obj)
	{
		status=p.output_obj(&fp_output_buffer,g_parse_fact,g_parse_obj);
		if(status==zs_no_match)
		{
			if(g_expected_output)
			{
				if(strlen(g_expected_output))
				{
					printf("No match, but expected output. FAIL.\n");
					return -1;
				}
			}
			printf("No match, no expected output. PASS.\n");
			return 0;
		}
		if(status)
		{
			printf("output_obj FAILED!\n");
			p.report_error();
			return -1;
		}
		if(g_expected_output)
		{

			//g_parse_obj->output(&fp_output_buffer);
			if(fp_output_buffer.get_buffer() != g_expected_output)
			{
				printf("Output FAILED!\n");
				printf("result=%s\n",zs_get_status_text(status));
				printf("Expected: %s (len=%d)\n",g_expected_output,strlen(g_expected_output));
				printf("Got     : %s (len=%d)\n",fp_output_buffer.get_buffer().c_str(),fp_output_buffer.get_buffer().size());
				return -1;
			}
			printf("Output MATCHED:\n");
			return 0;
		}
		printf("Output:\n");
		gz_out<<fp_output_buffer.get_buffer()<<'\n';
		return status;
	}
	/*
	if(g_expected_output)
	{
		if(strlen(g_expected_output)==0)
		{
			printf("No object, no expected output. PASS.\n");
			return 0;
		}
		else
		{
			printf("No object, but expected output. FAIL.\n");
			return -1;
		}
	}
	*/
	return 0;
}
int run_test_output_def()
{
	printf("OUTPUT DEFAULT:\n");
	if(!g_template)
	{
		if(g_arg_obj_type)
		{
			return run_create_def_obj();
		}
		printf("ERROR template and obj not set\n");
		return -1;

	}
	z_status status=zs_no_match;
	z_file_string_buffer fp_output_buffer;
	status=p.output_default_template(&fp_output_buffer,g_template);

	if(status)
	{
		
		/* sometimes we expect the output to fail, as in a bad object
		
		printf("Default output FAILED!\n");
		
		p.report_error();
		return -1;
		*/
	}
	if(g_expected_def_output)
	{
		if(fp_output_buffer.get_buffer() != g_expected_def_output)
		{
			printf("Default output FAILED!\n");
			printf("result=%s\n",zs_get_status_text(status));
			printf("Expected: %s (len=%d)\n",g_expected_def_output,strlen(g_expected_def_output));
			printf("Got     : %s (len=%d)\n",fp_output_buffer.get_buffer().c_str(),fp_output_buffer.get_buffer().size());
			return -1;
		}
		printf("Default Output MATCHED.\n");
	}

	return 0;
}


int run_create_def_obj()
{
	printf("CREATE DEFAULT OBJECT\n");
	z_status status=zs_no_match;
	g_parse_obj= zfs_create_obj_by_type(g_arg_obj_type);
	if(g_parse_obj)
	{
		return run_test_output();

	}
	printf("Default create FAILED!\n");	
	return -1;

}

/*--------------------------------------------------------------

	Main Operations

--------------------------------------------------------------*/


int run_parse_xml()
{
	/*

	zp_xml_file xml_file;
	z_status status=zs_no_match;

	p.set_ignore_whitespace();

	status=p.parse_obj(&xml_file,g_arg_data_in);




	if(status!=g_expected_result)
	{
		printf("TEST FAILED!\n");
		printf("result=%s\n",zs_get_status_text(status));
		printf("expected result=%s\n",zs_get_status_text(g_expected_result));
		p.report_error();
		return -1;
	}
	printf("TEST PASSED\n");
	if(g_arg_dump=="on")
	{
		printf("dumping...\n");
			p.dump_obj(&gz_out,&xml_file);
	}

	return 0;
	*/
	return -1;
}

int run_parse_obj()
{
	if(g_test_type_function_to_run)
		return (*g_test_type_function_to_run)();
	return -1;
}

int run_static_tests()
{
	//___________________________________________________________
	//
	//  
	//___________________________________________________________

	size_t num_tests=test_tmpl_list_count;
	size_t start=0;
	size_t end=num_tests-1;
	if(g_arg_test_num!="all")
	{
		U32 test_number=g_arg_test_num.GetDecVal();
		if(test_number>=num_tests)
		{
			gz_out << "Invalid test number \""<<g_arg_test_num <<"\"";
			return -1;
		}
		start=end=test_number;
	}
	size_t test_num;
	printf("\n");
	for(test_num=start;test_num<=end;test_num++)
	{
		const st_test_tmpl_entry& entry=test_tmpl_list[test_num];
		printf("TEST #%d: %s\n",test_num,(entry.desc?entry.desc:""));
		printf("tmpl=%s, data=%s, expected result=%s\n",
			entry.templ,
			entry.input_data,
			zs_get_status_text(entry.expected_result)
			);
		g_arg_data_in=entry.input_data;
		g_expected_output=entry.expected_output;
		g_expected_result=entry.expected_result;
		g_expected_def_output=entry.expected_default_output;
		g_template=entry.templ;

		if((*g_test_type_function_to_run)())
			break;
	}
	//if(test_num==num_tests) printf("ALL TESTS PASSED.\n");

	return 0;
}
int run_provided_template_test()
{
	//___________________________________________________________
	//
	//  
	//___________________________________________________________

	g_expected_output="";
	g_expected_result=zs_matched;
	g_template=g_arg_templ_in.c_str();

	(*g_test_type_function_to_run)();

	return 0;
}