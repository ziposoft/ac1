#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include  <string>
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
	int func() { return 0;};

};
typedef int (test::*obj_funcptr)();


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

	obj_funcptr p=&test::func;
	printf("obj_funcptr=%p\n",p);
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
	return 0;
}



