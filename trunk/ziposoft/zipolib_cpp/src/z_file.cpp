#include "zipolib_cpp.h"
#include "z_file.h"
using namespace std;
#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
int vsnprintf(char *str, size_t size, ctext format, va_list ap);


z_file::z_file()
{
	_file_handle=0;
	_log_file_handle=0;
}
z_file::z_file(ctext filename)
{
	_file_handle=0;
	_log_file_handle=0;
	_file_name=filename;
}
z_file::~z_file()
{
	close();

}

z_file::z_file(size_t h)
{
	_file_handle=h;
	_log_file_handle=0;
}
z_file::z_file(ctext filename,ctext mode)
{
	_file_name=filename;
	_file_handle=(size_t)(void*)fopen(filename,mode);
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
	fseek((FILE*)_file_handle,0,SEEK_END);
	size=ftell((FILE*)_file_handle);
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
	char* tb=z_temp_buffer_get();
	fgets( tb, z_temp_buffer_size(), (FILE*)_file_handle );
	size_t l=strlen(tb);
	if(tb[l-1]=='\n')
		tb[l-1]=0;

	s=tb;
	z_temp_buffer_release();
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
#ifdef WIN32
	write("\r\n",2);
#else
	write("\n",1);
#endif
}
int z_file::putf(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ap;
	char* tempbuf=z_temp_buffer_get();
    va_start (ap, lpszFormat);
    c=vsnprintf (tempbuf,z_temp_buffer_size(), lpszFormat, ap);
    va_end (ap);
    write(tempbuf,strlen(tempbuf));
	z_temp_buffer_release();
    return c;
}
void z_file::flush()
{
    
#ifdef WIN32
    FlushFileBuffers(HANDLE(_file_handle));
#endif
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
#ifdef WIN32
	 OutputDebugString(buf);
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
	char* tempbuf=z_temp_buffer_get();
    va_start (ap, lpszFormat);
    c=vsnprintf (tempbuf,z_temp_buffer_size(), lpszFormat, ap);
    va_end (ap);
	int d=_depth;
	while(d--)
	{
		write("  ",2);
	}
    write(tempbuf,strlen(tempbuf));
	z_temp_buffer_release();

    return c;
}
int z_debug::putf(const char*  lpszFormat,  ...  )
{
    int c;
    va_list ap;
	char* tempbuf=z_temp_buffer_get();
    va_start (ap, lpszFormat);
    c=vsnprintf (tempbuf,z_temp_buffer_size(), lpszFormat, ap);
    va_end (ap);
    write(tempbuf,strlen(tempbuf));
	z_temp_buffer_release();
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

z_error gz_error;
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
