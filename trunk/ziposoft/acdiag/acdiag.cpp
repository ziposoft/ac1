// test_console.cpp : Defines the entry point for the console application.
//

#include "acdiag.h"
#include "zipolib/include/z_factory_static.h"

class Animal
{
public:
	Animal()
	{
		legs=2;
		name="george";
	}
	virtual int makesound()
	{
		printf("could be anything!\n");
		return 0;
	}
	int legs;
	z_strlist nicknames;

	z_string name;
};
class Flea : public Animal
{
public:
	Flea()
	{
		legs=6;
	}
	virtual int makesound()
	{
			printf("itch!\n");
		return 0;
	}
};
class Dog : public Animal
{
public:
	Dog()
	{
		barks=1;
		legs=4;
	}
	int barks;
	z_obj_vector<Flea> fleas;

	virtual int makesound()
	{
		int i;
		for (i=0;i<barks;i++)
			printf("woof!\n");
		return 0;
	}
};
class Bird : public Animal
{
public:
	Bird()
	{
		legs=2;
	}
	virtual int makesound()
	{
		printf("chirp!\n");
		return 0;
	}
};
class Cat : public Animal
{
public:
	Cat()
	{
		legs=4;
	}
	virtual int makesound()
	{
		printf("meow!\n");
		return 0;
	}
};
class House
{
public:
	House()
	{
		onepet=0;//new Cat();
		newanimals=9;
	}
	z_obj_vector<Animal> pets;
	int newanimals;
	Animal* onepet;
	int hoard()
	{
		int i;
		for(i=0;i<newanimals;i++)
		{
			pets << new Cat();
			pets << new Bird();
			pets << new Dog();

		}
		return 0;
	}

};
ZFACT(Animal)
{
	ZACT(makesound);

	
	ZPROP(nicknames);
	ZPROP(legs);
	ZPROP(name);
};
ZFACT(House) 
{
	ZACT(hoard);
	ZVOBJ(onepet);
	ZPROP(pets);
	ZPROP(newanimals);
};
ZFACT_V(Dog,Animal) 
{
	ZPROP(barks);
	ZPROP(fleas);
};
ZFACT_V(Flea,Animal){};
ZFACT_V(Cat,Animal){};
ZFACT_V(Bird,Animal){};




class root
{
public:
	root()
	{
		x.push_back("x");
		x<<"y";
		s="ant\"hony";
		i=27;
		_p_logger=&gz_logger;

	}
	z_console console;
	z_logger* _p_logger;
	House house;
	z_strlist x;
	int i;
	z_string s;
	int add() {  x<<s;return 0;}
	int show() { gz_out << '\n'<<s<<'\n';return 0;}


};
ZFACT(root)
{
	ZOBJ(console);
	ZOBJ(house);
	ZPOBJ(_p_logger);
	ZPROP(x);
	ZPROP(i);
	ZACT(add);
	ZACT_XP(show,"show","desc",1,ZPARAM(s));

};
#ifdef BUILD_VX
int ztest()
{
	gz_out.set_handle((size_t)stdout);
	gz_in.set_handle((size_t)stdin);
	gz_out << "starting ztest\n";
	
	root o;
	o.console.setroot(&o);
	o.console.run();
	return 0;

}
#else
int main(int argc, char* argv[])
{

	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv);


	return 0;
}

#endif




//ZP_MODULE_INCLUDE( ZP_MOD(parse), ZP_MOD(logger));
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")

