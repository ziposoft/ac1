// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
#include "zipolib/include/z_parse.h"
#include "zipolib/include/z_error.h"



class testAs 
{
public:
	testAs() { i=0;child=0; }
	int func() { printf("hooray!!\n");return 0;};
	int func2() { printf("time for a %d min nap!!\n",i);return 0;};
	int i;
	testAs* child;
};
class testAd
{
public:
	testAd()
	{
		_i=123;
		_str="fred";

	}
	virtual ~testAd(){}
	int  _i;
	z_string _str;
	testAs child;
	int func()
	{
		printf("hooorraaayy!!! %d  %s\n",_i,_str.c_str());
		return 0;
	}
};
z_status z_console_ntf::select_obj(ctext name)
{
	z_status status;
	zf_feature f;
	status=_temp._fact->get_feature(name,f);
	if(status)
		return status;
	if(f._type==zf_ft_obj)
	{
		void* v=f.get_var_ptr(_temp._obj);
		if(!v)
			return Z_ERROR(zs_feature_not_found);

		const z_factory* fact=f.df->get_child_obj_fact();
		if(!fact)
			return Z_ERROR(zs_error);			
		_temp._obj=v;
		_temp._fact=fact;
		return zs_ok;
	}

	return Z_ERROR_MSG(zs_error,"\"%s\" is not an object",name);
}

z_status z_console_ntf::navigate_to_obj()
{
	z_status status;
	_temp= _selected;
	if(_cmdline._path)
	{

		if(_cmdline._path->_root_slash)
			_temp=_root;
		size_t i;
		for(i=0;i<_cmdline._path->_path_list.size();i++)
		{
			status=select_obj(_cmdline._path->_path_list[i]);
			if(status)
				return Z_ERROR(status);
		}
	}
	if(_cmdline._object)
	{
		status=select_obj(_cmdline._object);
		if(status)
			return Z_ERROR(status);
	}
	return zs_ok;

}

z_status z_console_ntf::evaluate_feature(zf_obj& o)
{
	z_status status;
	zf_feature f;
	if(!_cmdline._feature)
		return Z_ERROR(zs_error);
	status=o._fact->get_feature(_cmdline._feature->_name,f);


	if(status)
		return status;

	void* ftr_ptr=(char*)o._obj+f._offset;

	if(_cmdline._assignment)
	{
		if(!f.df)
			return Z_ERROR_MSG(zs_error,"Cannot assign value to function\n");//???
		if(!_cmdline._assign_val)
		{
			gz_out<<"Clearing member \""<<f._name<<"\"\n";
			f.df->clear(ftr_ptr);
			return zs_ok;


		}
		f.df->set(_cmdline._assign_val->_string,ftr_ptr);

		return zs_ok;
	}
	if(f._type==zf_ft_act)
	{
		int ret=o._fact->execute_act_ptr	(o._obj,f._offset);
		return zs_ok;//???
	}
	if(!f.df)
		return Z_ERROR(zs_error);//???
	gz_out << f._name<<"=";
	f.df->dump(gz_out,ftr_ptr);
	gz_out<<"\n";

	return zs_ok;//???
}
z_status z_console_ntf:: OnExecuteLine(ctext text)
{
	z_status status=parse_line(text);
	if(status)
		return status;
	_temp= _selected;
	if(!_cmdline._path)
	{
		//if no path is specified, then try the built in commands
		status=evaluate_feature(_self);
		if(status==zs_ok)
			return 	zs_ok;
	}
	zf_feature f;
	status=navigate_to_obj();
	if(status)
	{
		gz_out <<"Bad path\n";
		return status;
	}

	if(_cmdline._feature)
	{
		status=evaluate_feature(_temp);
	}
	else
	{


	}



	return status;
}


z_status z_console_ntf::list_features()
{
	_temp._fact->dump_obj(gz_out,_temp._obj);
	gz_out << "\n";

	return zs_ok;
}
z_status z_console_ntf::dumpcfg()
{

	return zs_ok;
}
z_status z_console_ntf::loadcfg()
{
	z_file f(_config_file,"rb");
	zp_cfg_file cfg;
	z_string data_in;
	f.read_all(data_in);
	z_status status=_parser.parse_obj(&cfg,data_in);
	if(status!=zs_ok)
	{
		_parser.report_error();
		return status;
	}
	//cfg._obj.get_by_name(
	return zs_ok;

	return zs_ok;
}
z_status z_console_ntf::savecfg()
{
	z_file f(_config_file,"wb");
	_root._fact->dump_obj(f,_root._obj);
	return zs_ok;
}
z_status z_console_ntf::help()
{
	gz_out << "help..\n";
	return zs_ok;
}
z_status z_console_ntf::exit()
{
	gz_out<< "exiting.\n";
	_running=false;
	return zs_ok;
}

ZFACT(z_console_ntf)
{
	ZACT_X(exit,"q","Quit/Exit");
	ZACT_X(list_features,"ls","List features");
	ZACT(list_features);
	ZACT(loadcfg);
	ZACT(savecfg);
	ZACT(help);
	ZACT(exit);
}

int main(int argc, char* argv[])
{
	z_status status=zs_no_match;
	/*
	testA A;

	zfs_get_static_factory("testA")->execute_act(&A,"func");
	zfs_get_static_factory("testA")->set_var_as_string(&A,"i","67");
	zfs_get_static_factory("testA")->execute_act(&A,"func2");
	*/
	gz_out << "load save args...\n";
	z_debug_load_save_args(&argc,&argv);
	gz_out << "load save args done\n";

	int i;
	//ZT_ENABLE();
	z_console_ntf console(argv[0]);

	zp_cmdline 	cmdline ;
	z_parser parser;
	for(i=1;i<argc;i++)
	{
		gz_out << "parsing [%s]:\n";

		status=parser.parse_obj(&cmdline,argv[i]);
		if(status==	zs_ok)
		{
			zf_dump_obj( &cmdline);



		}
		else
			parser.report_error();



	}
	testAd theobj;
	console.run_T(&theobj);
	return 0;
}





#define ZO_OBJ_LIST \
	ZCLS(testAs,void,"cmdline","{_val}ident:'=':{i123}int",ACT(func) ACT(func2) VAR(i) POBJ(child))  \
	ZCLS(testAd,void,"cmdline","{_val}ident:'=':{i123}int",ACT(func)  VAR(_str) OBJ(child)) 


#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod), ZP_MOD(parse));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")