#include "zipolib_cpp.h"
#include "z_file.h"


#define TEMP_BUFF_SIZE 100 //TODO fix this
char g_temp_buff[  TEMP_BUFF_SIZE];		 //TODO fix this terrible


const z_string z_string_null;
//TODO FIX THIS
ctext STR(U32 i)
{
    //4294967295
    static char num[13];
    snprintf(num,12,"%d",i);  
    return num;
}
ctext HEXSTR(U32 i)
{
    //4294967295
    static char num[13];
    snprintf(num,12,"%x",i);  
    return num;
}
//TODO FIX THIS
bool z_string::FormatV(ctext pFormat,va_list ArgList) 
{
    vsnprintf(g_temp_buff,TEMP_BUFF_SIZE,pFormat,ArgList);
    assign(g_temp_buff);
	return true;
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

int z_strlist::break_string(ctext s,char break_char)
{
	int count=0;
	ctext ptr=s;
	ctext word_start=0;
	z_string str;
	while(1)
	{
		if((*ptr==break_char)||(*(ptr)==0))
		{
			if(word_start) 
			{
				str.assign(word_start,ptr-word_start);
				push_back(str);
				count++;
				word_start=0;
			}
			if(*(ptr)==0) break;
		}
		else
		{
			if(!word_start) 
				word_start=ptr;
		}
		ptr++;
	}
	return count;
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


int z_parse_url_query(ctext buff,size_t len,z_str_map& vars)
{
	char *newstr;
	size_t iCoded=0;
	int num=0;
	size_t iNew=0;
	newstr=new char [len+1];
	char *pVarStart=newstr;
	char *pValStart=0;
	bool add_var=false;
	bool done=false;	
	while(!done)
	{
		if (iCoded>len) 
		{
			newstr[iNew]=0;
			break;
		}		
		char x=buff[iCoded++];

		if (x=='+')
		{
			newstr[iNew++]=' ';
			continue;
		}		
		if (x=='%')	
		{
			static char hex[3];
			if (iCoded>=len) break;
			hex[0]=buff[iCoded++];
			if (iCoded>=len) break;
			hex[1]=buff[iCoded++];
			hex[2]=0;
			int val;
			sscanf(hex,"%x",&val);
			if (val!=10) newstr[iNew++]=val;
			continue;
		}	
		if (x=='=')
		{
			newstr[iNew++]=0;
			pValStart=&newstr[iNew];
			continue;
		}
		if (x=='&')
		{
			newstr[iNew++]=0;
			add_var=true;
		}
		if (x==0)
		{
			newstr[iNew]=0;
			add_var=true;
			done=true;
		}
		if(add_var)
		{

			vars[pVarStart]=pValStart;
			pVarStart=&newstr[iNew];
			pValStart=0;
			add_var=false;
			num++;
			continue;
		}
		newstr[iNew++]=x;

	}
	return num;
}

