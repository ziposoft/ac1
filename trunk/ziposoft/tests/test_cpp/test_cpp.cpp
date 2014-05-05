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
};

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

	x.str="larry x";
	x.k=99;
	y.str="geny y";
	y.k=321;



	void* px=(void*)&x;
	void* py=(void*)&y;

	print_int(px,offset_k);
	print_str(px,offset_str);
	print_int(py,offset_k);
	print_str(py,offset_str);

	X::j=3;
	return 0;
}
