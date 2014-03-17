


#include "zbase_lib/include/zipobase_lib.h"


zb_ds_text test1ds("test1");

zb_st_test table_test();

int main(int argc, char* argv[])
{
	test1ds.open();
	zb_st_test table_test(&test1ds);

	zb_record rec;

	table_test.get_default_rec(&rec);

	table_test._f_name->set(&rec,"hello?");


	table_test.record_add(&rec);


	test1ds.close();
}
