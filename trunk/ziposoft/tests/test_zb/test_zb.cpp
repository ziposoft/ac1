


#include "zbase_lib/include/zipobase_lib.h"



int test_table(zb_source* p_ds)
{
	z_status status;
	zb_record* pRec=0;
	zb_st_test table_test(p_ds);

	do
	{
		status=p_ds->open(true);
		if(status)
			break;
		zb_record* pRec= table_test.new_default_rec();
		if(!pRec)
			break;
		status=table_test._f_name->set(pRec,"hello?");
		if(status)
			break;

		status=table_test.record_add(pRec);
		if(status)
			break;
		status=p_ds->commit();
		
	}while(0);
	
	p_ds->close();
	if(pRec)
		delete pRec;
	z_logger_dump();




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
