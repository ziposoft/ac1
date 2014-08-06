#include "zipolib_cpp_pch.h"
#include "z_console.h"
#include "z_factory_static.h"
#include "z_filesystem.h"

ZFACT(z_console)
{
	ZACT_X(exit,"q","Quit/Exit");
	ZACT_X(list_features,"ls","List features");
	ZACT(list_features);
	ZACT(dumpcfg);
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


/*________________________________________________________________________

z_console
________________________________________________________________________*/

z_console::z_console()
{
	_self._fact=&z_factory_T<z_console>::self;
	_self._obj=this;
	_selected= _self;
	_root._fact=0;
	_root._obj=0;
	_config_file="console.cfg";
	_dump_cmd_line=false;
	z_filesys_getcwd(_startup_path);
}

z_status  z_console::runapp(int argc, char* argv[])
{
 	z_status status;
 	z_debug_load_save_args(&argc,&argv);
	

	int i;
	for(i=1;i<argc;i++)
	{

		status=ExecuteLine(argv[i]);

		if(status)
		{
			Z_ERROR_MSG(status,"command failed: \"%s\"",argv[i]);
			z_logger_dump();
		}



	}
	if(argc==1)
		run();


	return zs_ok;

}
void z_console::OnDoubleBack()
{
	
	size_t slash=_path.rfind('/');
	if(slash==-1)
		_path="/";
	else
		_path.resize(slash);

	_selected=_root;
	z_string temp=_path;
	_path="";
	select_obj_from_path(_root,temp);
	_selected=_temp;
	_path=_temp_path;
	gz_out << "\n";
	put_prompt();


}
z_status z_console::select_obj_from_path(zf_obj& start,z_string& path)
{

	ExecuteLine(path);


	return zs_ok;

}

z_status z_console::get_feature_and_index()
{
	z_status status;	
	status=_tparser.test_any_identifier();
	if(status) 
		return status;
	_tparser.get_match(_cmd_line_feature);
	if(_tparser.test_char('[')==zs_ok)
	{
		status=_tparser.test_any_identifier();
		_tparser.get_match(_cmd_line_feature_index);
		if(_tparser.test_char(']'))
		{
			return Z_ERROR_MSG(zs_parse_error,"Expecting ']'");
		}
	}


	return zs_ok;
}

z_status z_console::select_obj()
{
	zf_feature *f;
	f=_temp._fact->get_feature(_cmd_line_feature);
	if(!f)
		return zs_feature_not_found;
	int index=-1;
	/*
	if(_cmd_line_feature_index)
		index=_cmd_line_feature_index.GetDecVal();
		*/


	return f->get_zf_obj(_temp,_cmd_line_feature_index,_temp);

}
z_status z_console:: EvaluateLine(ctext text)
{
	z_status status=zs_ok;	
	_cmd_line_feature.clear();
	_cmd_line_feature_index.clear();
	_tparser.set_source(text);
 	_has_path=false;

	_temp= _selected;
	_temp_path=_path;
	if(_tparser.test_char('/')==zs_matched)
	{
		_temp=_root;
		_temp_path="";
		_has_path=true;
	}
	while (1)
	{
		status=get_feature_and_index();
		if(status)
			break;



		status=select_obj();
		if(status)
		{
			break; //if it is not an object, thats ok
		}
		_has_path=true;

		_temp_path<<"/"<<_cmd_line_feature;
		if(_cmd_line_feature_index)
			_temp_path<<'['<<_cmd_line_feature_index<<']';
		if(_tparser.test_char('/') && _tparser.test_char('.'))
				break;	
		_cmd_line_feature.clear();
		_cmd_line_feature_index.clear();
	}
	if(!_tparser.eob())
		return Z_ERROR_DBG(zs_unparsed_data);
		
	return status;
}

z_status z_console::evaluate_feature(zf_obj& o)
{
	zf_feature *zff;
	zff=o._fact->get_feature(_cmd_line_feature);
	if(!zff)
		return zs_feature_not_found;
	int index=-1;
	if(_cmd_line_feature_index)
		index=_cmd_line_feature_index.GetDecVal();



	return zff->evaluate(_tparser,o);

	return zs_ok;//???
}


void z_console:: OnTab()
{
	if(!_tab_mode)
	{
		//This is just to find the target object.
		z_status status=EvaluateLine(_buffer);
		Z_ERROR_DBG(status);
		if(zs_unparsed_data==status)
		{
			//if we could not parse the line, then dont try to auto-tab
			return;
		}
		if(status!=zs_feature_not_found)
		{


			return;
			/*
			gz_out <<"\nBad path\n";
			
			gz_logger.dump();
				put_prompt();
			return ;
			*/
			
		}


		_auto_tab.clear();		
		_temp._fact->get_list_features(_auto_tab,_temp._obj);
		if(!_has_path)
		{
			_self._fact->get_list_features(_auto_tab,_self._obj);

		}

		_tab_mode_line_index=get_line_length();


		if(_cmd_line_feature)
		{
			_tab_mode_line_index-=_cmd_line_feature.size();
			size_t i=0;
			while(i<_auto_tab.size())
			{
				if(_auto_tab[i].compare(0,_cmd_line_feature.size(),_cmd_line_feature))
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
z_status z_console:: ExecuteLine(ctext text)
{
	z_status status=EvaluateLine(text);
	if((status==zs_ok)||(status==zs_eof)) //just a path change
	{
		_path=_temp_path;
		_selected=_temp;
		return zs_ok;
	}
	//if no path is specified, then try the built in commands
	status=evaluate_feature(_self);
	if(status==zs_ok)
		return 	zs_ok;
	status=evaluate_feature(_temp);
	//Z_ERROR_MSG(status,"\"%s\" not a feature of \"%s\"",_cmd_line_feature.c_str(),_path.c_str());

	return status;
}


#define INDENT "  "
z_status z_console::list_features()
{
	z_map_iter iter;
	zf_feature* p_feature;

	zf_feature_list list;

	gz_out.indent_inc();
	_temp._fact->get_map_features(list,zf_ft_var);
	_temp._fact->get_map_features(list,zf_ft_obj_list);
	gz_out << "\nVariables:\n";
	while(p_feature=list.get_next(iter))
	{
		p_feature->display(	gz_out,	_temp._obj);
	}
	iter.reset();
	list.clear();
	_temp._fact->get_map_features(list,zf_ft_act);
	gz_out << "\nActions:\n";
	while(p_feature=list.get_next(iter))
	{
		p_feature->display(	gz_out,	_temp._obj);
	}
	list.clear();
	iter.reset();
	_temp._fact->get_map_features(list,zf_ft_obj);
	gz_out << "\nChild Objects:\n";
	while(p_feature=list.get_next(iter))
	{
		p_feature->display(	gz_out,	_temp._obj);
	}

	gz_out << "\n";

	return zs_ok;
}
z_status z_console::dumpcfg()
{
 	_temp._fact->dump_obj(gz_out,_temp._obj);
	gz_out << "\n";

	return zs_ok;
}
z_status z_console::get_config_file_path(z_string& path)
{
	path=_startup_path;
	path+='/';	
	path+= _config_file;
	return zs_ok;

}

z_status z_console::loadcfg()
{
	z_string config_file_path;
	get_config_file_path(config_file_path);

	z_file f(config_file_path,"rb");
	z_string data_in;
	f.read_all(data_in);
	zp_text_parser parser;

	parser.set_source(data_in,data_in.size());


	z_status status=_root._fact->load_obj_contents(parser,_root._obj);
	if(status!=zs_ok)
	{
		Z_ERROR_MSG(status,"Load config file failed!");
 		z_logger_dump();
		// return status;   - still set the path, just in case it got loaded
	}


	select_obj_from_path(_root,_path);
	_selected=_temp;
	//cfg._obj.get_by_name(

	return status;
}
z_status z_console::savecfg()
{
	z_string config_file_path;
	get_config_file_path(config_file_path);
	z_file f(config_file_path,"wb");
	_root._fact->dump_obj_contents(f,_root._obj);
	return zs_ok;
}
z_status z_console::help()
{
	_temp=_self;
	return list_features();
}
z_status z_console::exit()
{
	gz_out<< "exiting.\n";
	_running=false;
	return zs_ok;
}

/*________________________________________________________________________

z_console_base
________________________________________________________________________*/

z_console_base::z_console_base()
{
	index=0;
	cur_start=0;
	len=0;
	insertmode=true;
	_tab_mode=false;
}
U32 z_console_base::get_index() { return index;}
U32 z_console_base::get_line_length() { return len;}
void z_console_base::AppendChar(char ch)
{
	
	_buffer+=ch;
	gz_out <<ch;
	len++;
	cur_x++;
	index++;
}
void z_console_base::RedrawLine(int blanks)
{
	const char* s;
	s=_buffer;
	s+=index;
	gz_out << s;
	while(blanks--) gz_out << ' ';
	curGotoXY(cur_x,cur_y);
}
void z_console_base::InsertChar(char ch)
{
	cur_x++;
	_buffer.insert(index,1,ch);
	RedrawLine();
	len++;
	index++;
}
void z_console_base::OverwriteChar(char ch)
{
	cur_x++;
	_buffer.replace(index,1,1,ch);
	RedrawLine();
	index++;
}
void z_console_base::hChar(char ch)
{
	if (len==index)
	{
		AppendChar(ch);
		return;
	}
	if(insertmode)  InsertChar(ch);
	else OverwriteChar(ch);
}
void z_console_base::reset_line()
{
	index=0;
	len=0;
	_buffer="";
	GetXY();
	cur_start=cur_x;
	
}
void z_console_base::output(ctext text)
{
	U32 l=(U32)strlen(text);
	gz_out << text;
	len+=l;
	_buffer+=text;
	index+=l;
	cur_x+=l;
}
void z_console_base::clear_line()
{
	curLeft(index);
	while(len--) gz_out << ' ';
	curGotoXY(cur_x,cur_y);
	index=0;
	len=0;
	_buffer="";
}
void z_console_base::trim_line_to(int trim_point)
{
	int amount_to_trim=len-trim_point;
	cur_x=cur_start+trim_point;
	curGotoXY(cur_x,cur_y);
	while(amount_to_trim--) gz_out << ' ';
	curGotoXY(cur_x,cur_y);
	index=trim_point;
	_buffer.erase(trim_point);
	//del(_buffer,trim_point);
	len=trim_point;
}
int z_console_base::run()
{
	terminal_open();
	_running=true;
	reset_line();
	put_prompt();
	
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
			if(_prev_key!=key_tab) _tab_mode=false;
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
				_buffer.erase(index,1);
				//del(_buffer,index,1);
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
				_buffer.erase(index,1);
				//del(_buffer,index,1);
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
	return 0;
}

void z_console_base::put_prompt()
{
	gz_out  << "/"<<_path<<">";

	//_obj_current->get_path(_path);
	reset_line();
}

 /*
z_status z_console_base::parse_line(ctext text)
{
 	z_status status=_parser.parse_obj(&_cmdline,text);
	if(status==	zs_ok)
	{
		return zs_ok;
	}
	else
		_parser.report_error();
	return zs_error;
}
*/
z_status z_console_base::ExecuteLine(ctext text)
{
	/*
	z_status status=parse_line(text);
	if(status)
		return status;
	  */
	z_status status=zs_ok;


	return status;
}
void z_console_base::OnEnter()
{
	int i;
	z_status result;
	gz_out << '\n';
	if(_buffer.size())
	{
		i=_history.find(_buffer);
		if(i!=-1)
		{
			_history.del(i);
		}
		_history<<_buffer;
		_history_index=(U32)_history.size();
	}
	if(_buffer.size())
	{
		//parse_line(_buffer,_zc);
		result=ExecuteLine(_buffer);
		if(result) 
		{
			switch(result)
			{
			case zs_success:
			case zs_error:
				break;
			default:
				break;
			}
			z_logger_dump();
			gz_out << "\ncommand failed.\n";

		}

	}
	put_prompt();

}

void z_console_base::inc_history(int i)
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
void z_console_base::OnUp()
{
	inc_history(-1);
};
void z_console_base::OnDown()
{
	inc_history(1);

};
void z_console_base::OnTab()
{
	gz_out << "\nTab.\n";


}



void z_console_base::OnDoubleBack()
{
	if(	_path.size())
		_path.pop_back();
	gz_out << "\n";
	put_prompt();


}

/* 
should be a static factory, but... whatever

#define ZO_OBJ_LIST \
	ZCLS(z_console,none,"","", VAR(_log_level) )




#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(logger);
*/