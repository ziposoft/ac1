#include "zipolib_cpp_pch.h"
#include "z_list.h"
void z_strlist::dump(z_file &out)
{
	size_t  i;
	for (i=0;i<size();i++)
	{
		out << (*this)[i] <<'\n';
	}


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


int z_parse_url_query(ctext buff,size_t len,z_strmap& vars)
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

