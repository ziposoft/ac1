#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include  <string>

class Abc
{
public:
	virtual int func1() const{ return 1; }
	virtual void func2(int x) const {  }
	virtual void func3(int x) const {  }
};
 template <class VAR >  class Bcd  : public Abc
 {
 public:
	virtual void func2(int x) const;

 };
// template <class VAR > void B<VAR>::func2(int x) const { printf("default  func2");};
 //template <class VAR > void B<VAR>::func3(int x) const {};
