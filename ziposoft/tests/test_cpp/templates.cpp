#include  "test_cpp.h"

class A
{
 public:
	A()
	{
		i=0;
	}
	int i;

};
class B : public A
{
 public:
	B()
	{
		i=2;
	}
	int j;

};
 template <class C >  class fact_T
 {
 public:
	 void run_func(C &obj);
	 template <class OTHER > void func(OTHER &obj);	
 };
 

template <> template <class OTHER > void fact_T<A>::func(OTHER &obj)
{
	printf("i=%d\n",obj.i);

}
template <> template <class OTHER > void fact_T<B>::func(OTHER &obj)
{
	printf("i=%d\n",obj.i);

}
fact_T<A> fA;
fact_T<B> fb;
template <>  void fact_T<A>::run_func(A& x)
{
	fA.func(x);
}
template <>  void fact_T<B>::run_func(B& x)
{
	fA.func(x);
	fb.func(x);
}
 void templates()
 {


	A a;
	B b;

	fA.run_func(b);











 }