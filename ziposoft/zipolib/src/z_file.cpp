#include "zipolib_cpp_pch.h"
#include "z_file.h"
#include "z_error.h"
#include "z_logger.h"
#include "zipolib/include/z_files.h"
#include "zipolib/include/z_os_specific.h"

using namespace std;
#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif


z_file::z_file()
{
	_file_handle=0;
	_log_file_handle=0;
	_max_line_length=0x1000;
	_indent_depth=0;
}
z_file::z_file(ctext filename)
{
	_file_handle=0;
	_max_line_length=0x1000;
	_log_file_handle=0;
	_file_name=filename;
	_indent_depth=0;
}
z_file::~z_file()
{
	close();

}

z_file::z_file(size_t h)
{
	_file_handle=h;
	_max_line_length=0x1000;
	_log_file_handle=0;
}
z_file::z_file(ctext filename,ctext mode)
{
	_file_name=filename;
	_file_handle=(size_t)(void*)fopen(filename,mode);
	_max_line_length=0x1000;
	_log_file_handle=0;
}
int z_file::open(ctext filename,ctext mode)
{
	_file_name=filename;
	_file_handle=(size_t)(void*)fopen(filename,mode);
	if( _file_handle==0) return -1;
	return 0;
}

void z_file::close()
{
	if(_file_name) //dont close stdin/stdout
		if(_file_handle)
		{
			fclose((FILE*)_file_handle);
			_file_handle=0;

		}
}
bool z_file::get_file_size(size_t &size)//TODO more efficient
{
	if (_file_handle==0) return false;
	::fseek((FILE*)_file_handle,0L,SEEK_END);
	size=::ftell((FILE*)_file_handle);
	::rewind((FILE*)_file_handle);
	return true;
}
bool z_file::read_all(z_string & str)
{
	size_t size;
	if(!get_file_size(size)) 
		return false;
	char* buff=(char*)malloc(size+1);
	read(buff,size);
	buff[size]=0;
	str=buff;

	return true;
}
bool z_file::read_all(char* & data,size_t& size)
{
	if(!get_file_size(size)) 
		return false;
	data=(char*)malloc(size+1);
	read(data,size);
	data[size]=0;
	return true;
}

bool z_file::rewind()
{
	::rewind((FILE*)_file_handle);
	return true;
}



size_t z_file::getline(char* buff,size_t size)
{
	size_t i;
	if (0==buff) return 0;
	fgets( buff, (int)size, (FILE*)_file_handle );
	i=strlen(buff);
	while( (i>0)&&
		   (
		     (buff[i-1]=='\n')||
		     (buff[i-1]=='\r')
		   )
		 )
	{
		buff[i-1]=0;
		i--;
	}
	return i; //TODO performance- better way to know how much we read?
}
size_t z_file::read(char* buff,size_t size)
{
	return fread( buff, 1,size, (FILE*)_file_handle );
}


char z_file::get(char& c)
{
	c=fgetc((FILE*)_file_handle);
	return c;
}

const char* z_file::getline(z_string& s)
{
	char* tb=z_temp_buffer_get(_max_line_length);
	fgets( tb, _max_line_length, (FILE*)_file_handle );
	size_t l=strlen(tb);
	if(tb[l-1]=='\n')
		tb[l-1]=0;

	s=tb;
	z_temp_buffer_release(tb);
	return  s.c_str();
}




void z_file::start_log_to_file(ctext filename)
{
#if 0
	_log_file_handle=(void*)(size_t)_open(filename,_O_CREAT | _O_TRUNC|_O_BINARY );
#else
	_log_file_handle=(void*)fopen(filename,"ab");
#endif
}
void z_file::stop_log_to_file()
{
	if(_log_file_handle)
	fclose((FILE*)_log_file_handle);
	_log_file_handle=0;

}
int z_file::write(const char* buf, size_t count )
{
	if(!_file_handle)
	{
		return -1;
	}
	::fwrite(buf,1,count,(FILE*)_file_handle);	
	if(_log_file_handle)
	{
		::fwrite(buf,1,count,(FILE*)_log_file_handle);	

	}
	return 0;
}
void z_file::eol()
{
#ifdef BUILD_VSTUDIO
	write("\r\n",2);
#else
	write("\n",1);
#endif
}
int z_file::putf(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ap;
	char* tempbuf=(char*)z_temp_buffer_get(_max_line_length);
    va_start (ap, lpszFormat);
    c=vsnprintf (tempbuf,_max_line_length, lpszFormat, ap);
    va_end (ap);
    write(tempbuf,strlen(tempbuf));
	z_temp_buffer_release(tempbuf);
    return c;
}
void z_file::flush()
{
    
#ifdef BUILD_VSTUDIO
    FlushFileBuffers(HANDLE(_file_handle));
#endif
}
void z_file::indent()
{
	int i=_indent_depth;
	while(i--)
		*this<<"  ";
}
void z_file::indent_inc()
{
	_indent_depth++;

}
void z_file::indent_reset()
{
	_indent_depth=0;
}
void z_file::indent_dec()
{
	_indent_depth--;

}
int z_file::delete_file()
{
	close();
	return z_file_delete(_file_name);
}

z_file gz_in((size_t)stdin);
z_file gz_out((size_t)stdout);

//____________________________________________________________________
//
// z_debug -  this is the old zipobase01 debug stuff
//
//____________________________________________________________________

int z_debug::write(const char* buf, size_t count )
{
#ifdef BUILD_VSTUDIO
	WCHAR* wc=WCHAR_str_allocate(buf,count);
	 OutputDebugString(wc);
	 WCHAR_str_deallocate(wc);
#else
	z_file::write(buf,count);
#endif
	return 0;
}
ctext GetSourceFileName(ctext fullpath)
{
	ctext filename=strrchr(fullpath,'\\');
	if(filename) filename++;
	else filename=fullpath;
	return filename;
}
int z_debug::putfline(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ap;
	char* tempbuf=z_temp_buffer_get(_max_line_length);
    va_start (ap, lpszFormat);
    c=vsnprintf (tempbuf,_max_line_length, lpszFormat, ap);
    va_end (ap);
	int d=_depth;
	while(d--)
	{
		write("  ",2);
	}
    write(tempbuf,strlen(tempbuf));
	z_temp_buffer_release(tempbuf);

    return c;
}
int z_debug::putf(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ap;
	char* tempbuf=z_temp_buffer_get(_max_line_length);
    va_start (ap, lpszFormat);
    c=vsnprintf (tempbuf,_max_line_length, lpszFormat, ap);
    va_end (ap);
    write(tempbuf,strlen(tempbuf));
	z_temp_buffer_release(tempbuf);
    return c;
}
int z_debug:: trace(ctext text,ctext fullpath,ctext func,int line)
{
	ctext filename=GetSourceFileName(fullpath);
	putfline("%s %s(%d): %s()\n",text,filename,line,func);
	return 0;
}
int z_debug:: enter(ctext fullpath,ctext func,int line)
{
	trace("->",fullpath,func,line);
	_depth++;
	return 0;
}
int z_debug:: exit(ctext fullpath,ctext func,int line)
{
	_depth--;
	trace("<-",fullpath,func,line);
	return 0;
}
z_debug::z_debug()
{
#ifdef LINUX

	//open("debug.log","w");
#endif

	_depth=0;
}
z_debug:: ~z_debug()
{

}



z_debug gz_debug;



z_file_string_buffer::z_file_string_buffer()
{


}
z_file_string_buffer::~z_file_string_buffer()
{


}
int z_file_string_buffer::write(const char* buf, size_t count )
{
	_buffer.append(buf,count);
	return (int) count;
}

z_status z_csv_encode_string(z_string& output)
{

	size_t dq=0;
	if(output.find_first_of(",\"\n")==z_string::npos)
		return z_status_ok;

	output.insert(0,1,'\"');
	while( (dq=output.find_first_of("\""))!=z_string::npos)
	{
		output.insert(dq,1,'\"');
	}
	output.append(1,'\"');

	return z_status_ok;
	

}


#define LIMIT_COL 10
#define LIMIT_ROW 1000

z_parse_csv::z_parse_csv()
{
	_column_idx=0;
	_row_idx=0;
	_buff=0;
	_i=0;
	_buffSize=0;
	_num_columns=0;
}
z_parse_csv::~z_parse_csv()
{
}
bool z_parse_csv::Inc()
{
	if (_bBufferEnd) return true;
	_i++;
	if (_i==_buffSize) _bBufferEnd=true;
	return _bBufferEnd;
}

bool z_parse_csv::ParseLine()
{
	_column_idx=0;
	_bLineEnd=false;

	
	while(_bLineEnd==false) 
	{
		if(!ParseValue()) 
			return false;
		if(_bLineEnd&&(_column_idx==0)&&(_value==""))
		{
			//If it is a blank svg:line, skip it.
			_bLineEnd=false;
			continue;
		}
		if(!NewValueCallback(_value)) return false;
		_column_idx++;
		if (_bBufferEnd) break;
	}
	return true;

}
//TODO this is terrible rewrite
bool z_parse_csv::ParseValue()
{
//	bool bValueEnd=false;
	_value="";
	_bInsideString=false;

	while(_i<_buffSize)
	{
		if(_bBufferEnd) return true;
		U8 c=_buff[_i];
		U8 c2=0;
		if((_i+1)<_buffSize)
			c2=_buff[_i+1];
		Inc();
		if (_bInsideString)
		{
			if (c=='\"')
			{				
				if (c2=='\"') 
					Inc();    
				else
				{				
					_bInsideString=false;
					continue;
				}
			}
		}
		else
		{
			if (c=='\"')
			{
				_bInsideString=true;
				continue;
			}
			if (c==',') return true;

			if  (  
				((c==0xD)&&(c2!=0xA)) //Crapintosh
				|| (c==0xA) //Unix
				)
			{
				_bLineEnd=true;
				return true;
			}
			if (c==0xD) //DOS
			{
				continue; //next byte is the newline
			}
			if((c!='\t')&&(c<' ')) 
			{
				
				//ZT("invalid char 0x%2x \"%c\"\n",c,c));
				//return false;
				c='~';
			}
			else
			{
				//continue; //skip white space?
			}

		}
		_value+=c;
	}
	_bBufferEnd=true;
	return true;
}




bool z_parse_csv::ParseBuffer(ctext buff,size_t size)
{
	_column_idx=0;
	_row_idx=0;
	_i=0;
	_bBufferEnd=false;
	_buff=buff;
	_buffSize=size;
	if(size==0)
		return false;
	while(_bBufferEnd==false) 
	{
		ZT("%20.20s",_buff+_i);
		if(!NewRowCallback()) 
		{
			ZT("!NewRowCallback()");
			return false;
		}
		if(!ParseLine())
		{
			ZT("ParseLine failed\n");
			return false;
		}
		if(!EndRowCallback()) 
		{
			ZT("!EndRowCallback()");
			return false;
		}
		_row_idx++;
	}
	ZT("ParseBuffer() done");
	return true;
}

