#include "test_parse.h"



/*
TODO AND Groups with multi/optional stages
  
  *'a'&'b'  how should it work?
*/
const st_test_tmpl_entry test_tmpl_list[]=
{
	{"('a')",		"a",	zs_matched,			1,	"a",	0,		0,		0	},
	{"'a'",			"a",	zs_matched,			0,	0,		0,		0,		0	},
	{"*('a')",		"aaa",	zs_matched,			3,	"a",	"a",	"a",	0	},
	{"*('a'|'c')",	"aca",	zs_matched,			3,	"a",	"c",	"a",	0	},
	{"+('a'|'c')",	"aca",	zs_matched,			3,	"a",	"c",	"a",	0	},
	{"(Az):(int):'.':(ident)",	"fred001.ext",	zs_matched,			3,	"fred",	"001",	"ext",	0	},

	{"((Az):'!')|((Az):'+')",	"a+",	zs_matched,		1,	"a",	"c",	"a",	0	},

//problems
#if 0
	{"^'c':'c'","aaaaac",zs_matched,"c","c"},
	{"*(^'c'):'c'","aaaaac",zs_matched,"c","c"},
	{"*(^'c'|'b'):'c'","aaaaac",zs_matched,"c","c"},

	{"*('a'|' ')",
	"aaaaaaaaaaaaaa "
	,zs_matched,"",""},

	//strings
	//single groups
	{"string_sq","'a'",zs_matched,"a",""},
	{"string","\"a\"",zs_matched,"a",""},
	{"'a'","a",zs_matched,"a","a","simple string literal match"},
	{"('a')","a",zs_matched,"a","a",""},
	{"('a')","",zs_eof,"","a","a"},
	{"('a')","c",zs_no_match,"a","a",""},
	{"'a'","b",zs_no_match,"","a","simple string literal no match"},
	{"+'a'","aaa",zs_matched,"a","a"},
	{"*'a'","aaa",zs_matched,"",""},
	{"*'a'","",zs_eof,"",""},
	{"*'a'","ad",zs_unparsed_data,"",""},
	{"*('b')","bd",zs_unparsed_data,"",""},
	{"*'a'","xyz",zs_no_match,"",""},
	{"*('a')","",zs_eof,"",""},
	{"#'a'","",zs_eof,"","a"},


	
	//AND groups
	{"'a'&'b'","b",zs_no_match,"b","ab"},

	{"'a'&'b'&'c'","cab",zs_matched,"abc","abc"},
	{"'a'&'b'&'c'","ab",zs_no_match,"abc","abc"},
	//{"(*'b')&'d'","bd",zs_matched,"bd","d"},
	//{"*'b'&*'d'","bd",zs_matched,"",""},
	{"'a'&+'b'","aba",zs_unparsed_data,"ab","ab"},
	{"'a'&'b'&'a'","aba",zs_matched,"aba","aba"},
	{"'b'&+'a'","aba",zs_matched,"ba","ba"},
	{"+'a'&'b'","aba",zs_matched,"ab","ab"},

	//OR groups
	{"('a'|'b')","ba",zs_unparsed_data,"a","a"},
	{"('a'|'b')","b",zs_matched,"a","a"},
	{"*('a'|'b')","c",zs_no_match,"",""},
	{"*('a'|'b')","b",zs_matched,"",""},
	{"*('a'|'b'):'c'","babbaaabababac",zs_matched,"c","c"},
	{"*('a'|'b'):'c'","c",zs_matched,"c","c"},
	{"'a'|'b'","c",zs_no_match,"a","a"},
	{"'a'|'b'","b",zs_matched,"a","a"},
	{"*'a'|'b'","b",zs_matched,"b","b"},
	{"*('a'|'b')","bd",zs_unparsed_data,"",""},
	{"*('a'|'b')|*('c'|'d')","bd",zs_unparsed_data,"",""},
	{"*('a'|'b')|*('c'|'d')","ccc",zs_matched,"",""},
	{"(string|string_sq|ident)","ba",zs_matched,"",""},

	{"('<?':^'?>':'?>')","<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>",zs_matched,"<??>","<??>"},

	//sequence groups
	{"'a':'b':'c'","abc",zs_matched,"abc","abc"},
	{"'a':'b':'c'","acb",zs_no_match,"abc","abc"},
	{"('a':'b':'c')","abc",zs_matched,"abc","abc"},
	{"scoped:'[':*'a':']')","job[",zs_eof,"[]","[]"},
	{"'a':whsp:'b'","a\t b",zs_matched,"a\t b","ab"},
	//test
	{"ident:'=':ident","x=2",zs_matched,"x=2","="},
	{"ident:'=':ident","x=2",zs_matched,"x=2","="},
	{"ident:'=':?ident","x=2",zs_matched,"x=2","="},
	{"ident:'=':ident:#','","x=2 ",zs_matched,"x=2,","=,"},
	{"ident:'=':?ident:#','","x=2 ",zs_matched,"x=2,","=,"},




	//ident
	{"ident","anthony corriveau",zs_unparsed_data,"anthony",""},
	{"*' '&+ident","anthony corriveau",zs_matched,"anthonycorriveau",""},

	//random
	{"%'a':'b':'c'","aabaac",zs_matched,"bc","bc"},
	{"%'a'&'b'&'c'","acaab",zs_matched,"bc","bc"},
	{"%('a'|' '):'b':'c'","a b aac",zs_matched,"bc","bc"},
	{"%whsp:'b':'c'","b\tc",zs_matched,"\tbc","bc"},



	//Mixed groups
	{"('a':'b')|'a'","a",zs_matched,"ab","ab"},
	{"*'a':'b'","aaab",zs_matched,"b","b"},
	{"*('a'|'b')&'d'","bd",zs_matched,"d","d"},
	{"('a'|'b')&('c'|'d')","bd",zs_matched,"ac","ac"},
	{"'b'&*('c'|'d')","bd",zs_matched,"b","b"},
	{"('a'|'b')&*('c'|'d')","cbd",zs_matched,"a","a"},
	{"('a'&'b')|('c'&'d')","cbd",zs_no_match,"ab","ab"},
	{"('a'&'b')|('c'&'d')","cdab",zs_unparsed_data,"ab","ab"},
	{"*('a'&'b'):*('c'&'d')","cdab",zs_unparsed_data,"",""},
	{"*('a'&'b'):*('c'&'d')","abbaabcddc",zs_matched,"",""},
	//identlist
	//objects
	{"badobject","a",zs_no_entry_for_item,"",""},
	{"testA","a=2",zs_matched,"a=2","defaultstr=123"},




/*
	{"*test1","a",zs_matched,"a",""},
	{"+test1","a",zs_matched,"a","a"},
	{"+test1","aaaa",zs_matched,"aaaa","a"},
	{"test2","a",zs_matched,"a","a"},
	{"test3","abc",zs_matched,"abc","abc"},
	{"test4","b",zs_matched," b"," tired"},
	{"+(test3:'x')","abcxabcxabcx",zs_matched,"abcx","abcx"},
	{"pair","x=2",zs_matched,"x=2,","=,"},
	{"test5","x y z",zs_matched," tired tired tired x y z"," tired tired tired"},

	//PROBLEMS!!


	{"test7","z,x,y:",zs_matched,"A,x,y,z:B","A:B"},
	{"test7","z:x,a",zs_matched,"A,z:B,a,x","A:B"},
	{"test7",":x,a",zs_matched,"A:B,a,x","A:B"},
	{"test6","dudley",zs_matched," dudley"," overwrite!"},


{"identlist","x,y,x",zs_matched,"x,y,x",""},
	{"*'a'&'b'","ab",zs_matched,"b","b"},
	{"*'a'&'b'","ba",zs_matched,"b","b"},
	{"*'a'&'b'","aba",zs_matched,"b","b"},

	*/
#endif
};



const size_t test_tmpl_list_count=sizeof(test_tmpl_list)/sizeof(st_test_tmpl_entry);