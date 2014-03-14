#ifndef FIELD_STATIC_H
#define FIELD_STATIC_H
#include "zbase_lib/include/zb.h"
#include "zbase_lib/include/field.h"

enum zk_static_field
{
	zk_sf_1_key=1,
	zk_sf_1_ds_tbl_name,
	zk_sf_1_tbl_name,
	zk_sf_1_label,
	zk_sf_1_desc,

	zk_sf_test_key,
	zk_sf_test_str,



	zk_sf_dynamic_start=1000,
};

#endif
