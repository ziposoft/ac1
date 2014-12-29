/**
* Copyright (C) 2005-2013 Christoph Rupp (chris@crupp.de).
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* See files COPYING.* for License information.
*
*
* A simple example, which creates a database, inserts some values,
* looks them up and erases them. Uses the C++ api.
*/

#include <iostream>
#include <string>
#include <ham/hamsterdb.h>
 #include "zipolib/include/z_console.h"
#include "zipolib/include/z_filesystem.h"
#include "zipolib/include/z_factory_static.h"

/* static modules */
ZP_MODULE_INCLUDE(  ZP_MOD(logger));

class root
{
public:
	root()
	{
		_p_logger=&z_logger_get();

	}
	z_console console;
	z_logger* _p_logger;
};

ZFACT(root)
{

};


int main(int argc, char* argv[])
{


	root o;
	o.console.setroot(&o);
	o.console.runapp(argc,argv,true);
	return 0;
}

