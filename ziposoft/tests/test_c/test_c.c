


#include "zipolib_c/include/zipo.h"
#include "zipolib_c/include/z_filesystem.h"




int main()
{
	ctext str1="ant";
	ctext str2="anthony";
	int x=2;
	z_directory_h hDir;
	utf8 pname;

//	Z_ERROR("Error test params %d %d",5,x);
//	Z_ERROR("Error test",5);



	printf("strcmp(%s,%s)=%d\n",str1,str2,strcmp(str1,str2));
	printf("strcmp(%s,%s)=%d\n",str2,str1,strcmp(str2,str1));

	if(z_dir_open("invaliddirectory",&hDir)==0)
	{
		printf("invaliddirectory opened?\n");
		return -1;
	}

	if(z_dir_open("testdir",&hDir)==-1)
	{
		printf("failed to open testdir\n");
		return -1;
	}
	while(z_dir_get_next(hDir,&pname,Z_DIR_TYPE_FILE)==0)
	{
		printf("%s\n",pname);
	}
	z_dir_close(hDir);
	if(z_dir_open("testdir/lvl2",&hDir)==-1)
	{
		printf("failed to open testdir\n");
		return -1;
	}
	while(z_dir_get_next(hDir,&pname,Z_DIR_TYPE_FILE)==0)
	{
		printf("%s\n",pname);
	}
	z_dir_close(hDir);

	return 0;//ZS_RET(base,feature_not_found);
}
