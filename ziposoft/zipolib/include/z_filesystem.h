#ifndef z_filesystem_cpp_h
#define z_filesystem_cpp_h
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_list.h"
#include "zipolib/include/z_directory.h"


class z_directory
{
	z_directory_h _hDir;
	z_string _path;
public:
	z_directory(); //current directory.
	virtual ~z_directory(); //current directory.
	z_status open(ctext path,bool create =false);
	void close();
	z_status traverse_tree(); 
	z_status traverse_tree_recurse(z_directory_h dir); 
	z_status  get_files_by_extension(ctext ext,z_strlist &str);
	z_status  set_to_cwdir();
	ctext get_path();
	virtual void callback_file(ctext name,ctext fullpath) ;
	virtual void callback_dir(ctext name,ctext fullpath) ;


};
z_status z_filesys_getcwd(z_string& path);
z_status z_filesys_setcwd(z_string& path);
z_status z_filesys_get_path_parts(z_string& fullpath,z_string* path_out,z_string* name_out=0,z_string* ext_out=0);

#endif

