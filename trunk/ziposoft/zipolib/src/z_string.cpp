#include "zipolib_cpp_pch.h"
#include "z_file.h"


#define TEMP_BUFF_SIZE 100 //TODO fix this
char g_temp_buff[  TEMP_BUFF_SIZE];		 //TODO fix this terrible


const z_string z_string_null;
//TODO FIX THIS
ctext STR(U32 i)
{
	//4294967295
	static char num[13];   //TODO - NOT THREAD SAFE
	snprintf(num,12,"%d",i);  
	return num;
}
ctext HEXSTR(U32 i)
{
	//4294967295
	static char num[13];//TODO - NOT THREAD SAFE
	snprintf(num,12,"%x",i);  
	return num;
}

void z_str_escape(ctext in,std::string& out)
{
	out.clear();
	size_t i;
	for(i=0;i<strlen(in);i++)
	{
		char c=in[i];
		switch (c)
		{
		case '\\':
			out.append( "\\\\");
			break;
		case '\"':
			out.append( "\\\"");
			break;
		default:
			out.append(1, c);
		break;
		}
	}

}


//TODO FIX THIS
bool z_string::FormatV(ctext pFormat,va_list ArgList) 
{
	vsnprintf(g_temp_buff,TEMP_BUFF_SIZE,pFormat,ArgList);
	assign(g_temp_buff);
	return true;
}




void z_string::stringize(z_string &to)
{
	to.clear();
	to.append("\"");
	size_t start=0;
	size_t dq=0;
	while( (dq=find_first_of("\"",start))!=z_string::npos)
	{
		to.append(*this,start,dq-start);
		to.append("\"");
		start=dq+1;
	}
 	to.append(*this,start,size()-start);



	to.append("\"");

}

bool z_string::Format(ctext pFormat,...) 
{
	va_list ArgList;
	va_start(ArgList,pFormat);
	FormatV(pFormat,ArgList);
	va_end(ArgList);
	return true;
}
void z_string::operator <<  (z_file &stream)
{
	stream.getline(g_temp_buff,TEMP_BUFF_SIZE);
	*this=g_temp_buff;
}
z_string & z_string::append_int (const int i)
{
	z_string temp;
	temp.Format("%d",i);
	//z_convert(i,temp);
	append(temp);
	return *this;
}

z_string & z_string::operator << (const int i)
{
	return append_int(i);
}
z_string & z_string::operator << (U32 i)
{
	z_string temp;
	temp.Format("%u",i);
	//z_convert(i,temp);
	append(temp);
	return *this;
}
#ifndef BUILD_VSTUDIO
int itoa(int val,char* buffer,int radix);
#endif

const z_string & z_string::operator = (const int val)
{
	char buff[20];
#ifdef BUILD_VSTUDIO
	_itoa_s(val,buff,19,10);
#else
	itoa(val,buff,10);
#endif
	assign(buff);
	return *this;
}
U32 z_string::GetDecVal() const
{
	return (U32)atol(c_str());    
}
const z_string & z_string::operator = (U32 i) 
{
	Format("%d",i);
	//z_convert(i,*this);
	return *this;
}
z_string::z_string(U32 i) 
{

	assign(STR(i));
}
z_string::z_string(int i) 
{
	assign(STR(i));
}

z_string operator+ (const char* a,const z_string& b)
{
	z_string c(a);
	c.append(b);
	return c;
}

z_string operator+ (const z_string& a, const char* b)
{
	z_string c(a);
	c.append(b);
	return c;
}
z_string operator+ (const z_string& a, const z_string& b)
{
	z_string c(a);
	c.append(b);
	return c;
}



void z_string::increment_name()
{
	if(size()==0) 
	{
		(*this)=="name";
		return;
	}
	ctext start=c_str();
	ctext pc=start;
	pc=pc+size()-1;
	while(1)
	{
		char c=*pc;
		if((c<'0')||(c>'9')) 
		{
			pc++;
			break;
		}
		if(pc==start) break;
		pc--;
	}
	z_string base_name;
	base_name.assign(this->c_str(),(size_t)(pc-start));
	int value=atoi(pc);
	value++;
	base_name<<value;
	*this=base_name;
}

