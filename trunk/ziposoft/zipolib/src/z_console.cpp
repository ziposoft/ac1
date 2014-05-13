#if 0

#include "z_parse_pch.h"
#include "z_console.h"




z_console::z_console()
{
	index=0;
	cur_start=0;
	len=0;
	insertmode=true;
	TabMode=false;
}
U32 z_console::get_index() { return index;}
U32 z_console::get_line_length() { return len;}
void z_console::AppendChar(char ch)
{
	
	buff+=ch;
	gz_out <<ch;
	len++;
	cur_x++;
	index++;
}
void z_console::RedrawLine(int blanks)
{
	const char* s;
	s=buff;
	s+=index;
	gz_out << s;
	while(blanks--) gz_out << ' ';
	curGotoXY(cur_x,cur_y);
}
void z_console::InsertChar(char ch)
{
	cur_x++;
	buff.insert(index,1,ch);
	RedrawLine();
	len++;
	index++;
}
void z_console::OverwriteChar(char ch)
{
	cur_x++;
	buff.replace(index,1,1,ch);
	RedrawLine();
	index++;
}
void z_console::hChar(char ch)
{
	if (len==index)
	{
		AppendChar(ch);
		return;
	}
	if(insertmode)  InsertChar(ch);
	else OverwriteChar(ch);
}
void z_console::reset_line()
{
	index=0;
	len=0;
	buff="";
	GetXY();
	cur_start=cur_x;
	
}
void z_console::output(ctext text)
{
	U32 l=(U32)strlen(text);
	gz_out << text;
	len+=l;
	buff+=text;
	index+=l;
	cur_x+=l;
}
void z_console::clear_line()
{
	curLeft(index);
	while(len--) gz_out << ' ';
	curGotoXY(cur_x,cur_y);
	index=0;
	len=0;
	buff="";
}
void z_console::trim_line_to(int trim_point)
{
	int amount_to_trim=len-trim_point;
	cur_x=cur_start+trim_point;
	curGotoXY(cur_x,cur_y);
	while(amount_to_trim--) gz_out << ' ';
	curGotoXY(cur_x,cur_y);
	index=trim_point;
	buff.erase(trim_point);
	//del(buff,trim_point);
	len=trim_point;
}
void z_console::run()
{
	_running=true;
	reset_line();
	
	_key=key_alpha;
	char ch;
	while(_running)
	{
		_prev_key=_key;
		GetKey(_key,ch);
		switch(_key)
		{
		case key_ctrl_C:
			_running=false;
			break;
		case key_insert:
			insertmode=!insertmode;
			break;
		case key_enter:
			OnEnter();
			break;
		case key_tab:
			if(_prev_key!=key_tab) TabMode=false;
			OnTab();
			break;
		case key_up:
			
			OnUp();
			break;
		case key_down:
			
			OnDown();
			break;

		case key_right:
			if(index<len) 
			{
				curRight(1);
				index++;
			}
			break;
		case key_left:
			if(index>0)
			{
				curLeft(1);
				index--;
			}
			
			break;
		case key_back:
			if(index>0)
			{
				curLeft(1);
				//gz_out << char_back;
				index--;
				len--;
				buff.erase(index,1);
				//del(buff,index,1);
				RedrawLine(1);
			}
			else
			{
				if(_prev_key==key_back)
				{
					OnDoubleBack();
				}
			}
			break;
		case key_delete:
			if(index<len)
			{
				
				len--;
				buff.erase(index,1);
				//del(buff,index,1);
				RedrawLine(1);
			}
			break;
		case key_alpha:
			
			hChar(ch);
			gz_out.flush();
			break;
		default:
			break;
		}
	}
}

void z_console::put_prompt()
{
	_path="";
	//_obj_current->get_path(_path);
	gz_out  << _path << ">";
	reset_line();
}


z_status z_console::parse_line(ctext text)
{
	z_status status=_parser.parse_obj(&_cmd_line_obj,text);
	if(status)
	{
		_parser.report_error(status);

		return status;
	}
	return status;
}
z_status z_console::execute_line(ctext text)
{
	z_status status=parse_line(text);
	if(status)
		return status;



	zo_manipulator man;

	man.dump_obj(&gz_out,&_cmd_line_obj);
	
#if 0
//find path
	if(_cmd_line_obj._root)
	{
		_obj_current_tmp=_root_obj;
	}
	else
	{
		_obj_current_tmp=_obj_current;
	}
	if(_obj_current_tmp)
	{
		if(_cmd_line_obj._path_list.size())
		{
			size_t i;
			for (i=0;i<_cmd_line_obj._path_list.size();i++)
			{
				ctext name=_cmd_line_obj._path_list[i];
				status=navigate_feature(name);
				if(status)
					return status;

			}
		}
		if(_cmd_line_obj._object)
		{
			ctext name=_cmd_line_obj._object;
			void* child=get_child_obj(name,_obj_current_tmp);
			if(!child)
			{
				Z_ERROR_MSG("Could not find child object \"%s\"",name);
				return zs_child_not_found;
			}
			_obj_current_tmp=child;
		}
		status=execute_feature(_obj_current_tmp);
	}
	else
		status=zs_no_entry_for_item;


	if(status==zs_no_entry_for_item)
		status=execute_feature(this);
	if(status==zs_no_entry_for_item)
		Z_ERROR_MSG("Unknown feature:\"%s\"",_cmd_line_obj._feature._name.c_str());
#endif
	return status;
}
void z_console::OnEnter()
{
	int i;
	z_status result;
	gz_out << '\n';
	if(buff.size())
	{
		i=_history.find(buff);
		if(i!=-1)
		{
			_history.del(i);
		}
		_history<<buff;
		_history_index=(U32)_history.size();
	}
	if(buff.size())
	{
		//parse_line(buff,_zc);
		result=execute_line(buff);
		if(result) 
		{
			switch(result)
			{
			case zs_no_match:
			case zs_unparsed_data:
				gz_out << "\nsyntax error at \"" << _parser.get_char_under_test()<<"\"\n";
				_parser.print_context();
				break;
			default:
				break;
			}
			gz_out << "\ncommand failed.\n";

		}

	}
	put_prompt();

}

void z_console::inc_history(int i)
{
	int history_count=(int)_history.size();
	if(history_count==0) return;
	clear_line();
	_history_index+=i;
	if(_history_index>=history_count)
	{
		_history_index=0;
	}		
	if(_history_index<0)
	{
		_history_index=history_count-1;
	}
	output( _history[_history_index]);



}
void z_console::OnUp()
{
	inc_history(-1);
};
void z_console::OnDown()
{
	inc_history(1);

};
void z_console::OnTab()
{


}



void z_console::OnDoubleBack()
{


}


#if 0
#define Z_MODULE _Z_MODULE(console)
#define ZO_OBJ_LIST \
	OBJ(zo_console,void,"console","console desc",0,\
		PROP(_config_file,"cfgfile","cfg","Name of configuration file",0)\
		PROP(_startup_path,"path","path","Startup Path",0)\
		LIST(_history,"history",0,0,0)\
		VAR(_executable_name)\
		ACT(list_features,0,"lf",0,0,NO_PARAMS)\
		ACT(savecfg,0,"sc",0,0,PRM("cfgfile"))\
		ACT(dumpcfg,0,"dc",0,0,NO_PARAMS)\
		ACT(loadcfg,0,"lc",0,0,PRM("cfgfile"))\
		ACT(help,0,"h",0,0,NO_PARAMS)\
		ACT(shell,0,0,0,0,NO_PARAMS)\
		ACT(exit,0,"quit",0,0,NO_PARAMS)\
		)\
	OBJ(zo_root,z_obj_vect<void>,"root","root desc",0,\
		CHILD(_console,"console","console","console desc",0)\
		PCHILD(_p_trace,"trace","trace","trace desc",0)\
		/*LIST(_child_list,"children",0,0,0)\ */ \
		)

	

#include "zipolib/include/z_obj_macro.h"



zo_root::zo_root()
{
	_p_trace=&g_z_trace;
	_console.set_root(this);

}
ctext zo_root::get_map_key()
{
	return "";
}




void zo_console::refresh()
{
	//prompt();
}

void zo_console::OnDoubleBack()
{
	//if(!use_double_back) return;
	if(!_obj_current)
		return;
	void* p=_obj_current->get_parent_obj();
	if(!p)
		return;
	_obj_current=p;
	gz_out << '\n';
	put_prompt();

}
void zo_console::runz(void* obj)
{
	_history_index=-1;
	_feature_index=0;
	if(obj)
		_obj_current=obj;
	else
		_obj_current=this;

	//np.exec(_zc,this);
	put_prompt();
	run();
}
void zo_console::show_history()
{
	U32 i;
	gz_out<<'\n';
	for (i=0;i<_history.size();i++)
	{
		gz_out<<'\t' <<_history[i]<<'\n';

	}

}
void zo_console::inc_history(int i)
{
	int history_count=(int)_history.size();
	if(history_count==0) return;
	clear_line();
	_history_index+=i;
	if(_history_index>=history_count)
	{
		_history_index=0;
	}		
	if(_history_index<0)
	{
		_history_index=history_count-1;
	}
	output( _history[_history_index]);



}
void zo_console::OnUp()
{
	inc_history(-1);
};
void zo_console::OnDown()
{
	inc_history(1);

};
void zo_console::display_text(const z_string& s)
{
	gz_out<< s << '\n';
}
void zo_console::put_prompt()
{
	path="";
	_obj_current->get_path(path);
	gz_out  << path << ">";
	reset_line();
}

void zo_console::get_auto_complete_list(zp_feature& parse_feature)
{
	zp_var_entry* f;


	int i=0;

	z_string partial_string=parse_feature._name;
	zo_feature_list full_list;
	_obj_current->get_feature_map(this,full_list,ZO_MT_ALL,true);
	get_feature_map(this,full_list,ZO_MT_ALL,true);

	_auto_tab.clear();
	_auto_tab.reset_iter();
	full_list.reset_iter();

	
	while(f=full_list.get_next())
	{
		ctext name=full_list._internal_iter.key;

		if(strncmp(partial_string.c_str(),name,partial_string.size())==0)
			_auto_tab.add(name,f);

	}
	return ;

}

void zo_console::OnEnter()
{
	int i;
	z_status result;
	gz_out << '\n';
	if(buff.size())
	{
		i=_history.find(buff);
		if(i!=-1)
		{
			_history.del(i);
		}
		_history<<buff;
		_history_index=(U32)_history.size();
	}
	if(buff.size())
	{
		//parse_line(buff,_zc);
		result=execute_line(buff);
		if(result) 
		{
			switch(result)
			{
			case zs_no_match:
			case zs_unparsed_data:
				gz_out << "\nsyntax error at \"" << _parser.get_char_under_test()<<"\"\n";
				_parser.print_context();
				break;
			default:
				break;
			}
			gz_out << "\ncommand failed.\n";

		}

	}
	put_prompt();
}
void zo_console::OnTab()
{
	z_string feature_name="";
	
	if(!TabMode)
	{
		int len=0;
		z_status status=parse_line(buff);
		if(status==zs_ok)
		{
			//TODO - user hits tab with bad line		
			//return;
		}
		get_auto_complete_list(_cmd_line_obj._feature);

		len=(int)_cmd_line_obj._feature.get_num_chars();
		
		_tab_count=(U32)_auto_tab.size();
		_auto_tab.reset_iter();
		TabMode=true;
		_tab_mode_line_index=get_line_length()-len;
	}
	if(!_tab_count)
		return;
	zp_var_entry* fe=_auto_tab.get_next();
	if(!fe)
	{
		_auto_tab.reset_iter();
		fe=_auto_tab.get_next();
	}
	ctext name=_auto_tab._internal_iter.key; //UGLY!!! TODO
	trim_line_to(_tab_mode_line_index);
	output(name);
	return;
	
}

void zo_console::dump_feature_outline(void* obj)
{
	const zp_factory* fact=obj->get_fact();
	if(!fact)
		return;
	int i_var;
	for(i_var=0;i_var<fact->var_list_size;i_var++)
	{
		zp_var_entry& p_var_entry=fact->var_list[i_var];
		gz_out<< "\t\t"<< p_var_entry._internal_name;
		if(obj)
			gz_out<<"="<<feature_get_as_string(obj,&p_var_entry);

		gz_out<<"\n";
	}
}
void zo_console::dump_all()
{
	int i_module;
	for(i_module=0;i_module<z_module_master_list_size;i_module++)
	{
		const z_module_entry* p_module=z_module_master_list[i_module];
		gz_out<< "MODULE: "<< p_module->module_name<<"\n";
		int i_obj;
		for(i_obj=0;i_obj<p_module->num_facts;i_obj++)
		{
			const z_module_obj_entry& p_obj_entry=p_module->facts[i_obj];
			const zp_factory* fact=p_obj_entry.fact;
			gz_out<< "\t"<< p_obj_entry.name;
			if(fact->base_fact)
				gz_out<< "::"<< z_obj_fact_get_name(fact->base_fact);
			gz_out<<"\n";
		}
	}
}
void zo_console::dump_obj2(void* obj)
{
	const zp_factory* fact=obj->get_fact();
	gz_out<< z_obj_fact_get_name(fact)<<"\n";
	gz_out<< fact->desc<<"\n";
}


z_status zo_console::list_features()
{
	return dump_features(&gz_out,_obj_current);
	
	
}
z_status zo_console::shell()
{
	terminal_open();
	runz(_obj_current);
	return zs_ok;
}

z_status zo_console::help()
{
	return dump_features(&gz_out,this);
	return zs_ok;
}
z_status zo_console::exit()
{
	gz_out<< "exit!!\n";
	_running=false;
	return zs_ok;
}




z_status zo_console::dumpcfg()
{
	dump_obj(&gz_out,_root_obj);
	gz_out<< "\n\n";
	return zs_ok;
}
z_status zo_console::savecfg()
{
	z_file f(_config_file,"wb");
	dump_obj(&f,_root_obj);
	return zs_ok;
}
z_status zo_console::loadcfg()
{
	z_string filename=_config_file;
	z_file f(filename,"rb");
	void_parse o;
	z_string data_in;
	f.read_all(data_in);
	z_status status=_parser.parse_obj(&o,data_in);
	if(status!=zs_ok)
	{
		_parser.report_error(status);
		return status;
	}
	//dump_obj(&gz_out,&o);
	load_obj(_root_obj,&o);
	_config_file=filename;
	return zs_ok;
}


z_status zo_console::process_args(int argc, char** argv)
{
	if(argc<1)
	{
		return zs_bad_argument_1;
	}
	_executable_name=argv[0];
	if(_executable_name.size()>4)
	{
		std::string ext=_executable_name.substr(_executable_name.size()-4,4);
		if(stricmp(ext.c_str(),".exe")==0)
			_executable_name.resize(_executable_name.size()-4);
	}
	_config_file=_executable_name;
	_config_file+=".cfg";


	if(argc>1)
	{
		int i;
		for(i=1;i<argc;i++)
			execute_line(argv[i]);
	}
	else
		list_features();

	return zs_ok;

}
#endif
#endif