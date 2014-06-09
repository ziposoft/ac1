#include "zipolib_cpp_pch.h"
#include "z_error.h"


#define BUFF_SIZE 100



struct st_error_msg
{
	int id;
	ctext string_error_code;
	ctext string_error_message;
};
#define ERR(_CODE_,_MSG_) { _CODE_,#_CODE_,_MSG_ },

st_error_msg status_text[]=
{
ERR(zs_success,	"success")
ERR(zs_no_match,	0)
ERR(zs_skipped,	0)
ERR(zs_unparsed_data,	0)
ERR(zs_eof,	0)
ERR(zs_end_of_list,	0)
ERR(zs_child_not_found,	0)
ERR(zs_fatal_error,0)
ERR(zs_error,0)
ERR(zs_could_not_open_dir,0)
ERR(zs_could_not_open_file,0)
ERR(zs_not_open,0)
ERR(zs_already_open,0)
ERR(zs_bad_parameter,0)
ERR(zs_item_not_found,0)
ERR(zs_feature_not_found,0)
ERR(zs_operation_not_supported,0)
ERR(zs_not_implemented,0)
ERR(zs_cannot_create_virtual_obj,0)
ERR(zs_no_entry_for_item,0)
ERR(zs_syntax_error,0)
ERR(zs_template_syntax_error,0)
ERR(zs_tmpl_expected_closing_parenthesis,0)
ERR(zs_internal_error,0)
ERR(zs_bad_argument_2,0)
ERR(zs_wrong_object_type,0)
ERR(zs_out_of_range,0)
ERR(zs_data_error,0)
ERR(zs_parse_error,0)


};

ctext zs_get_status_text(z_status status)
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
	static char buff[0x20];
	sprintf(buff,"error msg not found:%d",status);
	return buff;
}
