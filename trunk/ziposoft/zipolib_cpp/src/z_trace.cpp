/*________________________________________________________________________

	 z_trace.cpp

________________________________________________________________________*/
#include "zipolib_cpp_pch.h"
#include "z_trace.h"
#include "z_string.h"
#include "z_file.h"
#include "zipolib_c/include/z_time.h"


int vsnprintf(char *str, size_t size, ctext format, va_list ap);

/* GLOBALS */
z_string g_file_data;

/*

const z_obj_fact* trace_obj_list[]=
{
	ZP_LISTOBJ(zp_pair),
	ZP_LISTOBJ(zt_func),
	ZP_LISTOBJ(zt_profile),
	ZP_LISTOBJ(zt_src_file),
	ZP_LISTOBJ(z_trace),
	ZP_LISTOBJ(zt_profile_list),
	ZP_LISTOBJ(zt_module),


};
*/


ztp_setting get_setting_from_text(ctext t)
{
	if(strcmp(t,"on")==0) return ztp_on;	
	if(strcmp(t,"off")==0) return ztp_off;	
	return ztp_inherit;	
}
ctext get_text_from_setting(ztp_setting s)
{
	switch(s)
	{
	case ztp_off:
		return "off";
	case ztp_on:
		return "on";
	case ztp_inherit:
		return "";
	default:
		break;
	}
	return "invalid";
}

/*________________________________________________________________________
 
						zt_profile 
________________________________________________________________________*/

/*________________________________________________________________________
 
						zt_profile_list 
________________________________________________________________________*/

ztp_setting zt_profile_list::get_setting(ctext prf)
{
	/*
	if(_on_list.exists(prf))
		return ztp_on;
	if(_off_list.exists(prf))
		return ztp_off;
	return ztp_inherit;
	*/

	return ztp_on;


}


//____________________________________________________________________
//
// z_trace_func_tracer 
//
//____________________________________________________________________
z_trace_func_tracer::z_trace_func_tracer(ctext module,ctext file,ctext func,int line)
{
	
	if(g_z_trace.enabled && g_z_trace.is_tracing())
		_z_trace_func=g_z_trace.in(module,file,func,line,true);
	else
		_z_trace_func=0;
}
z_trace_func_tracer::z_trace_func_tracer(ctext module,ctext file,ctext func,int line,const char*  lpszFormat,  ...)
{
	
	if(g_z_trace.enabled && g_z_trace.is_tracing())
	{
		_z_trace_func=g_z_trace.in(module,file,func,line,true);
		if(_z_trace_func)
		{
			va_list ArgList;
			va_start (ArgList, lpszFormat);
			g_z_trace.putfline_vargs(lpszFormat,ArgList);
			va_end (ArgList);
		}

	}
	else
		_z_trace_func=0;
}
z_trace_func_tracer::~z_trace_func_tracer()
{
	if(_z_trace_func)
		g_z_trace.out(_output);

}


//____________________________________________________________________
//
// zt_func_call 
//
//____________________________________________________________________

zt_func_call::zt_func_call()
{
	_function=0;
	_parent=0;

}
zt_func_call::zt_func_call(zt_func* fp,zt_func_call* parent)
{
	_function=fp;
	_parent=parent;

}
double zt_func_call::get_d_total_time()
{
	double d_total_time;
	d_total_time=(double)_total_time;
#if WIN32
	d_total_time=d_total_time*1000;
	d_total_time=d_total_time/g_z_trace._win32_freq.QuadPart;
#else
	d_total_time=d_total_time/1000;

#endif
	return d_total_time;
}
ctext zt_func_call::get_name()
{

	return _function->get_name();
}

zt_func_call*	zt_func_call::get_or_add_func_call(zt_func* fp)
{
	zt_func_call* fc=0;
	if(_funcs_calls.get(fp,fc)==false)
	{
		fc=new zt_func_call(fp,this);
		_funcs_calls[fp]=fc;

	}
	return fc;
}
void  zt_func_call::dump_(int level)
{
	double d_total_time;
	double d_avg_time;
	d_total_time=get_d_total_time();
	d_avg_time=d_total_time/_count;
/*	if(!_file) 
		return;
	ctext file_name=z_get_filename_from_path(_file->_full_name);
	z_string file_line;
	file_line.Format("%s(%d)",file_name,_line_number);

	z_string name;
	int l=level;
	while(l--) name+=' ';
	name+=_func_name;
	g_z_trace.putf("%3.0f %5d %5.2lf %5.2lf %-38.38s %-33s\n",
		d_total_time/g_z_trace.get_d_total_time()*100,
		_count,
		d_avg_time,
		d_total_time,		
		name.c_str(),
		file_line.c_str()

		);
	
	z_string key;
	zt_func p;
	level++;

	z_stl_map<z_string,zt_func>::iter i=_funcs.begin();
	while(_funcs.get_next(i,key,p))
	{
		p.dump(level);
	}
	level--;
	*/
}

//____________________________________________________________________
//
// zt_func 
//
//____________________________________________________________________

zt_func::zt_func()
{
	_count=0;
	_line_number=0;
	_total_time=0;
	_setting=ztp_inherit;

}
zt_func::zt_func(ctext name,ztp_setting setting)
{
	_count=0;
	_func_name=name;
	_line_number=0;
//	_start_time=0;
	_total_time=0;
	_setting=setting;
}

double zt_func::get_d_total_time()
{
	double d_total_time;
	d_total_time=(double)_total_time;
#if WIN32
	d_total_time=d_total_time*1000;
	d_total_time=d_total_time/g_z_trace._win32_freq.QuadPart;
#else
	d_total_time=d_total_time/1000;

#endif
	return d_total_time;
}

ctext zt_func::get_name()
{

	return _func_name;
}
void zt_func::init(ctext prof)
{
	_setting=_profiles.get_setting(prof);
	if(_setting==ztp_inherit)
		_setting=get_setting_from_text(_default);
	ZT(("[func] %s = %s\n",_func_name.c_str(),get_text_from_setting(_setting)));
	
}


void  zt_func::dump(int level)
{
	double d_total_time;
	double d_avg_time;
	d_total_time=get_d_total_time();
	d_avg_time=d_total_time/_count;
	if(!_file) 
		return;
	ctext file_name=z_get_filename_from_path(_file->get_full_name());
	z_string file_line;
	file_line.Format("%s(%d)",file_name,_line_number);
/*
	z_string name;
	int l=level;
	while(l--) name+=' ';
	name+=_func_name;
	g_z_trace.putf("%3.0f %5d %5.2lf %5.2lf %-38.38s %-33s\n",
		d_total_time/g_z_trace.get_d_total_time()*100,
		_count,
		d_avg_time,
		d_total_time,		
		name.c_str(),
		file_line.c_str()

		);
	
	z_string key;
	zt_func p;
	level++;

	z_stl_map<z_string,zt_func>::iter i=_funcs.begin();
	while(_funcs.get_next(i,key,p))
	{
		p.dump(level);
	}
	level--;
	*/
}
//____________________________________________________________________
//
// zt_src_file 
//
//____________________________________________________________________


zt_src_file::zt_src_file()
{
	_setting=ztp_inherit;
}
zt_src_file::zt_src_file(ctext full_name,ztp_setting setting)
{
	_full_name=full_name;
	_setting=setting;
}
ctext zt_src_file::get_full_name()
{
	return _full_name;
}

ctext zt_src_file::get_short_name()
{
	return _name;
}


zt_func*	zt_src_file::get_or_add_func(ctext func_name)
{
	zt_func* f=_funcs.get(func_name);
	if(!f) 
	{
		f=new zt_func(func_name,ztp_inherit);
		_funcs.add(func_name,f);
	}
	return f;
}

void zt_src_file::init(ctext prof)
{
	_setting=_profiles.get_setting(prof);
	if(_setting==ztp_inherit)
		_setting=get_setting_from_text(_default);
	ZT(("[src file] %s = %s\n",_name.c_str(),get_text_from_setting(_setting)));
	z_map_iter iter=zmi_null;
	zt_func* p_f=0;
	while((p_f=_funcs.get_next(iter)))
	{
		p_f->init(prof);
	}
}
//____________________________________________________________________
//
// zt_module 
//
//____________________________________________________________________

zt_src_file* zt_module::get_or_add_file(ctext file_name)
{
	zt_src_file* f=_src_files.get(file_name);
	if(!f) 
	{
		f=new zt_src_file(file_name,ztp_inherit);
		_src_files.add(file_name,f);
	}
	return f;
}
void zt_module::init(ctext prof)
{
	_setting=_profiles.get_setting(prof);
	if(_setting==ztp_inherit)
		_setting=get_setting_from_text(_default);
	z_map_iter iter=zmi_null;
	ZT(("[module] %s = %s\n",_name.c_str(),get_text_from_setting(_setting)));
	zt_src_file* p_sf=0;
	while((p_sf=_src_files.get_next(iter)))
	{
		p_sf->init(prof);
	}
}
//____________________________________________________________________
//
// z_trace -
//
//____________________________________________________________________




z_trace::~z_trace()
{
	if(enabled)
	{
		dump_profile();
		save_config("trace.cfg");
	}
		//dump_config();
}
z_trace::z_trace()
{
	enabled=false;
	_save_cfg_file=false;
	_b_tracing=false;
	_b_profiling=false;
	_b_outputing=false;
	_depth=0;
	_output_os_debug=true;
	_buffer= new char[_buff_size];

	_current_func_call=this;
	_output_file=0;
	_current_profile_default_set=ztp_off;

	#ifdef WIN32
		QueryPerformanceFrequency(&_win32_freq);
	#endif


}

/*-----------------------------------------------------------------------
	z_trace   LOAD/SAVE/INIT 
-----------------------------------------------------------------------*/
void  z_trace::init_modules()
{
	z_map_iter iter=zmi_null;
	zt_module* p_mod=0;
	while((p_mod=_modules.get_next(iter)))
	{
		p_mod->init(_str_profile);
	}
}
void  z_trace::init()
{
	//OutputDebugString("load_config\n");

	load_config("trace.cfg");
	//OutputDebugString("load_config done\n");

	if(_profiles.size()==0)
	{
		_p_current_profile=new zt_profile("all_on",true);
		_profiles<< _p_current_profile
			<< new zt_profile("all_off",false);
		_str_profile="all_on";
	}
	if(!_str_profile)
		_str_profile="all_on";

	_p_current_profile=_profiles.get(_str_profile);
	if(!_p_current_profile)
	{
		z_map_iter iter=zmi_null;
		_p_current_profile=_profiles.get_next(iter);
	}
	_current_profile_default_set=_p_current_profile->get_setting();
	_b_tracing=true;;
	_b_profiling=true;;
	_b_outputing=true;;
	init_modules();
	enabled=true;;
}


bool  z_trace::load_config(ctext file_name)
{
	/*
	z_status status;
	bool result=false;
	z_file file(file_name,"rb");
	if(file.read_all(g_file_data))
	{
		status=_parser.parse_obj(this,g_file_data);
		if(status!=zs_matched)
		{
			_parser.report_error(status);
		}
		else
		{
			result=true;
			_save_cfg_file=true;
		}
	}
	else
		_save_cfg_file=true;
		*/
	return true;
}

void  z_trace::save_config(ctext file_name)
{
	/*
	if(!_save_cfg_file)
		return;
	enabled=false;
	z_status status;
	if(_p_current_profile)
		_str_profile=_p_current_profile->get_name();
	//Z_ASSERT(0); //TODO
	//_parser.set_obj_table(PARSE_TABLE(trace_obj_list));
	z_file file_cfg(file_name,"wb");

	status=_parser.output_obj(&file_cfg,this);
	if(status!=zs_matched)
	{
		_parser.report_error(status);
	}
	*/
}
/*-----------------------------------------------------------------------
	z_trace   entry calls
-----------------------------------------------------------------------*/

zt_func_call*  z_trace::in(ctext module,ctext full_file_name,ctext func_name,int line,bool ouput)
{
	//zt_module* pmodule;
	//zt_src_file* pfile;
	zt_func* pfunc;
	if(lookup(module,full_file_name,func_name,0,&pfunc)==0)
	{
		return 0;
	}
	
	zt_func_call* parent_fc=_current_func_call;
	if(parent_fc)
	{
		_current_func_call=parent_fc->get_or_add_func_call(pfunc);
	}
	else
	{
		_current_func_call=this;
	}
	zt_func_call& fc=*_current_func_call;
/*
	if(!fc->_count)
	{
		p._parent=parent;
		//p._file_name=fullpath;
		p._func_name=func;
		p._line_number=line;
		p._file=&file;
		file._funcs[func]=_current_function;
	}
	*/
	fc._count++;
	z_set_start_time(&fc._start_time);

	if(ouput)
		putfline("> %s",fc.get_name());
	_depth++;
	return _current_func_call;

}

/*-----------------------------------------------------------------------
	z_trace   checking
-----------------------------------------------------------------------*/
ztp_setting  z_trace::_check(zt_src_file *pfile,zt_func *pfunc)
{
	ztp_setting setting=ztp_not_found;
	setting=pfunc->_setting;
	if(setting!=ztp_inherit)
		return setting;
	setting=pfile->_setting;
	if(setting!=ztp_inherit)
		return setting;
	return _current_profile_default_set;
}

bool  z_trace::lookup(
		ctext module,
		ctext full_file_name,
		ctext func_name,
		int line,
		//zt_src_file* &pfile_out,
		zt_func** pfunc_out)
{
	if(!is_tracing())
		return false;
	ctext short_name=z_get_filename_from_path(full_file_name);
	zt_module* pmod=get_or_add_module(module);
	zt_src_file* pfile=pmod->get_or_add_file(short_name);
	zt_func* pfunc=pfile->get_or_add_func(func_name);
	ztp_setting set=_current_profile_default_set;
	if(pfunc->_setting!=ztp_inherit) 
		set= pfunc->_setting;
	else
	{
		if(pfile->_setting!=ztp_inherit)
			set= pfile->_setting;
		else
		{
			if(pmod->_setting!=ztp_inherit)
				set= pmod->_setting;
		}
	}
	
	//ztp_setting set= _check(pfile,pfunc);
	//if(pfile_out) pfile_out=pfile;
	if(pfunc_out) *pfunc_out=pfunc;
	return set==ztp_on;
}
zt_module* z_trace::get_or_add_module(ctext module_name)
{
	zt_module* m=_modules.get(module_name);
	if(!m) 
	{
		m=new zt_module(module_name,ztp_inherit);
		_modules.add(module_name,m);
	}
	return m;
}


/*-----------------------------------------------------------------------
	z_trace   Profiling stuff
-----------------------------------------------------------------------*/
#ifdef WIN32
LARGE_INTEGER z_trace::_win32_freq={0,0};
#endif

//void  z_trace::err(ctext module,ctext full_file_name,ctext func_name,int line,bool ouput)
void  z_trace::err(bool output)
{
	zt_func_call& fc=*_current_func_call;//profile_list[func];
	Z_ASSERT(fc._count);
    U64 elapsed_time;
	elapsed_time=z_get_elapsed_time_us_64(&fc._start_time);
	fc._total_time+=elapsed_time;
	_depth--;
	if(output)
	{
		//putfline("< %s",fc.get_name());
		putfline("<");
	}
	_current_func_call=fc._parent;
}
/*-----------------------------------------------------------------------
	z_trace   Outputing
-----------------------------------------------------------------------*/
void  z_trace::out(bool output)
{
	zt_func_call& fc=*_current_func_call;//profile_list[func];
	Z_ASSERT(fc._count);
    U64 elapsed_time;
	elapsed_time=z_get_elapsed_time_us_64(&fc._start_time);
	fc._total_time+=elapsed_time;
	_depth--;
	if(output)
	{
		//putfline("< %s",fc.get_name());
		putfline("<");
	}
	_current_func_call=fc._parent;
}
void  z_trace::dump_profile()
{
	g_z_trace.putf("\n%%   COUNT   AVG TOTAL FUNC                                   FILE(LINE#) \n");
//                  100     2     1     2 main                                   zipobase_test.cpp(29)            
}

int z_trace::write(const char* buf, size_t count )
{

	z_debug_out(buf);

	if(_output_file)
		_output_file->write(buf,count);

	return 0;
}



int z_trace::putfline_vargs(const char*  lpszFormat,  va_list ArgList  )
{
    int c;
    c=vsnprintf (_buffer,_buff_size, lpszFormat, ArgList);
	int d=_depth;
	while(d--)
	{
		write("  ",2);
	}
    write(_buffer,strlen(_buffer));
	write("\n",1);

    return c;
}
int z_trace::putfline(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ArgList;
    va_start (ArgList, lpszFormat);
    c=putfline_vargs(lpszFormat,ArgList);
    va_end (ArgList);
    return c;
}

int z_trace::putf(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ap;
    va_start (ap, lpszFormat);
    c=vsnprintf (_buffer,_buff_size, lpszFormat, ap);
    va_end (ap);
    write(_buffer,strlen(_buffer));
    return c;
}
void z_trace::set_output_to_stdout()
{
	_output_file=&gz_out;
}
z_trace g_z_trace;


void z_trace_enable()
{
	g_z_trace.init();


}

#if 0
#define Z_MODULE _Z_MODULE(trace)
#define ZO_OBJ_LIST \
OBJ(zt_module,zp_obj,"zt_module","",\
	   "#'\t':{_name}ident:'<':?{_default}ident:'>':{_profiles}prof_list:#'\n\t':"\
	   "'{':#'\n':*{_src_files}zt_file:#'\t':'}':#'\n'" ,VAR(_name) VAR(_default) VAR(_profiles) VAR(_src_files)) \
OBJ(z_trace,zp_obj,"z_trace","", "'profile':'=':{_str_profile}ident:#'\n':"\
	   "'profiles':#'\n':'{':#'\n':*{_profiles}zt_profile:'}':#'\n':"\
	   "'modules':#'\n':'{':#'\n':*{_modules}zt_module:'}'"\
	  , VAR(_profiles) VAR(_modules) VAR(_str_profile)) \
OBJ(zt_profile_list,zp_obj,"prof_list","","'[':?{_on_list}identlist:':':?{_off_list}identlist:']'" ,VAR(_on_list) VAR(_off_list)) \
OBJ(zt_profile,zp_obj,"zt_profile","",\
	   "#'\t':{_name}ident:'<':#{_default}ident:'>':#'\n'", VAR(_name) VAR(_default)) \
OBJ(zt_func,zp_obj,"zt_func","",\
	   "#'\t\t\t':{_func_name}scoped:'<':?{_default}ident:'>':{_profiles}prof_list:#'\n'",\
	   VAR(_func_name) VAR(_profiles) VAR(_default)) \
OBJ(zt_src_file,zp_obj,"zt_file","",\
	   "#'\t\t':{_full_name}path:'<':#{_default}ident:'>':{_profiles}prof_list:#'\n':"\
	   "#'\t\t':'{':#'\n':*{_funcs}zt_func:#'\t\t':'}':#'\n'",\
	   VAR(_full_name) VAR(_default) VAR(_profiles) VAR(_funcs)) 

#include "zipolib/include/z_obj_macro.h"

#endif