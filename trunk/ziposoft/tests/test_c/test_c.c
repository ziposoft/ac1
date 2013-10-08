


#include "zipolib_c/include/zipo.h"




int main()
{
	ctext str1="ant";
	ctext str2="anthony";
	int x=2;
//	Z_ERROR("Error test params %d %d",5,x);
//	Z_ERROR("Error test",5);



	printf("strcmp(%s,%s)=%d\n",str1,str2,strcmp(str1,str2));
	printf("strcmp(%s,%s)=%d\n",str2,str1,strcmp(str2,str1));

	return 0;//ZS_RET(base,feature_not_found);
}
