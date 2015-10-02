
#include "z_parse_internal.h"


z_status z_zipex_base::report_error()
{
	z_logger_dump();
	printf("status=%s %s\n",zs_get_status_text(_last_status),_err_msg);
	if(_last_status==zs_ok)
		return zs_ok;
	zp_text_parser& tpl=tmpl();
	//TODO it should report from the FARTHEST that we got
	//printf("template=\n%s\n",t.get_buffer());
	
	if(_last_status)
	{
		//print_context();
		//tmpl.print_context();
		data().set_index(_furthest_index);
		data().print_context();
	}

	if(_last_status==zs_no_entry_for_item)
	{
		z_string match;
		tmpl().get_match(match);
		printf("No entry found for item \"%s\"",match.c_str());

	}
	if(_last_status==zs_syntax_error)
	{



	}
	return _last_status;
}

