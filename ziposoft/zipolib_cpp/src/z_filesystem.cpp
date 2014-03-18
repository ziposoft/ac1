#include "zipolib_cpp_pch.h"
#include "z_filesystem.h"
#include "z_error.h"


z_status z_directory::open(ctext path)
{


}
void z_directory::close()
{


}
z_directory::z_directory()
{
	hDir=NULL;

}
z_directory::~z_directory()
{
	if(hDir)
		z_dir_close(hDir);

}
z_status z_directory::get_files_by_extension(ctext ext,z_strlist &list)
{
	z_status status;
	if(hDir==NULL)
	{
		status=z_dir_open(".",&hDir); //current dir
		if(status==-1) 
			return Z_ERROR_RETURN(z_status_could_not_open_dir,"Could not open local directory");

	}
	z_string filename;
	ctext pname;
	list.clear();
	while(z_dir_get_next(hDir,&pname,Z_DIR_TYPE_FILE)==0)
	{
		filename=pname;
		size_t found=filename.find_last_of('.');
		if(found==z_string::npos)
			continue;

		if (filename.substr(found+1)==ext)
		{
			list << filename;
		}
	}
	z_dir_close(hDir);
	hDir=NULL;
	return z_status_success;

}

