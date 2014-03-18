#include "zb_pch.h"

#include "zbase_lib/include/ds_text.h"


zb_ds_text::zb_ds_text(ctext name)
{
	_name=name;

}
z_status zb_ds_text::open()
{


	z_strlist list;

	_dir.open(_name);
	_dir.get_files_by_extension("txt",list);

	return zb_not_implemented;

}
z_status zb_ds_text::close()
{
	return zb_not_implemented;

}

