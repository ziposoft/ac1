/*________________________________________________________________________

 z_trace_h

________________________________________________________________________*/


#ifndef z_trace_h
#define z_trace_h

#include "zipolib/include/zipolib_cpp.h"


class zt_src_file;

void z_trace_enable();

enum ztp_setting{
		ztp_not_set,
		ztp_off,
		ztp_on,
		ztp_not_found,
		ztp_inherit
};	
ztp_setting get_setting_from_text(ctext t);
ctext get_text_from_setting(ztp_setting s);

class zt_profile_list 
{
public:
	zt_profile_list() { 
	}

	//z_str_map _on_list;
	//zo_str_map _off_list;
	ztp_setting get_setting(ctext prf);

};
class zt_func
{
	z_string _func_name;
	z_string _default;
public:
	ztp_setting _setting;
	ctext get_map_key();
	zt_profile_list _profiles;

	int _line_number;
	U32 _count;
    U64 _total_time;
	zt_src_file* _file;
	zt_func();
	zt_func(ctext name,ztp_setting setting);
	virtual ~zt_func(){}
	double get_d_total_time();
	void dump(int level);
	void init(ctext profile);
};


class zt_profile 
{
public:
	zt_profile() { 
	}
	zt_profile(ctext name,bool def) { 
		_name=name;
		if(def)
			_default="on";
		else
			_default="off";;
	}
	z_string _name;
	z_string _default;
	virtual ctext get_map_key() { return _name.c_str(); }
	virtual ztp_setting get_setting() { return get_setting_from_text(_default); }
};
class zt_func_call
{
public:
	zt_func* _function;
	zt_func_call* _parent;
	U32 _count;
	ctext get_map_key();
    z_time _start_time;
    U64 _total_time;
	z_stl_map<zt_func*,zt_func_call*> _funcs_calls;
	zt_func_call*	get_or_add_func_call(zt_func*);

	zt_func_call(zt_func* fp,zt_func_call* parent);
	zt_func_call();
	virtual ~zt_func_call(){}
	double get_d_total_time();

	void dump_(int level);
};
class zt_src_file
{
	z_string _default;
	z_string _name;
	z_string _full_name;
public:
	zt_src_file();
	z_stl_obj_vector<zt_func> _funcs_v;
	zt_profile_list _profiles;
	zt_src_file(ctext name,ztp_setting setting);
    virtual ~zt_src_file(){}
	ztp_setting _setting;
	ctext get_full_name();
	ctext get_short_name();
	virtual ctext get_map_key() { return _full_name;}

	void init(ctext profile);


	z_map<zt_func> _funcs;
	zt_func*	get_or_add_func(ctext func_name);
};




class zt_module 
{
public:
	zt_module() { 
	}
	zt_module(ctext name,ztp_setting setting) { 
		_name=name;
		_setting=setting;
		_default=get_text_from_setting(setting);
	}	
	ztp_setting _setting;
	z_string _name;//set in config file
	z_string _default;//set in config file
	z_map<zt_src_file> _src_files;
	zt_profile_list _profiles;
	zt_src_file* get_or_add_file(ctext file_name);

	void init(ctext prof);

	virtual ctext get_map_key() { return _name.c_str(); }
};
class z_trace : public zt_func_call
{
	bool _b_tracing;
	bool _b_profiling;
	bool _b_outputing;
	bool _save_cfg_file;
	
	z_file *_output_file;
	zt_profile* _p_current_profile;
public:
	z_trace();
	~z_trace();
	//zp_obj_parser _parser;
	bool _output_os_debug;

	bool enabled;
	z_string _str_profile;
	static const int _buff_size=0x1000;
	int _depth;
	char* _buffer;


	bool is_tracing() { return _b_tracing; }
	bool is_profiling() { return _b_profiling; }
	bool is_outputing() { return _b_outputing; }
	void enable_output() { _b_tracing=true; _b_outputing=true;}
	void enable_profiling() { _b_tracing=true; _b_profiling=true;}
	zt_module* get_or_add_module(ctext module_name);

	int  write(const char* buf, size_t count);
	zt_func_call*  in(ctext module_name,ctext file,ctext func,int line,bool output);
	int  out(ctext file,ctext func,int line);
	void  dump_profile();
	void  save_config(ctext file_name);
	bool  load_config(ctext file_name);
	
	void  err(bool output);
	void  out(bool output);
	void init();
	void init_modules();
	ztp_setting _current_profile_default_set;
	bool  lookup(
		ctext module,
		ctext file,
		ctext func,
		int line,
		//zt_src_file* &pfile,
		zt_func** pfunc
		
		);
	ztp_setting  _check(zt_src_file *pfile,zt_func *pfunc);

    int putf(const char*  lpszFormat,  ...  );
    int putfline(const char*  lpszFormat,  ...  );
    int putfline_vargs(const char*  lpszFormat,  va_list _ArgList  );

	//z_map<zt_func> _funcs;



	z_map_obj<zt_profile> _profiles;
	z_map_obj<zt_module> _modules;


	void set_output_to_stdout();
	zt_func_call* _current_func_call;
	#ifdef BUILD_VSTUDIO
	static LARGE_INTEGER _win32_freq;
	#endif
};

const bool off=false;
const bool on=true;
class z_trace_func_tracer
{
	bool _output;
public:
	z_trace_func_tracer(ctext module,ctext file,ctext func,int line);
	z_trace_func_tracer(ctext module,ctext file,ctext func,int line,const char*  lpszFormat,  ...);
    ~z_trace_func_tracer();
	zt_func_call* _z_trace_func;

};
extern z_trace g_z_trace;



#if Z_TRACE
#ifndef Z_TRACE_MODULE
	#define Z_TRACE_MODULE general
	//#pragma message( "Warning  Z_MODULE not defined"  ) 
#endif
#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
const ctext module_name=STRINGIZE(Z_TRACE_MODULE);
#define ZT_ENABLE() z_trace_enable();


#define ZTF z_trace_func_tracer ZFT_OBJ( module_name,__FILE__,__FUNCTION__,__LINE__) ;
#define	ZTFP(...)  z_trace_func_tracer ZFT_OBJ( module_name,__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__) ;
//#define	ZT(_X_)  {if(g_z_trace.enabled&&g_z_trace.lookup(module_name,__FILE__,__FUNCTION__,__LINE__,0)) g_z_trace.putfline _X_;}
#define	ZT_AT(_X_)  { if(g_z_trace.enabled) g_z_trace.at( module_name,__FILE__,__FUNCTION__,__LINE__); }
#define	ZT_IN(_X_)  { if(g_z_trace.enabled) g_z_trace.in(module_name,__FILE__,__FUNCTION__,__LINE__); }
#define	ZT_OUT(_X_)  { if(g_z_trace.enabled) g_z_trace.out( module_name,__FILE__,__FUNCTION__,__LINE__); }
/*
#define	ZTF  
#define	ZFTO  
#define	ZTFP(...)  
#define	ZT(_X_)  
#define	ZTL(_X_)  
#define	ZT_AT(_X_)  
#define	ZT_OUT(_X_) 
*/

#else
#define ZT_ENABLE() 

#define	ZTF  
#define	ZFTO  
#define	ZTFP(...)  
//#define	ZT(_X_)  
#define	ZTL(_X_)  
#define	ZT_AT(_X_)  
#define	ZT_OUT(_X_)  


#endif
#endif

