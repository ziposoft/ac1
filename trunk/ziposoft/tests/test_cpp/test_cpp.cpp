


#include "zipolib_cpp/include/zipolib_cpp.h"




int main()
{
	ZT_ENABLE();
	ZTF;
	z_string larry="fred";

	ZT(("Testing trace...\n"));


	gz_out << larry;

	return 0;//ZS_RET(base,feature_not_found);
}
