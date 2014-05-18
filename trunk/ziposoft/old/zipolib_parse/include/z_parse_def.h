#ifndef z_parse_def_h
#define z_parse_def_h

enum 
{
	zs_ok=0,
	zs_matched=0,
	zs_no_match,
	zs_skipped,
	zs_unparsed_data,
	zs_eof,
	zs_end_of_list,
	zs_child_not_found,
	zs_fatal_error,//fatal errors below. Status above!
	zs_template_syntax_error,
	zs_error,
	zs_feature_not_found,
	zs_operation_not_supported,
	zs_cannot_create_virtual_obj,
	zs_no_entry_for_item,
	zs_syntax_error,
	zs_tmpl_expected_closing_parenthesis,
	zs_internal_error,
	zs_bad_argument_2,

};
ctext z_status_get_text(z_status status);
#endif
