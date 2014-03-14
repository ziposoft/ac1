


#include "zbase_lib/include/zipobase_lib.h"


zb_ds_text test1ds("test1");

zb_st_test table_test();

int main(int argc, char* argv[])
{
	test1ds.open();
	zb_st_test table_test(&test1ds);

	zb_record rec;

	table_test.get_default_rec(&rec);




	test1ds.close();
}
