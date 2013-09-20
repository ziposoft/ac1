#include "zipolib/include/z_filesystem.h"
#include "zipolib/include/z_utility.h"
#include "zipolib/include/z_debug.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef BUILD_VSTUDIO
#include  <direct.h>
#else
#include  <string.h>
#include  <errno.h>
#define MAX_PATH 2048
#include  <unistd.h>
#include  <dirent.h>
#include  <sys/stat.h>
#endif
#if UNIX

#endif
int z_fopen(z_fileh* filep,ctext _Filename,ctext _Mode)
{
#ifdef BUILD_VSTUDIO
	return fopen_s((FILE**)filep,_Filename,_Mode);
#else
	*filep=(z_fileh)fopen(_Filename,_Mode);
	if(*filep == 0) return -1;
	return 0;
#endif
}



char* z_file_open_and_read(ctext in_filepath,unsigned long *bytesread  )
{
#ifdef BUILD_VSTUDIO
	SECURITY_ATTRIBUTES sa;
	int size;
	char* code;
	HANDLE handle= CreateFile(in_filepath, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL /*&sa*/, OPEN_EXISTING, 
		FILE_FLAG_SEQUENTIAL_SCAN, 0);	
	memset(&sa,0,sizeof(SECURITY_ATTRIBUTES));
	if(handle==INVALID_HANDLE_VALUE) return 0;
	size=GetFileSize(handle,NULL);
	code=(char*)malloc (size+1);
	if(ReadFile(handle,code,size,bytesread,NULL)) return code;
	return 0;

#else
	FILE* pFile=fopen(in_filepath,"r");
	char * code;
	size_t len;
	printf("pFile=%lx\n",(size_t)pFile);

	fseek(pFile,0,SEEK_END);
	len=ftell(pFile);
	printf("len=%lu\n",len);
	code=(char*)malloc(len+1);
	rewind(pFile);
	*bytesread=fread(code,1,len,pFile);
	code[len]=0;
	printf("bytesread=%lu\n",*bytesread);
	fclose(pFile);

	return code;
#endif

}
int z_file_open_and_write(ctext in_filepath,const char* data,unsigned long length  )
{
	U32 byteswritten=0;
#ifdef BUILD_VSTUDIO
	//SECURITY_ATTRIBUTES sa;
	HANDLE handle= CreateFile(in_filepath, GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL /*&sa*/, CREATE_ALWAYS, 
		FILE_FLAG_SEQUENTIAL_SCAN, 0);	

	//memset(&sa,0,sizeof(SECURITY_ATTRIBUTES));

	if(handle==INVALID_HANDLE_VALUE) return 0;
	if(WriteFile(handle,data,length,(LPDWORD)&byteswritten,NULL)) return byteswritten;
	return 0;

#else
	FILE* pFile=fopen(in_filepath,"wb");
	byteswritten=fwrite(data,1,length,pFile);

	fclose(pFile);

	return byteswritten;
#endif

}


/*
void OutFile(ctext out_filepath,outmemory& out)
{
outfile of(out_filepath);
try 
{
of.open();
//int status=of.get_status();
of.put(out.get_data());
//gz_out.put(out.get_data());
}
catch (estream* e) 
{
gz_out.putf("Error: %s\n", (ctext)e->get_message());
delete e;
}
}
*/


/*
file delete

returns 
'0' on success
'1' if it cant find the file
'-1' if it cannot delete the file
*/
int z_file_delete(const char* name)
{
#ifdef BUILD_VSTUDIO
	ULONG error;
	if(DeleteFile(name)==TRUE) return 0;
	error=GetLastError();
	if(error==ERROR_FILE_NOT_FOUND) return 1;
#else
	return remove(name);
#endif
	return -1;
}

int    z_make_dir(ctext dir_name)
{
#ifdef BUILD_VSTUDIO
	return (mkdir(dir_name)) ;
#elif defined BUILD_MINGW
	return mkdir(dir_name /*S_IRUSR| S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH*/) ;

#else
	return mkdir(dir_name,0xffff /*S_IRUSR| S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH*/) ;

#endif
}
int    z_change_dir(ctext dir_name,int create)
{
	if(chdir(dir_name)==0) return 0;
	if(!create) return -1;
	if(z_make_dir(dir_name)) return -2;
	if(chdir(dir_name)) return -3;
	return 0;
}
typedef struct _z_directory_t
{
#ifdef BUILD_VSTUDIO
	WIN32_FIND_DATA FindFileData;
	HANDLE handleDirectory ;
#else
    DIR* handleDirectory ;
	struct dirent* entry;
#endif
	char* buff;

} _z_directory;
int    z_dir_open(ctext name,z_directory_h* h)
{
	_z_directory* zdir=malloc(sizeof(_z_directory));
	zdir->buff=malloc(MAX_PATH);
	snprintf(zdir->buff,MAX_PATH,"%s//*",name);
	zdir->handleDirectory=0;
	DBG_OUT(("z_dir_open %s\n",name));
	*h=(z_directory_h)zdir;
#ifdef BUILD_VSTUDIO
	return 0;
#else
	zdir->handleDirectory=opendir(name);
	DBG_OUT(("handleDirectory %x\n",zdir->handleDirectory));
	if(!zdir->handleDirectory) 
		return -1;
#endif
	return 0;
}


int     z_dir_get_next(z_directory_h h,ctext* currentfile,int type)
{
	int isDir;
	int handle_dir=0;
	_z_directory* zdir=(_z_directory*)h;
	DBG_OUT(("z_dir_get_next\n"));
	while(1)
	{
#ifdef BUILD_VSTUDIO
		if(zdir->handleDirectory==0)
		{
			zdir->handleDirectory = FindFirstFile(zdir->buff, &(zdir->FindFileData));
			if(INVALID_HANDLE_VALUE==zdir->handleDirectory)
			{
				zdir->handleDirectory=0;
				return -1;
			}
		}
		else
		if (FindNextFile(zdir->handleDirectory, &(zdir->FindFileData))==0) 
		{
			return -1;
		}
		*currentfile=zdir->FindFileData.cFileName;
		isDir=(zdir->FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
#else
		struct stat status;
		zdir->entry=readdir(zdir->handleDirectory);
		DBG_OUT(("zdir->entry=%x\n",zdir->entry));
		if(!zdir->entry)
			return -1;
		*currentfile=zdir->entry->d_name;
		DBG_OUT(("currentfile=%x %s\n",currentfile,*currentfile));
#ifdef BUILD_MINGW	
		isDir=opendir(zdir->entry->d_name);
		if(isDir) close(isDir);
		
#else
		isDir=(zdir->entry->d_type==DT_DIR);
#endif
		DBG_OUT(("isDir=%x\n",isDir));

#endif
		if(type&Z_DIR_TYPE_DIR)
			if(!isDir) continue;
		if(type&Z_DIR_TYPE_FILE)
			if(isDir) continue;
		if(strcmp(*currentfile,".")==0) continue;
		if(strcmp(*currentfile,"..")==0) continue;
		return 0;
	}
	return -1;
}

void   z_dir_close(z_directory_h h)
{
	_z_directory* zdir=(_z_directory*)h;
	if(!zdir) return;
	if(zdir->buff) free(zdir->buff);

	if(zdir->handleDirectory) 
#ifdef BUILD_VSTUDIO	
		FindClose(zdir->handleDirectory);
#else
		closedir(zdir->handleDirectory);
#endif
	zdir->handleDirectory=0;

	free(zdir);
}
