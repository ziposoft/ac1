#include "zipolib_cpp_pch.h"
#include "z_filesystem.h"
#include "z_error.h"


#ifdef UNIX
const char path_slash='/';
#else
const char path_slash='\\';
#endif
z_status z_filesys_get_filename_from_path(z_string& fullpath,z_string& path_out,z_string& name_out,z_string& ext_out)
{
	size_t dot=fullpath.rfind(	'.');
	size_t slash=fullpath.rfind(path_slash,dot);
	if(dot==-1)
	{
		dot=fullpath.length();
	}
	else
	{
		ext_out.assign(fullpath,dot+1,fullpath.length()-dot);
	}
	
	if(slash==z_string::npos)
		slash=0;
	name_out.assign(fullpath,slash,dot-slash);
	path_out.assign(fullpath,0,slash);


	return zs_ok;
}


z_status z_filesys_getcwd(z_string& path)
{
	utf8 current_dir=new char[128];
	if(z_filesys_get_current_dir(current_dir,128))
	{
		return Z_ERROR_MSG(zs_error,"Could not get current directory");
	}
	else
		path= current_dir;
	delete 	current_dir;

	return zs_ok;
}
z_status z_filesys_setcwd(z_string& path)
{
	int s= z_change_dir(path.c_str(),false);
	if(s)
		return zs_could_not_open_dir;
	return zs_ok;
}

z_status z_directory::set_to_cwdir()
{
	return z_filesys_getcwd(_path);

}
z_status z_directory::open(ctext path,bool create)
{
	z_status status;
	if(_hDir!=0)
			return Z_ERROR_MSG(zs_already_open,"z_directory already open");

	status=z_dir_open(path,&_hDir); //current dir
	if(status==zs_success)
		return status;

	if(create)
	{
		status=z_dir_create(path); //current dir
		if(status!=zs_success)
			return Z_ERROR_MSG(zs_could_not_open_dir,"Could not create directory \"%s\"",path);
		status=z_dir_open(path,&_hDir); //current dir
		if(status==zs_success)
			return status;
	}
	return Z_ERROR_MSG(zs_could_not_open_dir,"Could not open directory \"%s\"",path);


}
void z_directory::close()
{
	z_dir_close(_hDir);
	_hDir=0;

}
z_directory::z_directory()
{
	_hDir=0;

}
z_directory::~z_directory()
{
	if(_hDir)
		z_dir_close(_hDir);

}
z_status z_directory::get_files_by_extension(ctext ext,z_strlist &list)
{
	
	if(_hDir==0)
	{

		if(open(".")) 
			return Z_ERROR_MSG(zs_could_not_open_dir,"Could not open local directory");

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
	return zs_success;

}

