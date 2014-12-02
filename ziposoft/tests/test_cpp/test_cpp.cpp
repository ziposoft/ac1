#include  "test_cpp.h"
void test_func_ptrs();
 void templates();
;
int main()
{
	test_func_ptrs();



	return 0;
};

#if 0

int templ_test()
{
	A* a=new B<char>();
	a->func2(3);
	a=new B<bool>();
	a->func2(3);

	return 0;
}
// template <> void B<bool>::func2(int x) const { printf("bool func2 overload"); };
// template <> void B<bool>::func3(int x) const {};



using namespace std;
namespace X 
{
	int i;
};
namespace X 
{
	int j;
};
class test 
{
public:
	int i;
	string str;
	int j;
	int k;
	int func() { printf("hooray!!\n");return 0;};

};
typedef int (test::*test_obj_funcptr)();


 template <class C >  class fact_T
 {
 public:
	 static const fact_T<C> &p;

	const static fact_T<C> static_instance;
	void* create() const ;
 };
 

 template class fact_T<test> ;


 const  fact_T<test> theone;
template<> const fact_T<test>&  fact_T<test>::p=theone;
template<> const fact_T<test>  fact_T<test>::static_instance;
template<> void* fact_T<test>::create() const { return 0;};

class test_ntf
{
public:

};
void print_str(void* obj,size_t offset)
{
	char* p=(char*)obj+offset;

	string* sp=reinterpret_cast<string*>(p);
	printf("str= %s",sp->c_str());


}
void print_int(void* obj,size_t offset)
{
	char* p=(char*)obj+offset;

	int* sp=reinterpret_cast<int*>(p);
	printf("int= %d",*sp);


}


int main()
{
	printf("offsetof(test,_i)=%d\n",offsetof(test,k));
	size_t offset_k=offsetof(test,k);
	size_t offset_str=offsetof(test,str);

	test x,y,z;

	test* null_obj=0;

	test_obj_funcptr pFunc=(&test::func);






												  
	x.str="larry x";
	x.k=99;
	y.str="geny y";
	y.k=321;

	fact_T<test>::p.create();

	void* px=(void*)&x;
	void* py=(void*)&y;

	print_int(px,offset_k);
	print_str(px,offset_str);
	print_int(py,offset_k);
	print_str(py,offset_str);

	X::j=3;
	(x.*pFunc)();
	return 0;
}


#endif
