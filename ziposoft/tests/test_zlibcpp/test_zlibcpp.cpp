


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
		_i=123;
		_str="fred";

	 }
	 virtual ~testA(){}
	 int  _i;
	 z_string _str;
	 int func()
	 {
		 printf("hooorraaayy!!! %d  %s\n",_i,_str.c_str());
		return 0;
	 }

 };
#define ZFACT(_CLASS_)  z_factory_T<_CLASS_> ZFACT##_CLASS_(#_CLASS_);\
	z_factory_T<_CLASS_>& z_factory_T<_CLASS_>::self=ZFACT##_CLASS_;\
	void z_factory_T<testA>	::add_features()

//#define ZPROP(_VAR_) _features.add(z_new	zf_feature(#_VAR_,zp_var_funcs_get(_VAR_),zp_offsetof_class(THECLASS,_VAR_)));
#define ZPROP(_VAR_) _features.add(z_new	zf_feature(#_VAR_,zp_var_funcs_get( ((THECLASS*)0)->_VAR_),zp_offsetof_class(THECLASS,_VAR_)));

ZFACT(testA)
{
	add_act("func",&testA::func);

	ZPROP (_i);
	ZPROP (_str);

}
int main(int argc, char** argv)
{
	ZT_ENABLE();
	ZTF;

	ZT("Testing trace %s %d...\n","duds",4);
	//dummy.add_features();
	z_debug_load_save_args(&argc,&argv);
	testA A;
	z_factory_T<testA>::self.dump_static(gz_out);
	const z_factory* f=zf_get_factory("testA");
	z_parser p;

	if(argc>1)
	{
		if(p.parse_obj_d(&A,argv[1],"{_str}ident:'=':{_i}ident"))
			p.report_error();

	}


	f->dump_obj(gz_out,&A);

	A._i=27;
	A._str="go run!";
 	f->dump_obj(gz_out,&A);

	f->set_var_as_string(&A,"_str","ok ok");
	f->set_var_as_string(&A,"_i","1");
	f->execute_act(&A,"func");

  	f->dump_obj(gz_out,&A);



	return 0;//ZS_RET(base,feature_not_found);
}
