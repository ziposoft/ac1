


#include "zipolib/include/zipolib_cpp.h"
#include "zipolib/include/z_filesystem.h"
#include "zipolib/include/z_trace.h"
#include "zipolib/include/z_ntf.h"
#include "zipolib/include/z_console_base.h"
#include "zipolib/include/z_factory_static.h"
/*
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

 */
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
  class testB
 {
 public:
	 testB()
	 {
		_xi=456;
		_name="larry";

	 }
	 virtual ~testB(){}
	 z_string _name;
	 int  _xi;
	 int func()
	 {
		 printf("hooorraaayy!!! %d  %s\n",_xi,_name.c_str());
		return 0;
	 }

 };

  /*
 class testc : public  z_ntf<testc>
 {
 public:
	testc()
	{
		_i=9;
			ZACT(func);
			ZPROP(_i);
	}
	int _i;
 	 int func()
	 {
		
		 printf("hooorraaayy lets nap!!\n");
		return 0;
	 }
 };

 */



int main(int argc, char** argv)
{
	ZT_ENABLE();
	ZTF;

	ZT("Testing trace %s %d...\n","duds",4);
	//dummy.add_features();
	z_debug_load_save_args(&argc,&argv);
	testA A;
	z_factory_T<testA>::self.dump_static(gz_out);
	z_factory* f=zf_get_factory("testA");
	f->dump_obj_static(gz_out,&A);

	f=	zf_get_factory_T<testA>();
	z_parser p;

	if(argc>1)
	{
		if(p.parse_obj_d(&A,argv[1],"{_str}ident:'=':{_i}ident"))
			p.report_error();

	}


	f->dump_obj_static(gz_out,&A);

	A._i=27;
	A._str="go run!";
 	f->dump_obj_static(gz_out,&A);

	f->set_var_as_string(&A,"_str","ok ok");
	f->set_var_as_string(&A,"_i","1");
	f->execute_act(&A,"func");

  	f->dump_obj_static(gz_out,&A);
	/*
	testc c;
	c.execute_act(&c,"func");
	c.dump_obj_static(gz_out,&c);

	*/

	return 0;//ZS_RET(base,feature_not_found);
}
  
ZFACT(testA)
{
	ZACT(func);

	ZPROP (_i);
	ZPROP (_str);

}

 #define ZO_OBJ_LIST \
	ZCLS(testB,none,"cmdline","{_xi}ident:'=':{i123}int",ACT(func)  VAR(_name) VAR(_xi)) 

		  
#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod), ZP_MOD(parse));