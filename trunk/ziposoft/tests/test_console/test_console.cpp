// test_console.cpp : Defines the entry point for the console application.
//

#include "test_console.h"
#include "zipolib/include/z_factory_static.h"

class z_intf_random_access
{
public:
	z_intf_random_access()
	{
		total_size=0;
		offset=0;
		width=1;
		length=0x10;

	}
 	size_t total_size;

	int offset;
	int width;
	int length;
	virtual int read(int offset,int width,U64& data)
	{
		return zs_not_implemented;
	}
	virtual int write(int offset,int width,U64 data)
	{
		return zs_not_implemented;
	}
	virtual int dump()
	{

		int i;
		z_status status;
		for (i = offset; i < (offset+length); i+=width)
		{
			U64 data;
			if( (i%0x10)==0)
				printf("\n%02x : ",i);

			status=read(i,width,data);
			if(status)
				return status;
			switch(width)
			{
			case 1:		
				//TODO support BIG endian
				printf("%02x ",(U8) data);break; 
			case 2:			
				printf("%04x ",(U16) data);break;
			case 4:			
				printf("%04x ",(U32) data);break;
			case 8:			
				printf("%08llx ",(U64) data);break;
			default:
				printf("Invalid width: %d\n",width);
				break;
			}
		}
		printf("\n");
		return 0;
	}
	virtual int write_pattern_incrementing()
	{
		return 0;
	}
	virtual int write_pattern_set()
	{

		return 0;
	}
};

class z_intf_mapped_access: public  z_intf_random_access
{
public:

	z_intf_mapped_access()
	{
		_p_data=0;

	}
	char* _p_data;
	virtual int read(int offset,int width,U64& data)
	{
		U8* p;
		p=(U8*)_p_data+offset;


		switch(width)
		{
		case 1:		
			data=*(U8*)p;
			break;
		case 2:			
			data=*(U16*)p;
			break;
		case 4:			
			data=*(U32*)p;
			break;
		case 8:			
			data=*(U64*)p;
			break;
		default:
			break;
		}
		return 0;
	}
	virtual int write(int offset,int width,U64 data)
	{
		return zs_not_implemented;
	}
};
class z_binary_file	: public  z_intf_mapped_access
{
public:
	z_file _file;

	z_string filename;
	virtual int load()
	{
		z_status status=_file.open(filename,"rb");
		if(status==zs_ok)
		{
			_file.read_all(_p_data,total_size);
			_file.close();
		}
		return status;
	}
	virtual int save()
	{
		z_status status=_file.open(filename,"wb");
		if(status==zs_ok)
		{
			_file.write(_p_data,total_size);
			_file.close();
		}
		return status;
	}


};

ZFACT(z_intf_random_access)
{
	ZACT(dump);
	ZACT(write_pattern_incrementing);
	ZACT(write_pattern_set);

	
	ZPROP(offset);
	ZPROP(width);
	ZPROP(length);
};

ZFACT_V(z_binary_file,z_intf_random_access)
{
	ZPROP(filename);
	ZACT_XP(save,"save","Save to file",1,ZPARAM(filename));
	ZACT_XP(load,"load","Load from file",1,ZPARAM(filename));
	

};




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
	z_binary_file binfile;
	z_strlist x;
	int i;
	z_string s;
	int add() {  x<<s;return 0;}
	int show() { gz_out << '\n'<<s<<'\n';return 0;}


};
ZFACT(root)
{
	ZOBJ(console);
	ZOBJ(binfile);
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