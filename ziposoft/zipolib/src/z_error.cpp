#include "zipolib_cpp_pch.h"
#include "z_error.h"


#define BUFF_SIZE 100


z_error_handler gz_error;




z_status z_error_handler::report(ctext file,ctext func,int line,z_status status,const char*  lpszFormat,  ... )
{
    int c;
	static char buff[BUFF_SIZE];

	va_list ArgList;
	va_start (ArgList, lpszFormat);

    c=vsnprintf (buff,BUFF_SIZE-1, lpszFormat, ArgList);

	z_debug_out(buff);
	gz_out << buff << '\n';

	return status;
}

struct st_error_msg
{
	int id;
	ctext string_error_code;
	ctext string_error_message;
};
#define ERR(_CODE_,_MSG_) { _CODE_,#_CODE_,_MSG_ },

st_error_msg status_text[]=
{
ERR(z_status_success,	"success")
ERR(zs_eof,				0)
ERR(zs_end_of_list	,	0)
ERR(zs_child_not_found,	0)
ERR(zs_fatal_error,	0)
ERR(z_status_error,	0)
ERR(z_status_could_not_open_dir,	0)
ERR(z_status_already_open,	0)
ERR(z_status_bad_parameter,	0)
ERR(z_status_item_not_found,	0)
ERR(zs_feature_not_found,	0)
ERR(zs_operation_not_supported,	0)
ERR(zs_cannot_create_virtual_obj,	0)
ERR(zs_no_entry_for_item,	0)
ERR(zs_syntax_error,	0)
ERR(zs_tmpl_expected_closing_parenthesis,	0)
ERR(zs_internal_error,	0)
ERR(zs_bad_argument_2,	0)
ERR(zs_bad_parameter,	0)
ERR(zs_wrong_object_type,	0)
ERR(zs_template_syntax_error,	0)
ERR(zs_skipped,	0)
ERR(zs_unparsed_data,	0)

};

ctext z_status_get_text(z_status status)
{
	size_t  i;
	for (i=0;i<(sizeof(status_text)/ sizeof(st_error_msg) );i++)
	{
		st_error_msg* e=& status_text[i];
		if(e->id==status)
		{
			if(	e->string_error_message)
				return e->string_error_message;
			else
				return e->string_error_code;

		}
	}
	return "error msg not found";
}
