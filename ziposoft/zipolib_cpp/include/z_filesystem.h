#ifndef z_filesystem_cpp_h
#define z_filesystem_cpp_h
#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_cpp/include/z_string.h"
#include "zipolib_c/include/z_files.h"


class z_directory
{
	z_directory_h _hDir;
public:
	z_directory(); //current directory.
	virtual ~z_directory(); //current directory.
	z_status open(ctext path,bool create =false);
	void close();
	 
	z_status  get_files_by_extension(ctext ext,z_strlist &str);


};


#endif

