#include "zb_pch.h"

#include "zbase_lib/include/ds_text.h"

/*___________________________________________________________________________

	zb_ds_text 
____________________________________________________________________________*/
zb_ds_text::zb_ds_text(ctext name) : zb_source(name)
{

}


zb_ds_text::~zb_ds_text()
{
	//TODO delete 
}
z_status zb_ds_text::open(bool create,bool writable)
{
	z_status status;
	size_t i;
	status=_dir.open(_name,create);
	if(status)
		return Z_ERROR(zb_status_cant_open_file,"can't open directory");

	z_strlist list;
	_dir.get_files_by_extension("txt",list);

	for(i=0;i<list.size();i++)
	{
		_tables << new 
			zb_ds_table_txt(this,list[i]);

	}




	 return 0;
}
z_status zb_ds_text::close()
{
	 return ZB_ERROR(zb_status_not_implemented);

}

/*___________________________________________________________________________

	zb_ds_table_txt 
____________________________________________________________________________*/



zb_ds_table_txt::zb_ds_table_txt(zb_ds_text* ds,ctext unique_id):zb_ds_table(ds,unique_id)
{
	_ds=ds;
}
z_status zb_ds_table_txt::record_add(zb_ds_rec_ptr* rec)
{
	zb_rec_ptr_txt* mk_rec=dynamic_cast<zb_rec_ptr_txt*>(rec);
	return zb_status_ok;
}

z_status zb_ds_table_txt::open(bool writable)
{
	ctext flags="r";
	if(writable)
		flags="rw";
	z_status s=_file.open(_id,flags);
	if(s)
		return ZB_ERROR(zb_status_cant_open_file);


	return 0;
}
size_t zb_ds_table_txt::get_record_count()
{
	return 0;
}
z_status zb_ds_table_txt::get_record_by_index(size_t index,
											  zb_ds_rec_ptr** cursor)
{
		return ZB_ERROR(zb_status_index_out_of_range);
		return ZB_ERROR(zb_status_bad_param);

	zb_rec_ptr_txt* r=dynamic_cast<zb_rec_ptr_txt*>(*cursor);
	if(r==0)
		r=new zb_rec_ptr_txt(false);
	r->set(this,index);
	*cursor=r;
	//r->get_row_ref()=_mk_view[index];
	return zb_status_ok;
}
/*___________________________________________________________________________

	zb_rec_ptr_txt 
____________________________________________________________________________*/


zb_rec_ptr_txt::zb_rec_ptr_txt(bool solo)
{



}

zb_rec_ptr_txt::~zb_rec_ptr_txt()
{



}
void zb_rec_ptr_txt::set(zb_ds_table* rs,size_t index)
{
	//_set=dynamic_cast<zb_rec_ptr_txt*>(rs);
	//_index=index;

}
