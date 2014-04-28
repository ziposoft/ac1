


#include "zipolib_cpp/include/zipolib_cpp.h"
#include "zipolib_cpp/include/z_filesystem.h"
#include "zipolib_cpp/include/z_trace.h"
#include "zipolib_cpp/include/z_ntf.h"

 class test	 : public 	z_ntf_obj
 {


 };




int main()
{
	ZT_ENABLE();
	ZTF;
	z_string larry="fred";

	ZT("Testing trace %s %d...\n","ddd",4);


	gz_out << "list of exe files:\n";


	z_directory localdir;
	z_strlist list;

	localdir.get_files_by_extension("exe",list);
	list.dump(gz_out);
	localdir.get_files_by_extension("lib",list);
	list.dump(gz_out);



	return 0;//ZS_RET(base,feature_not_found);
}
