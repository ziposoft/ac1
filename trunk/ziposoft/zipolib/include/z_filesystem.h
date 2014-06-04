#ifndef z_filesystem_cpp_h
#define z_filesystem_cpp_h
#include "zipolib/include/zipo.h"
#include "zipolib/include/z_list.h"
#include "zipolib/include/z_files.h"


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
