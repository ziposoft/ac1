#include "test_parse.h"
class testA 
{
public:
	testA()
	{
	   i123=123;
	   _val="defaultstr";

	}
	int i123;
	z_string _val;
};
class testB 
{
public:
	int i222;
	testB()
	{
		_child=0;
		i222=222;
	}
	testA* _child;
};



 #define ZO_OBJ_LIST \
	ZCLS(testA,void,"cmdline","{_val}ident:'=':{i123}int",VAR(i123) VAR(_val)) \
	ZCLS(testB,void,"cmdline","{_child}testA",VAR(i222)  POBJ(_child) )


#include "zipolib_parse2/include/z_obj_macro.h"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod));


#if 0
ZP_MODULE_DECLARE(test);
ZP_MODULE_DECLARE(xml);
ZP_MODULE_INCLUDE(ZP_MOD(test),ZP_MOD(xml)/*,ZP_MOD(parse)*/);


#define ZO_OBJ_LIST \
	CLS(zp_xml_file,zp_obj,"zp_xml_file",0,"%whsp:('<?':^'?>':'?>'):{_tcd}zp_xml_elm:%whsp",VAR(_tcd))\
CLS(zp_xml_tcd,zp_obj,"zp_xml_tcd",0,"%whsp:'<TrainingCenterDatabase>':+zp_xml_elm:'</TrainingCenterDatabase>'",NO_FTR)\
CLS(zp_xml_activity,zp_obj,"zp_xml_activity",0,"%whsp:'<Activity>':+zp_xml_elm:'</Activity>'",NO_FTR)\
CLS(zp_xml_trackpoint,zp_obj,"zp_xml_trackpoint",0,"%whsp:'<Trackpoint>':+zp_xml_elm:'</Trackpoint>'",NO_FTR)\
CLS(zp_xml_elm,zp_obj,"zp_xml_elm",0,"%whsp:'<':{_name}ident:*zp_xml_atr:(('>': *(^'<'|zp_xml_trackpoint|zp_xml_elm):'</':ident:'>')|'/>')",VAR(_name))\
CLS(zp_xml_atr,zp_obj,"zp_xml_atr",0,"%whsp:scoped:'=':string",NO_FTR)

#include "zipolib_parse2/include/z_obj_macro.h"
ZP_MODULE_DEFINE(xml);

class zp_test1
{
};

class zp_test2 
{
};

class zp_test3 
{
};
class zp_test4 
{
public:
	zp_test4() { _val="tired"; }
	z_string _val;
};

class zp_test5 
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
		_on_list<<"A";
		_off_list<<"B";
		
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
CLS(zp_test1,zp_obj,"test1",0,"'a'",NO_FTR)\
CLS(zp_test2,zp_obj,"test2",0,"test1",NO_FTR)\
CLS(zp_test3,zp_obj,"test3",0,"'a':'b':'c'",NO_FTR)\
CLS(zp_test4,zp_obj,"test4",0,"#' ':{_val}ident",VAR(_val))\
CLS(zp_test5,zp_obj,"test5",0,"{_list_test4}*test4",VAR(_list_test4))\
CLS(zp_test6,zp_obj,"test6",0,"{_static_var}test4",VAR(_static_var))\
CLS(zp_test7,zp_obj,"test7",0,"?{_on_list}identlist:':':?{_off_list}identlist",VAR(_on_list) VAR(_off_list))


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
#endif