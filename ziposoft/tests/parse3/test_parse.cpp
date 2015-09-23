#include "test_parse.h"
#include "z_parse_internal.h"

//GLOBALS

ctext g_template=0;
ctext g_expected_output=0;
ctext g_expected_def_output=0;
z_status g_expected_result=zs_ok;
opt_func g_test_type_function_to_run=0;

/*
const st_test_tmpl_entry test_tmpl_list[]=
{
{"'a'","a",zs_matched,"a","a","simple string literal match"},
{"'a'","b",zs_no_match,"a","a","simple string literal match"},
{"^'c':'c'","aaaaac",zs_matched,"c","c"},
{"badkeyword","a",zs_no_entry_for_item,"",""},

};
const size_t test_tmpl_list_count=sizeof(test_tmpl_list)/sizeof(st_test_tmpl_entry);

*/
/*--------------------------------------------------------------

Test Types

--------------------------------------------------------------*/

int  run_test_only()
{

	z_status status=zs_no_match;

	z_zipex zipex(g_template,g_arg_data_in);


	if(g_template)
		status=zipex.parse();
	else
	{
		printf("ERROR! no object or template specified.\n");
		return -1;
	}



	if(status!=g_expected_result)
	{
		printf("TEST FAILED!\n");
		printf("result=%s\n",zs_get_status_text(status));
		printf("expected result=%s\n",zs_get_status_text(g_expected_result));
		zipex.report_error();
		return -1;
	}
	printf("TEST PASSED\n");

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
#if 0
	z_file_string_buffer fp_output_buffer;
	if(parse_root)
	{
		status=p.output_obj(&fp_output_buffer,parse_root);
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
			p.report_error(status);
			return -1;
		}
		if(g_expected_output)
		{

			//parse_root->output(&fp_output_buffer);
			if(fp_output_buffer.get_buffer() != g_expected_output)
			{
				printf("Output FAILED!\n");
				printf("result=%s\n",z_status_get_text(status));
				printf("Expected: %s (len=%d)\n",g_expected_output,strlen(g_expected_output));
				printf("Got     : %s (len=%d)\n",fp_output_buffer.get_buffer().c_str(),fp_output_buffer.get_buffer().size());
				return -1;
			}
			printf("Output MATCHED:\n");
			return 0;
		}
		printf("Output:\n");
		zout<<fp_output_buffer.get_buffer()<<'\n';
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
#endif
	return 0;
}
int run_test_output_def()
{
	printf("OUTPUT DEFAULT:\n");

#if 0
	if(!g_template)
	{
		if(g_arg_obj)
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
		printf("Default output FAILED!\n");
		p.report_error(status);
		return -1;
	}
	if(g_expected_def_output)
	{
		if(fp_output_buffer.get_buffer() != g_expected_def_output)
		{
			printf("Default output FAILED!\n");
			printf("result=%s\n",z_status_get_text(status));
			printf("Expected: %s (len=%d)\n",g_expected_def_output,strlen(g_expected_def_output));
			printf("Got     : %s (len=%d)\n",fp_output_buffer.get_buffer().c_str(),fp_output_buffer.get_buffer().size());
			return -1;
		}
		printf("Default Output MATCHED.\n");
	}
#endif
	return 0;
}


int run_create_def_obj()
{
	printf("CREATE DEFAULT OBJECT\n");
	z_status status=zs_no_match;


	return -1;

}

/*--------------------------------------------------------------

Main Operations

--------------------------------------------------------------*/
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
int  run_custom()
{

	z_zipex zipex("(Az):(int):'.':(ident)",	"fred001.ext");



	zipex.parse();
	zipex.output(&zout);



	return 0;
}
int  run_dump_all()
{
	return 0;
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
			zout << "Invalid test number \""<<g_arg_test_num <<"\"";
			return -1;
		}
		start=end=test_number;
	}
	size_t test_num;
	printf("\n");
	for(test_num=start;test_num<=end;test_num++)
	{
		const st_test_tmpl_entry& entry=test_tmpl_list[test_num];
		printf("TEST #%d: %s\n",test_num,"");
		printf("tmpl=%s, data=%s, expected result=%s\n",
			entry.templ,
			entry.input_data,
			zs_get_status_text(entry.expected_result)
			);

		z_zipex zipex(entry.templ,entry.input_data);	
		z_status status=zs_no_match;
		status=zipex.parse();
		
		size_t i;
		size_t count=zipex.get_group_count();
		for(i=0;i<count;i++)
		{
			z_string s=zipex.get_group(i);
			printf("[%s]",s.c_str());
			if(i< entry.num_results)
			{
				if(s!=entry.result[i])
				{
					printf("!=%s ",entry.result[i]);
				}
				
			}

		}



		if(status!=entry.expected_result)
		{
			printf("TEST FAILED!\n");
			printf("result=%s\n",zs_get_status_text(status));
			printf("expected result=%s\n",zs_get_status_text(entry.expected_result));
			zipex.report_error();
			return -1;
		}
		if(count != entry.num_results)
		{
			printf("ERROR expected count=%d actual=%d\n",zipex.get_group_count(),entry.num_results);
			return -1;
		}

		printf("TEST PASSED\n");


	}
	//if(test_num==num_tests) printf("ALL TESTS PASSED.\n");

	return 0;
}
	