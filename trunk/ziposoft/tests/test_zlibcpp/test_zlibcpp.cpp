


#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_filesystem.h"
#include "zipolib/include/z_trace.h"
#include "zipolib/include/z_ntf.h"
#include "zipolib/include/z_console.h"
#include "zipolib/include/z_factory_dyn.h"

 class test	 : public 	z_ntf_obj
 {
 public:
	 test():z_ntf_obj("test")
	 {

		PROP(_i);
		PROP(_str);
		PROP_T(hex32,_hex);

	 }
	 virtual ~test(){}
	 U32 _hex;
	 U32 _i;
	 z_string _str;


 };


 int test_ntf()
 {
	printf("offsetof(test,_i)=%d\n",offsetof(test,_i));

	z_string larry="Fred";

	test x;
	x.set_prop("_i","123");
	x.get_prop("_i",larry);
	gz_out
		<<larry
		<<','
		<<x._i 
		<<'\n';

	x._i=982;
	x.get_prop("_i",larry);
	gz_out <<larry<<','<<x._i <<'\n';
 	x._hex=982;
	x.get_prop("_hex",larry);
	gz_out <<larry<<','<<x._hex <<'\n';



	return 0;
 }
#define TEST_FILE "testfile.txt"

 int test_filesystem()
 {
	z_directory localdir;
	z_strlist list;
	gz_out << "list of exe files:\n";

	localdir.get_files_by_extension("exe",list);
	list.dump(gz_out);
	localdir.get_files_by_extension("lib",list);
	list.dump(gz_out);

	z_file test_file;
	test_file.open(TEST_FILE,"w");
	test_file.putf("testing %d,%d,%d\n",1,2,3);
	test_file.close();

	printf("z_file_exists(TEST_FILE)=%d\n",z_file_exists(TEST_FILE));

	test_file.open(TEST_FILE,"r");

	size_t size;
	test_file.get_file_size(size);
	printf("get_file_size=%d\n",size);

	test_file.close();




	return 0;
 }



 class testA
 {
 public:
	 testA()
	 {


	 }
	 virtual ~testA(){}
	 U32 _i;
	 z_string _str;
	 int func()
	 {
		printf("hooorraaayy!!!\n");
	 }

 };

z_factory_T	<testA>	  factA;


int main()
{
	ZT_ENABLE();
	ZTF;

	ZT("Testing trace %s %d...\n","duds",4);




	return 0;//ZS_RET(base,feature_not_found);
}
