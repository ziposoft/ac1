/*________________________________________________________________________

 z_ntf_h

________________________________________________________________________*/


#ifndef z_ntf_h
#define z_ntf_h

#include "zipolib/include/z_factory_dyn.h"
class z_ntf_obj;




class z_ntf :public z_factory_dyn// Interface that contains it's own data. 
{

public:
	z_ntf(ctext name) : z_factory_dyn(name)
	{
	}

	

};


#endif

