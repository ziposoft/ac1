#include "test_parse.h"

Z_MODULE_DECLARE(test);
Z_MODULE_DECLARE(xml);
Z_MODULE_INCLUDE(Z_MOD(test),Z_MOD(parse),Z_MOD(xml));

class zp_xml_atr : public zp_obj
{
	ZO_OBJ_H;
};
class zp_xml_elm : public zp_obj
{
	ZO_OBJ_H;
};
class zp_xml_trackpoint : public zp_obj
{
	ZO_OBJ_H;
	zp_xml_trackpoint()
	{
		static int i=0;
		i++;
		if(i%100 == 0)
			printf("\ntrackpoints=%d\n",i);


	}
};
class zp_xml_tcd : public zp_obj
{
	ZO_OBJ_H;
};
#define Z_MODULE _Z_MODULE(xml)
#define ZO_OBJ_LIST \
	OBJ(zp_xml_file,zp_obj,"zp_xml_file",0,"%whsp:('<?':^'?>':'?>'):+zp_xml_elm:%whsp",NO_FTR)\
OBJ(zp_xml_tcd,zp_obj,"zp_xml_tcd",0,"'a'",NO_FTR)\
OBJ(zp_xml_trackpoint,zp_obj,"zp_xml_trackpoint",0,"%whsp:'<Trackpoint>':+zp_xml_elm:'</Trackpoint>'",NO_FTR)\
OBJ(zp_xml_elm,zp_obj,"zp_xml_elm",0,"%whsp:'<':ident:*zp_xml_atr:(('>': *(^'<'|zp_xml_trackpoint|zp_xml_elm):'</':ident:'>')|'/>')",NO_FTR)\
OBJ(zp_xml_atr,zp_obj,"zp_xml_atr",0,"%whsp:scoped:'=':string",NO_FTR)

#include "zipolib_parse/include/z_obj_macro.h"


class zp_test1 : public zp_obj
{
	ZO_OBJ_H;
};

class zp_test2 : public zp_obj
{
	ZO_OBJ_H;
};

class zp_test3 : public zp_obj
{
	ZO_OBJ_H;
};
class zp_test4 : public zp_obj
{
public:
	zp_test4() { _val="tired"; }
	z_string _val;
	ZO_OBJ_H;
};

class zp_test5 : public zp_obj
{
public:
	zp_test5() 
	{ 
		_list_test4.push_back(new zp_test4());
		_list_test4.push_back(new zp_test4());
		_list_test4.push_back(new zp_test4());
		
	}
	void dump_custom(z_file& outf)
	{
		outf<<"list size is: "<<(int)_list_test4.size();
	}
	z_obj_vect<zp_test4> _list_test4;
	ZO_OBJ_H;
};

class zp_test6 : public zp_obj
{
public:
	zp_test4 _static_var;
	//zp_test4 *_p_var;

	zp_test6() 
	{ 
		_static_var._val="overwrite!";
		
	}
	void dump_custom(z_file& outf)
	{
		outf<<"_static_var="<<_static_var._val;
		//outf<<"_p_var="<<_p_var->_val;
	}
	ZO_OBJ_H;
};

class zp_test7 : public zp_obj
{
public:
	zp_test7() 
	{ 
		_on_list<<"def_on";
		_off_list<<"def_off";
		
	}
	void dump_list(zo_str_map& list,z_file& outf)
	{
		list.reset_iter();
		ctext val;
		do
		{
			val=list.get_next();
			if(val)
				outf<<val<<'\n';

		}while(val);
	}
	void dump_custom(z_file& outf)
	{
		outf<<"\nList_ON:\n";
		dump_list(_on_list,outf);		
		outf<<"\nList_OFF:\n";
		dump_list(_off_list,outf);
	}
	zo_str_map _on_list;
	zo_str_map _off_list;
	ZO_OBJ_H;
};

#define Z_MODULE _Z_MODULE(test)
#define ZO_OBJ_LIST \
OBJ(zp_test1,zp_obj,"test1",0,"'a'",NO_FTR)\
OBJ(zp_test2,zp_obj,"test2",0,"test1",NO_FTR)\
OBJ(zp_test3,zp_obj,"test3",0,"'a':'b':'c'",NO_FTR)\
OBJ(zp_test4,zp_obj,"test4",0,"#' ':{_val}ident",VAR(_val))\
OBJ(zp_test5,zp_obj,"test5",0,"{_list_test4}*test4",VAR(_list_test4))\
OBJ(zp_test6,zp_obj,"test6",0,"{_static_var}test4",VAR(_static_var))\
OBJ(zp_test7,zp_obj,"test7",0,"?{_on_list}identlist:':':?{_off_list}identlist",VAR(_on_list) VAR(_off_list))


#include "zipolib_parse/include/z_obj_macro.h"
/*

class zp_xml : public zp_obj
{
public:
	zt_func() { 
	}
	z_stl_obj_vector<zp_pair> _profiles;
	z_string _func_name;
	ZO_OBJ_H;
};
ZP_OBJ(zp_xml,zp_obj,"'<':ident:'>':",,,);
*/



zp_obj_parser p;
