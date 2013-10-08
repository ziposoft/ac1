#include "test_parse.h"



/*
TODO AND Groups with multi/optional stages
  
  *'a'&'b'  how should it work?
*/
const st_test_tmpl_entry test_tmpl_list[]=
{
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
	//OR groups
	{"('a'|'b')","ba",zs_unparsed_data,"a","a"},
	{"('a'|'b')","b",zs_matched,"a","a"},
	{"*('a'|'b')","c",zs_no_match,"",""},
	{"*('a'|'b')","b",zs_matched,"",""},
	{"*('a'|'b')","ba",zs_matched,"",""},
	{"'a'|'b'","c",zs_no_match,"a","a"},
	{"'a'|'b'","b",zs_matched,"a","a"},
	{"*'a'|'b'","b",zs_matched,"b","b"},
	{"*('a'|'b')","bd",zs_unparsed_data,"",""},
	{"*('a'|'b')|*('c'|'d')","bd",zs_unparsed_data,"",""},
	{"*('a'|'b')|*('c'|'d')","ccc",zs_matched,"",""},
	//AND groups
	{"'a'&'b'","b",zs_eof,"b","ab"},
	{"'a'&'b'","b",zs_eof,"b","ab"},

	{"'a'&'b'&'c'","cab",zs_matched,"abc","abc"},
	{"'a'&'b'&'c'","ab",zs_eof,"abc","abc"},
	//{"(*'b')&'d'","bd",zs_matched,"bd","d"},
	//{"*'b'&*'d'","bd",zs_matched,"",""},
	{"+'a'&+'b'","aba",zs_matched,"ab","ab"},


	//sequence groups
	{"'a':'b':'c'","abc",zs_matched,"abc","abc"},
	{"'a':'b':'c'","acb",zs_no_match,"abc","abc"},
	{"('a':'b':'c')","abc",zs_matched,"abc","abc"},
	{"scoped:'[':*'a':']')","job[",zs_eof,"[]","[]"},
	{"'a':whsp:'b'","a\t b",zs_matched,"a\t b","ab"},


	//random
	{"%'a':'b':'c'","aabaac",zs_matched,"bc","bc"},
	{"%'a'&'b'&'c'","acaab",zs_matched,"bc","bc"},
	{"%('a'|' '):'b':'c'","a b aac",zs_matched,"bc","bc"},
	{"%whsp:'b':'c'","b\tc",zs_matched,"\tbc","bc"},



	//Mixed groups
	{"('a':'b')|'a')","a",zs_matched,"a","a"},
	{"*'a':'b'","aaab",zs_matched,"b","b"},
	{"*('a'|'b')&'d'","bd",zs_matched,"d","d"},
	{"('a'|'b')&('c'|'d')","bd",zs_matched,"ac","ac"},
	{"'b'&*('c'|'d')","bd",zs_matched,"b","b"},
	{"('a'|'b')&*('c'|'d')","cbd",zs_matched,"a","a"},
	{"('a'&'b')|('c'&'d')","cbd",zs_no_match,"ab","ab"},
	{"('a'&'b')|('c'&'d')","cdab",zs_matched,"ab","ab"},
	{"*('a'&'b'):*('c'&'d')","cdab",zs_matched,"",""},
	{"*('a'&'b'):*('c'&'d')","abbaabcddc",zs_matched,"",""},
	//ident
	{"*' '&+ident","anthony corriveau",zs_matched,"anthonycorriveau",""},
	//objects
	{"test1","a",zs_matched,"a","a"},
	{"*test1","a",zs_matched,"a",""},
	{"+test1","a",zs_matched,"a","a"},
	{"+test1","aaaa",zs_matched,"aaaa","a"},
	{"test2","a",zs_matched,"a","a"},
	{"test3","abc",zs_matched,"abc","abc"},
	{"test4","b",zs_matched," b"," tired"},
	{"+(test3:'x')","abcxabcxabcx",zs_matched,"abcx","abcx"},
	{"pair","x=2",zs_matched,"x=2,","=,"},
	{"test5","x y z",zs_matched," x y z"," tired tired tired"},
	//identlist
	{"test7","z,x,y:",zs_matched,"x,y,z:","def_on:def_off"},
	{"test7","z:x,a",zs_matched,"z:a,x","def_on:def_off"},
	{"test7",":x,a",zs_matched,":a,x","def_on:def_off"},
	{"test6","dudley",zs_matched," dudley"," overwrite!"},


	//PROBLEMS!!
/*
	{"identlist","x,y,x",zs_matched,"x,y,x",""},
	{"*'a'&'b'","ab",zs_matched,"b","b"},
	{"*'a'&'b'","ba",zs_matched,"b","b"},
	{"*'a'&'b'","aba",zs_matched,"b","b"},

	*/
};



const size_t test_tmpl_list_count=sizeof(test_tmpl_list)/sizeof(st_test_tmpl_entry);