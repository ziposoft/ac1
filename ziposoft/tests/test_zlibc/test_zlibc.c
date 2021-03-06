


#include "zipolib/include/zipo.h"
#include "zipolib/include/z_directory.h"


void z_itobase2(char* buf, I64 value, int base, int buff_length, char padchar, int _signed);

int main()
{
	ctext str1="ant";
	ctext str2="anthony";
	int x=2;
	z_directory_h hDir;
	utf8 pname;
	I64 i64=-12345;
		int type;

	char buff[100];

	z_itobase2(buff,i64,10,99,' ',1);
	printf("%lld=%s\n",i64,buff);
	z_itobase2(buff,i64,16,99,' ',1);

	printf("%llx=%s\n",i64,buff);


	printf("strcmp(%s,%s)=%d\n",str1,str2,strcmp(str1,str2));
	printf("strcmp(%s,%s)=%d\n",str2,str1,strcmp(str2,str1));

	if(z_dir_open("invaliddirectory",&hDir)!=zs_ok)
	{
		printf("invaliddirectory opened?\n");
		_CrtDumpMemoryLeaks();
		return -1;
	}
	_CrtDumpMemoryLeaks();
	if(z_dir_open("testdir",&hDir)!=zs_ok)
	{
		printf("failed to open testdir\n");
		_CrtDumpMemoryLeaks();
		return -1;
	}
	while(z_dir_get_next(hDir,&pname,Z_DIR_TYPE_FILE,&type)==zs_ok)
	{
		printf("%s\n",pname);
	}
	z_dir_close(hDir);
	if(z_dir_open("testdir/lvl2",&hDir)==-1)
	{
		printf("failed to open testdir\n");
		_CrtDumpMemoryLeaks();
		return -1;
	}
	while(z_dir_get_next(hDir,&pname,Z_DIR_TYPE_FILE,&type)==0)
	{
		printf("%s\n",pname);
	}
	z_dir_close(hDir);
		_CrtDumpMemoryLeaks();

	return 0;//ZS_RET(base,feature_not_found);
}
