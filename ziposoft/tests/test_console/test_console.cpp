// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
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
		onepet=new Cat();
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
	//gz_out << "load save args...\n";
	z_debug_load_save_args(&argc,&argv);
	//gz_out << "load save args done\n";


	z_status status;
	root o;
	o.console.setroot(&o);

	int i;
	for(i=1;i<argc;i++)
	{

		status=o.console.ExecuteLine(argv[i]);
		if(status)
			gz_logger.dump();



	}
	if(argc==1)
		o.console.run();



	return 0;
}

#endif




//ZP_MODULE_INCLUDE( ZP_MOD(parse), ZP_MOD(logger));
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")


/*
OLD
*/
#if OLD
void z_console:: OnTab_old()
{
	if(!_tab_mode)
	{
		z_status status=EvaluateLine(_buffer);
		if(status)
		{
			gz_out <<"\nBad path\n";
			gz_logger.dump();
				put_prompt();
			return ;
		}


		_auto_tab.clear();		
		_temp._fact->get_list_features(_auto_tab);
		if(!_cmdline.has_path())
		{
			_self._fact->get_list_features(_auto_tab);

		}

		_tab_mode_line_index=get_line_length();
		if(_has_feature)
		{
			zp_feature* zpf=_cmdline.get_feature();
			if(zpf)
			{
				z_string& s=zpf->_name;
				_tab_mode_line_index-=s.size();
				size_t i=0;
				while(i<_auto_tab.size())
				{
					if(_auto_tab[i].compare(0,s.size(),s))
						_auto_tab.del(i);
					else
						i++;
				}
			}
		}

		
		_tab_count=_auto_tab.size();
		_tab_mode=true;
		_tab_index=0;

	}
	if(!_tab_count)
		return;
	trim_line_to(_tab_mode_line_index);
	output(_auto_tab[_tab_index]);
	_tab_index++;
	if(_tab_index>=_tab_count)
		_tab_index=0;


}

z_status z_console:: ExecuteLine_old(ctext text)
{
	z_status status=EvaluateLine_old(text);
	if(status)
		return status;
	if(!_cmdline.has_path())
	{
		//if no path is specified, then try the built in commands
		status=evaluate_feature_old(_self);
		if(status==zs_ok)
			return 	zs_ok;
	}

	status=evaluate_feature_old(_temp);

	return status;
}
z_status z_console::evaluate_feature_old(zf_obj& o)
{
	z_status status;
	if(!_has_feature)
	{
		_path=_temp_path;
		_selected=_temp;
		return zs_ok;
	}

	zp_feature* zpf=_cmdline.get_feature();
	zf_feature *zff;
	zff=o._fact->get_feature(zpf->_name);
	if(!zff)
		return Z_ERROR(zs_feature_not_found);
	int index=-1;
	if(zpf->_sub)
		index=zpf->_sub->_id.GetDecVal();


	if(zff->_type==zf_ft_act)
	{
		if(_cmdline._params)
		{
			zf_action* action=zff->get_action();
			if(!action)
			{
				return Z_ERROR_MSG(zs_error,"Action not an action\n");//???
			}
			size_t num_valid_params=action->_params.size();
			size_t i;
			for(i=0;i<_cmdline._params->_param_list.size();i++)
			{
				zp_value* value=_cmdline._params->_param_list[i];
				if(i>=num_valid_params)
				{
					return Z_ERROR_MSG(zs_error,"Too many parameters\n");//???
				}
				zf_feature* param=action->_params[i];
				void* ftr_ptr=(char*)o._obj+param->_offset;
				param->df->set_from_value(value,ftr_ptr);//TODO check status

			}

		}

		int ret=o._fact->execute_act_ptr	(o._obj,zff->_offset);
		return zs_ok;//???
	}
	void* membervar=zff->get_memvar_ptr(o._obj);
	if(!membervar)
		return Z_ERROR(zs_feature_not_found);


	if(_cmdline._assignment)
	{
		if(!zff->df)
			return Z_ERROR_MSG(zs_error,"Cannot assign value to function\n");//???
		if(!_cmdline._assign_val)
		{
			gz_out<<"Clearing member \""<<zff->_name<<"\"\n";
			zff->df->clear(membervar);
			return zs_ok;


		}
		status=zff->df->set_from_value(_cmdline._assign_val,membervar,index);

		return status;
	}
	if(!zff->df)
		return Z_ERROR(zs_error);//???

	if(zff->df->get_type()==zf_ft_var)
	{
		z_string str;
		zff->df->get(str,membervar,index);
		gz_out << zff->_name<<"="<<str<<"\n";
		return zs_ok;//???

	}


	return zs_ok;//???
}
z_status z_console:: EvaluateLine_old(ctext text)
{
	_has_feature=false;
	z_status status=parse_line(text);
	if(status)
		return status;
	_temp= _selected;
	_temp_path=_path;

	if(_dump_cmd_line)
		zf_dump_obj(&_cmdline);
	if(_cmdline._root_slash)
	{
		_temp=_root;
		_temp_path="";
	}
	size_t objcount=_cmdline._path.size();
	size_t i;
	if(!objcount)
		return zs_ok;
	z_string s;
	bool needs_slash=false;
	for(i=0;i<objcount;i++)
	{

		status=select_obj_old(_cmdline._path[i]);
		if(status)
		{
			if(i==(objcount-1))
			{
				_has_feature=true;
				return zs_ok;
			}
			return Z_ERROR(status);
		}
		_temp_path<<"/"<< _cmdline._path[i]->get_full_name(s);
		needs_slash=true;
	}

	return zs_ok;
}
z_status z_console::select_obj_old(zp_feature* zpf)
{
	zf_feature *f;
	f=_temp._fact->get_feature(zpf->_name);
	if(!f)
		return zs_feature_not_found;
	int index=-1;
	if(zpf->_sub)
		index=zpf->_sub->_id.GetDecVal();
	if(!f->df)
		return zs_error;

	char* membervar=(char*)_temp._obj+f->_offset;


	void * subobj=f->df->get_sub_obj(membervar,index);
	if(!subobj)
	{
		return Z_ERROR_MSG(zs_error,"\"%s\" is not an object",zpf->_name.c_str());
	}

	z_factory* fact=f->df->get_fact_from_obj(subobj);
	if(!fact)
		return Z_ERROR(zs_error);			
	_temp._obj=subobj;
	_temp._fact=fact;

	return zs_ok;
}
#endif