


#include <stdio.h>
#include <stdlib.h>


unsigned char byte[2];

int main()
{
	char* str1="ant";
	char* str2="anthony";
	int x=2;
	int* p=0;
	int* xp=0;

   printf("byte=%x\n",byte);
   printf("&byte=%x\n",&byte);
   printf("&byte[0]=%x\n",&byte[0]);




	xp=&*p;
	printf("xp=%p\n",xp);
	printf("strcmp(%s,%s)=%d\n",str1,str2,strcmp(str1,str2));
	printf("strcmp(%s,%s)=%d\n",str2,str1,strcmp(str2,str1));


	return 0;//ZS_RET(base,feature_not_found);
}
