#ifndef z_filesystem_cpp_h
#define z_filesystem_cpp_h
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_list.h"
#include "zipolib/include/z_files.h"


class z_directory
{
	z_directory_h _hDir;
	z_string _path;
public:
	z_directory(); //current directory.
	virtual ~z_directory(); //current directory.
	z_status open(ctext path,bool create =false);
	void close();
	 
	z_status  get_files_by_extension(ctext ext,z_strlist &str);
	z_status  set_to_cwdir();

};
z_status z_filesys_getcwd(z_string& path);
z_status z_filesys_setcwd(z_string& path);
z_status z_filesys_get_filename_from_path(z_string& fullpath,z_string& path_out,z_string& name_out,z_string& ext_out);

#endif

