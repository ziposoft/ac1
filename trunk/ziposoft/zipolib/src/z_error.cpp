#include "zipolib_cpp_pch.h"
#include "z_os_specific.h"
#include "z_error.h"


struct z_error_tanslate_st
{
	int os_code;
	z_status status;
};
extern z_error_tanslate_st z_error_tanslate_list[];
extern size_t z_error_tanslate_list_size;

z_status z_get_zerror_from_os_error(int os_error)
{
	size_t i;
	for(i=0;i<z_error_tanslate_list_size;i++)
	{
		if(z_error_tanslate_list[i].os_code==os_error)
			return z_error_tanslate_list[i].status;
	}
	return zs_unknown_error;
}

#ifdef BUILD_VSTUDIO




z_error_tanslate_st z_error_tanslate_list[]=
{
	{ERROR_FILE_NOT_FOUND,zs_not_found},
	{ERROR_PATH_NOT_FOUND,zs_not_found},
	{ERROR_ACCESS_DENIED,zs_access_denied},
	{ERROR_SHARING_VIOLATION,zs_access_denied},
	{ERROR_NOT_ENOUGH_MEMORY,zs_not_found},
	{ERROR_READ_FAULT,zs_read_error},

};
size_t z_error_tanslate_list_size=sizeof(z_error_tanslate_list)/sizeof(z_error_tanslate_st);


z_status z_get_os_error(z_string *msg)
{
	DWORD error, success;
	
	LPVOID lpMsgBuf;
	error = GetLastError ();
	success = FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),	// Default language
				 (LPTSTR) & lpMsgBuf, 0, NULL);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.

	if (success)
	{
		if(msg)
		{
			*msg=(LPCWSTR)lpMsgBuf;
			*msg << '(' << (U32)error << ')';
		}
		//printf("ERROR %lx: %S\n",error,(LPCWSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
	}

	// Free the buffer.
	return z_get_zerror_from_os_error(error);


}
#else

z_status z_get_os_error(z_string *msg)
{




	return z_get_zerror_from_os_error(error);

}


#endif