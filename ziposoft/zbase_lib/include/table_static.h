#ifndef TABLE_STATIC_H
#define TABLE_STATIC_H
#include "zbase_lib/include/table.h"
#include "zbase_lib/include/field_static.h"


enum zk_st
{
	zk_st_master=1,
	zk_st_test,
	zk_st_users,
	zk_st_fields,
	zk_st_field_text,
	zk_st_field_int,
	zk_st_dynamic_start=100,
};

class zb_st_test : public zb_table_static
{//zk_st_master=1
public:
	zb_st_test(zb_source* ds);
	zb_field_string* _f_name;


};


class zb_st_master : public zb_table_static
{//zk_st_master=1
public:
	zb_st_master(zb_source* ds);


	zb_table_base* get_table(zk_st id);
};

class zb_st_users : public zb_table_static
{//zk_st_users=2
public:
	zb_st_users(zb_source* ds);
};
class zb_st_fields : public zb_table_static
{//zk_st_fields=3
public:
	zb_st_fields(zb_source* ds);
};
class zb_st_field_base : public zb_table_static
{
public:
	zb_st_field_base(zb_source* ds);
};
class zb_st_field_text : public zb_st_field_base
{//zk_st_field_text
public:
	zb_st_field_text(zb_source* ds);
};
class zb_st_field_int : public zb_st_field_base
{//zk_st_field_text
public:
	zb_st_field_int(zb_source* ds);
};
#endif
