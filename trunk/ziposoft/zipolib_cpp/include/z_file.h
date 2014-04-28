//________________________________________________________________________/////////////////////////
//
// z_streams header
//
//________________________________________________________________________/////////////////////////
#ifndef z_file_header
#define z_file_header
#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_cpp/include/z_string.h"
#include "zipolib_c/include/z_temp_buff.h"
#include "zipolib_cpp/include/z_type_converter.h"
#ifndef WIN32
#define	HANDLE size_t
#endif

void z_debug_load_save_args(int* pargc, char*** pargv);


class z_file
{
	size_t  _file_handle;	
	z_string _file_name;
	void* _log_file_handle;
protected:
	int _max_line_length;
public:
	static const int 
		flag_write=0x001,
		flag_read=0x002,
		flag_append=0x004,
		flag_share_read=0x008, 
		flag_overwrite=0x010,	
		flag_create_new=0x020,	
		flag_open_always=0x040,	
		flag_delete_on_close=0x080,	
		flag_temporary=0x10000;	
	
    z_file();
    z_file(size_t file_handle);
    z_file(ctext filename);
    z_file(ctext filename,ctext mode);
    virtual ~z_file();

	int open(ctext filename,ctext mode);
	void close();
	void flush();
	//HANDLE  _file_handle_win32;

	//operations
	int delete_file();

	//open/close
	bool is_open() { return _file_handle!=0; }
	bool rewind();
	bool get_file_size(size_t &size);
    
	virtual int  write(const char* buf, size_t count);

	//input
	char get(char& c);
	size_t getline(char* buff,size_t size);
	size_t read(char* buff,size_t size);
	const char*  getline(z_string & str);
	bool read_all(z_string & str);
	bool read_all(char*& data,size_t& size);
	template <class TYPE>  z_file  &put(TYPE data)
	{ 
		char* tb=(char*)z_temp_buffer_get(0x100);
		
		z_convert(data,tb,0x100);
		write(tb,strlen(tb));
		z_temp_buffer_release(tb);
		return *this;
	}	


	void eol();
	//virtual z_file  &operator <<  (cset d) { return *this; }
	virtual z_file  &operator <<  (double x) { return put(x); }
    virtual z_file  &operator <<  (const char x)
	{ 
		write(&x,1);
		return *this;
	}
    virtual z_file  &operator <<  (const char* x)
	{ 
		write(x,strlen(x));
		return *this;
	}
    virtual z_file  &operator <<  (const z_string &x)
	{ 
		write(x.c_str(),x.length());
		return *this;
	}
    virtual z_file  &operator <<  (int x){ return put(x); }
    virtual z_file  &operator <<  (size_t x){ return put(x); }

    virtual int putf(const char*  lpszFormat,  ...  );

	//duplicate log file
	virtual void start_log_to_file(ctext filename);
	virtual void stop_log_to_file();
};
class z_file_string_buffer: public z_file
{
	z_string _buffer;
public:
	z_file_string_buffer();
    virtual ~z_file_string_buffer();
	virtual int  write(const char* buf, size_t count);
	z_string &get_buffer() { return _buffer;}
};

//ctext GetSourceFileName(ctext fullpath);
class z_debug: public z_file
{
public:
	z_debug();
    virtual ~z_debug();
	int _depth;
	virtual int  write(const char* buf, size_t count);
	virtual int  enter(ctext file,ctext func,int line);
	virtual int  exit(ctext file,ctext func,int line);
	virtual int  trace(ctext text,ctext file,ctext func,int line);
    virtual int putf(const char*  lpszFormat,  ...  );
    virtual int putfline(const char*  lpszFormat,  ...  );
};


class z_parse_csv
{
public:
	z_parse_csv();
	virtual ~z_parse_csv();
    int _column_idx;    
    int _row_idx; 	
    int _num_columns; 	
	size_t _buffSize;
	ctext _buff;
	size_t _i;
	z_string _value;

	virtual bool ParseBuffer(ctext buff,size_t size);
	virtual bool ParseLine();
	virtual bool ParseValue();

	bool _bInsideString;
	bool _bLineEnd;
	bool _bBufferEnd;
	bool Inc();
	virtual bool NewRowCallback()
	{
		//z_printf("\n");
		return true;
	}
	virtual bool EndRowCallback()
	{
		//z_printf("\n");
		return true;
	}
	virtual bool NewValueCallback(const z_string & value)
	{
		//z_printf("%s ",value.c_str());
		return true;
	}
};
z_status z_csv_encode_string(ctext input,z_string& output);


extern z_debug gz_debug;

extern z_file gz_in;
extern z_file gz_out;

#endif

