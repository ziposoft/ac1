#include "test_parse.h"



class testBadObj 
{
public:
;
};
class testBadObjChild 
{
public:
	testBadObj _child;
};

 #define ZO_OBJ_LIST \
   ZCLS(fileEnum,none,"fileEnum","{_root}ident:'-':{_num}int:'.':{_ext}ident",VAR(_root) VAR(_num) VAR(_ext)) \
	ZCLS(testA,none,"cmdline","{_val}ident:'=':{i123}int",VAR(i123) VAR(_val)) \
	ZCLS(testB,none,"cmdline","{_child}testA:{_exclam}?'!'",VAR(i222) VAR(_exclam)  POBJ(_child) )	\
	ZCLS(testBadObj,none,"cmdline","bogusident",NOFTR)	\
	ZCLS(testBadObjChild,none,"cmdline","{_child}testBadObj",OBJ(_child) )	\
	ZCLS(testStrList,none,"cmdline","+({_list}ident:?',')",VAR(_list)  )\
	ZCLS(testDrv,none,"cmdline","{_val}ident:'=':{i123}int",VAR(i123) VAR(_val) VAR(_val2))\
	ZCLS(testObjList,none,"cmdline","*({_list}testA:?',')",VAR(_list) )\
	ZCLS(zp_xml_elm,none,"zp_xml_elm","%whsp:'<':{_name}ident:{_attribs}*zp_xml_atr:(('>': *(^'<'|{_children}zp_xml_elm):'</':ident:'>')|'/>')",VAR(_name) VAR(_attribs) VAR(_children) )\
	ZCLS(zp_xml_atr,none,"zp_xml_atr","%whsp:{_name}scoped:'=':{_val}string_sq",VAR(_name) VAR(_val))

		  
#include "zipolib/include/z_obj.inc"
ZP_MODULE_DEFINE(testmod);


ZP_MODULE_INCLUDE(ZP_MOD(testmod), ZP_MOD(parse));


#if 0
ZP_MODULE_DECLARE(test);
ZP_MODULE_DECLARE(xml);
ZP_MODULE_INCLUDE(ZP_MOD(test),ZP_MOD(xml)/*,ZP_MOD(parse)*/);
	//ZCLS(zp_xml_file,base,"zp_xml_file","%whsp:('<?':^'?>':'?>'):{_tcd}zp_xml_elm:%whsp",VAR(_tcd))\


#define ZO_OBJ_LIST \
CLS(zp_xml_tcd,zp_obj,"zp_xml_tcd",0,"%whsp:'<TrainingCenterDatabase>':+zp_xml_elm:'</TrainingCenterDatabase>'",NO_FTR)\
CLS(zp_xml_activity,zp_obj,"zp_xml_activity",0,"%whsp:'<Activity>':+zp_xml_elm:'</Activity>'",NO_FTR)\
CLS(zp_xml_trackpoint,zp_obj,"zp_xml_trackpoint",0,"%whsp:'<Trackpoint>':+zp_xml_elm:'</Trackpoint>'",NO_FTR)\

#include "zipolib/include/z_obj.macro"
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


#include "zipolib_parse/include/z_obj.macro"
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