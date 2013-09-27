#include "zipo.h"
#include "z_utility.h"

//TODO make Unicode
char z_to_upper(char c) {if ((c>='a')&&(c<='z')) c=c-'a'+'A'; return c; }



U16 z_byte_swap16(U16 x)
{
	union {
		unsigned char	c[2];
		U16 i;
	} wc, wc1 ;

	wc.i = x;	/* assign */

	wc1.c[0] = wc.c[1];	/*swap */
	wc1.c[1] = wc.c[0];

	return(wc1.i);	/* return the swapped result */
}
U32 z_byte_swap32(U32 x)
{
	union {
		unsigned char	c[4];
		unsigned int i;
	} wc, wc1 ;

	wc.i = x;	/* assign */

	wc1.c[0] = wc.c[3];	/*swap */
	wc1.c[1] = wc.c[2];
	wc1.c[2] = wc.c[1];
	wc1.c[3] = wc.c[0];

	return(wc1.i);	/* return the swapped result */
}

int z_ipower(int val,int power)
{
	int i=1;
	while(power--)
	{
		i=i*val;
	}
	return i;
}

int z_str_same(utf8 s1,utf8 s2)
{
	if(!s1) return 0;
	if(!s2) return 0;
	if(strlen(s1)!=strlen(s2))
		return 0;
	return (strcmp(s1,s2)==0);
};
int z_str_check3(ctext id,utf8 s1,utf8 s2,utf8 s3)
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

utf8 z_get_filename_from_path(utf8 fullpath)
{
	utf8 filename=strrchr(fullpath,'\\');
	if(filename) filename++;
	else filename=fullpath;
	return filename;
}

