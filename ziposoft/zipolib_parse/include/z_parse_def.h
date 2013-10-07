#ifndef z_parse_def_h
#define z_parse_def_h

enum zp_status
{
	zs_ok=0,
	zs_matched=0,
	zs_no_match,
	zs_skipped,
	zs_error,
	zs_template_syntax_error,
	zs_unparsed_data,
	zs_fatal_error,
	zs_eof,
	zs_feature_not_found,
	zs_operation_not_supported,
	zs_end_of_list,
	zs_cannot_create_virtual_obj,
	zs_no_entry_for_item,

};
#endif
