


#include "zbase_lib/include/zipobase_lib.h"



int test_table(zb_source* p_ds)
{
	zb_st_test table_test(p_ds);


	p_ds->open(true);
	zb_record* pRec= table_test.new_default_rec();

	table_test._f_name->set(pRec,"hello?");


	table_test.record_add(pRec);

	delete pRec;

	p_ds->commit();
	p_ds->close();

	return 0;


}





int main(int argc, char* argv[])
{
	z_status status;


	/*____________________________________________________________________________
		Test Metakit 
	____________________________________________________________________________*/

	zb_ds_metakit test_ds_mk("test");

	test_table(&test_ds_mk);




 	/*____________________________________________________________________________

	Test Text 
	____________________________________________________________________________*/

	zb_ds_text test_ds_text("test");
	test_table(&test_ds_mk);


}
