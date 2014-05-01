


#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_cpp/include/z_filesystem.h"
#include "zipolib_cpp/include/z_trace.h"
#include "zipolib_cpp/include/z_ntf.h"

 class test	 : public 	z_ntf_obj
 {
 public:
	 test()
	 {

		PROP(_i);
		PROP(_str);
		PROP_T(hex32,_hex);

	 }
	 virtual ~test(){}
	 U32 _i;
	 U32 _hex;
	 z_string _str;


 };



#define TEST_FILE "testfile.txt"

int main()
{
	ZT_ENABLE();
	ZTF;
	z_string larry="Fred";

	ZT("Testing trace %s %d...\n","duds",4);


	test x;
	x.props["_i"]->set_value("123");
	x.props["_i"]->get_value(larry);
	gz_out <<larry<<','<<x._i <<'\n';

	x._i=982;
 	x.props["_i"]->get_value(larry);
	gz_out <<larry<<','<<x._i <<'\n';
 	x._hex=982;
 	x.props["_hex"]->get_value(larry);
	gz_out <<larry<<','<<x._hex <<'\n';


	gz_out << "list of exe files:\n";


	z_directory localdir;
	z_strlist list;

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


	return 0;//ZS_RET(base,feature_not_found);
}
