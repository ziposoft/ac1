//________________________________________________________________________/////////////////////////
//
// z_filesystem_h
//
//________________________________________________________________________/////////////////////////
#ifndef z_filesystem_h
#define z_filesystem_h
#ifdef	__cplusplus
extern "C" {
#endif
#include "zipolib_c/include/zipo.h"
typedef size_t z_fileh;
typedef size_t z_directory_h;
char*  z_file_open_and_read(ctext in_filepath,unsigned long *bytesread  );
int    z_file_open_and_write(ctext in_filepath,const char* data,unsigned long length  );
int    z_fopen(z_fileh* filep,ctext _Filename,ctext _Mode);
int    z_file_delete(const char* name);

int    z_change_dir(ctext dir,int create);
int    z_make_dir(ctext dir_name);


int    z_dir_open(ctext name,z_directory_h* h);
void    z_dir_close(z_directory_h h);

#define Z_DIR_TYPE_DIR  1
#define Z_DIR_TYPE_FILE 2
int    z_dir_get_next(z_directory_h h,ctext* currentfile,int type);


#ifdef	__cplusplus
}
#endif
#endif

