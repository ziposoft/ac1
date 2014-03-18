/*________________________________________________________________________

z_utility.c

________________________________________________________________________*/

#include "zipo.h"
#include "z_utility.h"

//TODO make Unicode
char z_to_upper(char c) {if ((c>='a')&&(c<='z')) c=c-'a'+'A'; return c; }

ctext z_int_to_ctext(int i)
{
	static char buff[15];
	return itoa(i,buff,10);

}
int z_str_same(utf8 s1,utf8 s2)
{
	if(!s1) return 0;
	if(!s2) return 0;
	if(strlen(s1)!=strlen(s2))
		return 0;
	return (strcmp(s1,s2)==0);
};
int z_str_check3(utf8 id,utf8 s1,utf8 s2,utf8 s3)
{
	size_t len;
	if(!id) return 0;
	len=strlen(id);
	if(s1)
		if(strlen(s1)==len)
			if(strcmp(s1,id)==0)
				return 1;
	if(s2)
		if(strlen(s2)==len)
			if(strcmp(s2,id)==0)
				return 1;
	if(s3)
		if(strlen(s3)==len)
			if(strcmp(s3,id)==0)
				return 1;

	return 0;
};




