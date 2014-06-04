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
		_str="fr\"ed";

	}
	virtual ~testAd(){}
	int  _i;
	z_string _str;
	testAs child;
	z_strlist sl;
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
void z_console_ntf::OnDoubleBack()
{
	if(	_path.size())
		_path.pop_back();
	select_obj_from_path(_root,_path);
	_selected=_temp;
	gz_out << "\n";
	put_prompt();


}
z_status z_console_ntf::select_obj_from_path(zf_obj& start,z_strlist& list)
{
	z_status status;
	_temp= start;
	size_t i;
	for(i=0;i<list.size();i++)
	{
		ctext name=list[i];
		status=select_obj(name);
		if(status)
			return Z_ERROR(status);
	}
	return zs_ok;

}
z_status z_console_ntf::navigate_to_obj()
{
	z_status status;
	_temp= _selected;
	_temp_path=_path;

	if(_cmdline._root_slash)
	{
		_temp=_root;
		_temp_path.clear();
	}
	size_t i;
	for(i=0;i<_cmdline._path_list.size();i++)
	{
		ctext name=_cmdline._path_list[i];
		status=select_obj(name);
			
		if(status)
			return Z_ERROR(status);
		_temp_path<<name;
	}
	if(_cmdline._object)
	{
		status=select_obj(_cmdline._object);
		if(status)
			return Z_ERROR(status);
		_temp_path<<_cmdline._object;
	}
	return zs_ok;

}

z_status z_console_ntf::evaluate_feature(zf_obj& o)
{
	z_status status;
	zf_feature*  feature;
	int index=-1;
	if(!_cmdline._feature)
		return Z_ERROR(zs_error);
	status=o._fact->get_feature(_cmdline._feature->_name,feature);


	if(status)
		//May not be an error. If it is searching multiple objects.
		return zs_feature_not_found; 

		//return Z_ERROR_MSG(status,"Feature \"%s\" not found\n",_cmdline._feature->_name.c_str());

	void* ftr_ptr=(char*)o._obj+feature._offset;
	if(_cmdline._feature->_sub)
		index=_cmdline._feature->_sub->_id.GetDecVal();

	if(_cmdline._assignment)
	{
		if(!feature.df)
			return Z_ERROR_MSG(zs_error,"Cannot assign value to function\n");//???
		if(!_cmdline._assign_val)
		{
			gz_out<<"Clearing member \""<<feature._name<<"\"\n";
			feature.df->clear(ftr_ptr);
			return zs_ok;


		}
		feature.df->set_from_value(_cmdline._assign_val,ftr_ptr,index);

		return zs_ok;
	}
	if(feature._type==zf_ft_act)
	{
		if(_cmdline._params)
		{
			zf_action* action=(&feature)->get_action();
			if(!action)
			{
				return Z_ERROR_MSG(zs_error,"Action not an action\n");//???
			}
			size_t num_valid_params=action->_params.size();
			int i;
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

		int ret=o._fact->execute_act_ptr	(o._obj,feature._offset);
		return zs_ok;//???
	}
	if(!feature.df)
		return Z_ERROR(zs_error);//???

	if(feature._type==zf_ft_var)
	{
		z_string str;
		feature.df->get(str,ftr_ptr,index);
		gz_out << feature._name<<"="<<str<<"\n";
		return zs_ok;//???

	}
	if(feature._type==zf_ft_obj)
	{
		void* obj=feature.get_var_ptr(o._obj);
		if(!obj)
			return Z_ERROR_MSG(zs_error,"Cannot select NULL object \"%s\".",feature._name.c_str());

		_temp_path<< feature._name;

		_selected._fact=feature.df->get_child_obj_fact();
		_selected._obj=feature.get_var_ptr(o._obj);
		_path=_temp_path;

	}
	//feature.df->dump(gz_out,ftr_ptr);
	//gz_out<<"\n";

	return zs_ok;//???
}


void z_console_ntf:: OnTab()
{
	if(!_tab_mode)
	{
		z_status status=parse_line(_buffer);
		if(status)
		{
			gz_logger.dump();
				put_prompt();
			return ;
		}

		status=navigate_to_obj();
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
		if(_cmdline._feature)
		{
			z_string& s=_cmdline._feature->_name;
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

z_status z_console_ntf:: ExecuteLine(ctext text)
{
	z_status status=parse_line(text);
	if(status)
		return status;
	_temp= _selected;

	if(_dump_cmd_line)
		zf_dump_obj(&_cmdline);

	if(!_cmdline.has_path())
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
		if(status==zs_feature_not_found)
			return Z_ERROR_MSG(status,"Feature \"%s\" not found\n",_cmdline._feature->_name.c_str());

	}
	else
	{
		if(_cmdline.has_path())//if they just provide a path then assign the path
		{
			_path=_temp_path;
			_selected=_temp;
		}

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
	cfg.load_obj(_root._obj,_root._fact);
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
	ZACT(run);
	ZPROP_X(_dump_cmd_line,"dump_cmdline","Dump the parsed command line contents");
	ZPROP_X(_path,"path","Current path");
	ZPROP_X(_history,"history","Command line history");
	ZPROP_X(_config_file,"cfgfile","Filename of configuration file");
	ZPROP_X(_script_file,"script","Filename of script to run/save");


}

class root
{
public:
	root()
	{
		x.push_back("x");
		x<<"y";
		s="ant\"hony";
		i=27;

	}
	z_console_ntf console;
	testAd a;
	z_strlist x;
	int i;
	z_string s;
	int add() {  x<<s;return 0;}
	int show() { gz_out << '\n'<<s<<'\n';return 0;}


};
ZFACT(root)
{
	ZOBJ(console);
	//ZOBJ(a);
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
	gz_out << "load save args...\n";
	z_debug_load_save_args(&argc,&argv);
	gz_out << "load save args done\n";




	z_status status=zs_no_match;
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



#define ZO_OBJ_LIST \
	ZCLS(testAs,void,"cmdline","{_val}ident:'=':{i123}int",ACT(func) ACT(func2) VAR(i) POBJ(child))  \
	ZCLS(testAd,void,"cmdline","{_val}ident:'=':{i123}int",ACT(func) VAR(sl)  VAR(_str) OBJ(child)) 


#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod), ZP_MOD(parse));

#pragma comment(linker, "/alternatename:zp_module_master_list_size=zp_module_master_list_size_exe")
