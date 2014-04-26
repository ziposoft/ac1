#include "zipolib_cpp_pch.h"
#include "z_filesystem.h"
#include "z_error.h"


z_status z_directory::open(ctext path,bool create)
{
	z_status status;
	if(_hDir!=NULL)
			return Z_ERROR_RETURN(z_status_already_open,"z_directory already open");

	status=z_dir_open(path,&_hDir); //current dir
	if(status==z_status_success)
		return status;

	if(create)
	{
		status=z_dir_create(path); //current dir
		if(status!=z_status_success)
			return Z_ERROR_RETURN(z_status_could_not_open_dir,"Could not create directory \"%s\"",path);
		status=z_dir_open(path,&_hDir); //current dir
		if(status==z_status_success)
			return status;
	}
	return Z_ERROR_RETURN(z_status_could_not_open_dir,"Could not open directory \"%s\"",path);


}
void z_directory::close()
{
	z_dir_close(_hDir);
	_hDir=NULL;

}
z_directory::z_directory()
{
	_hDir=NULL;

}
z_directory::~z_directory()
{
	if(_hDir)
		z_dir_close(_hDir);

}
z_status z_directory::get_files_by_extension(ctext ext,z_strlist &list)
{
	
	if(_hDir==NULL)
	{

		if(open(".")) 
			return Z_ERROR_RETURN(z_status_could_not_open_dir,"Could not open local directory");

	}
	z_string filename;
	ctext pname;
	list.clear();
	while(z_dir_get_next(_hDir,&pname,Z_DIR_TYPE_FILE)==0)
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
	close();
	return z_status_success;

}

