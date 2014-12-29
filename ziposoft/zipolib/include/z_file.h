/*________________________________________________________________________

z_file.h

________________________________________________________________________*/


#ifndef z_file_header
#define z_file_header
#include "zipolib/include/zipo.h"


#include "zipolib/include/z_temp_buff.h"
#include "zipolib/include/z_type_converter.h"



/*________________________________________________________________________

								z_file
________________________________________________________________________*/
class z_file
{
	size_t  _file_handle;	
	z_string _file_name;
	void* _log_file_handle;
	int _indent_depth;
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
	/*______________________________________

				Init
	  ______________________________________*/	
    z_file();
    z_file(size_t file_handle);
    z_file(ctext filename);
    z_file(ctext filename,ctext mode);
    virtual ~z_file();

	void init();

	void set_handle(size_t t);
	/*______________________________________

				Info
	  ______________________________________*/	
	bool get_file_size(size_t &size);
	bool is_open() { return _file_handle!=0; }



	/*______________________________________

				Open/Close
	  ______________________________________*/	
	z_status open(ctext filename,ctext mode);
	void close();
	/*______________________________________

				Reading
	  ______________________________________*/	
	bool rewind();
	char get(char& c);
	size_t getline(char* buff,size_t size);
	z_status  getline(z_string & str);
	size_t read(char* buff,size_t size);
	bool read_all(z_string & str);
	bool read_all(char*& data,size_t& size);


	/*______________________________________

				Writing
	  ______________________________________*/	
	virtual int  write(const char* buf, size_t count);
    virtual int putf(const char*  lpszFormat,  ...  );
	void flush();

 	/*______________________________________

				String Output
	  ______________________________________*/	

	//TODO - This is awful! Fix this
	template <class TYPE>  z_file  &put(TYPE data)
	{ 
		char* tb=(char*)z_temp_buffer_get(0x100);
		z_convert(data,tb,0x100);
		write(tb,strlen(tb));
		z_temp_buffer_release(tb);
		return *this;
	}		
	
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
    virtual z_file  &operator <<  (I64 x){ return put(x); }
    virtual z_file  &operator <<  (I32 x){ return put(x); }
    virtual z_file  &operator <<  (U64 x){ return put(x); }
    virtual z_file  &operator <<  (U32 x){ return put(x); }
	void indent();
	void indent_inc();
	void indent_reset();
	void indent_dec();	
	void eol();
	
	/*______________________________________

				Misc
	  ______________________________________*/	
	z_status delete_file();

	z_status mmap_open(U8** map_ptr_out,bool readonly);
	z_status mmap_close(U8* map_ptr);

	virtual void start_log_to_file(ctext filename);
	virtual void stop_log_to_file();


};
/*________________________________________________________________________

								z_file_string_buffer
________________________________________________________________________*/
class z_file_string_buffer: public z_file
{
	z_string _buffer;
public:
	z_file_string_buffer();
    virtual ~z_file_string_buffer();
	virtual int  write(const char* buf, size_t count);
	z_string &get_buffer() { return _buffer;}
};

/*________________________________________________________________________

								z_debug
________________________________________________________________________*/
class z_debug: public z_file
{
public:
	z_debug();
    virtual ~z_debug();
	virtual int  write(const char* buf, size_t count);
};
/*________________________________________________________________________

								z_parse_csv
________________________________________________________________________*/

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

	virtual bool ParseFile(z_file& file);
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
z_status z_csv_encode_string(z_string& output);

/*________________________________________________________________________

								GLOBALS
________________________________________________________________________*/
z_debug& z_debug_get();
z_file& z_stdout_get();
#define zout z_stdout_get()
z_file& z_stdin_get();


#endif

