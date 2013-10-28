#include "z_parse_pch.h"
#include "z_parse_text.h"





//________________________________________________________________
//
//zp_text_parser
//________________________________________________________________
cset *cset_letters =0;
cset *cset_identchars =0;
cset *cset_scoped_identchars =0;
cset *cset_path_string =0;
cset *cset_digits =0;
cset *cset_white_space =0;

//________________________________________________________________
//
// INIT
//________________________________________________________________

zp_text_parser::zp_text_parser()
{
	set_source(0,0);
	_options.as_u32=0;
}
zp_text_parser::zp_text_parser(ctext code,size_t len) 
{
	set_source(code,len);
	_options.as_u32=0;
}

zp_status zp_text_parser::check_status(zp_status status)
{
	if(status>zs_eof)
	{
		int breakpoint;
		breakpoint=1;
	}
	if(status==zs_eof)
	{
		int breakpoint;
		breakpoint=1;
	}
	return status;
}


void zp_text_parser::set_source(const char* code,size_t len)
{
	n_newlines=0;
//	_index_furthest=0;
	_index_under_test=0;
	_match_start=0;
	if(len==(size_t)-1)
	{
		if(code)
			len=strlen(code);
	}
	if(!cset_letters)
	{
		cset_white_space=new cset(" \t\r\n");
		cset_letters=new cset("_A-Za-z");
		cset_digits=new cset("0-9");
		cset_identchars=new cset(*cset_letters + *cset_digits);
		cset_scoped_identchars=new cset(*cset_letters + *cset_digits+':');
		cset_path_string=new cset(*cset_letters + *cset_digits+"\\/.:"+'-');
	}
	set_index(code);
	_start=	code;
	_end=_start+len;
}
//________________________________________________________________
//
// UTIL
//________________________________________________________________

bool zp_text_parser::eob(char* i) { return (i>=_end);}
bool zp_text_parser::eob() 
{ 
	return ((_index_current==0)||(_index_current>=_end));
}
char zp_text_parser::current_ch() 
{ 
//	_index_under_test= _index_current;
	return *_index_current; 
}
ctext zp_text_parser::get_index_skip_ignored_chars()
{
	while(1)
	{
		char c=*_index_current;
		if(
			(_options.ignore_tabs && (c=='\t')) ||
			(_options.ignore_space && (c==' ')) ||
			(_options.ignore_newline && (c=='\n')) ||
			(_options.ignore_newline && (c=='\r')))
		{
			   _index_current++;
			   continue;
		}
		break;
	}
	return _index_current;
}
void zp_text_parser::index_reset()
{
	_index_current=_start;
}

void zp_text_parser::print_status()
{


}
void zp_text_parser::set_ignore_whitespace()
{
	_options.ignore_tabs=1;
	_options.ignore_space=1;
	_options.ignore_newline=1;

}


zp_status zp_text_parser::advance(size_t count)
{
	if(!_index_current) 
		return check_status(zs_eof);

	if( _index_current+count>_end)
		return check_status(zs_eof);

	while(count--)
	{
		if(*_index_current=='\n')
		{
				n_newlines++;
		}
		_index_current++;
	}
	/*
	if(_index_current>_index_furthest)
		_index_furthest=_index_current;
	*/
	return zs_ok;
}


char zp_text_parser::inc()
{
	if (eob()) return 0;
	char c=current_ch();
	if(c=='\n')
		n_newlines++;
	_index_current++;
	/*
	if(_index_current>_index_furthest)
		_index_furthest=_index_current;
		*/
	return c;
}
zp_status zp_text_parser::skip_ws()
{
	zp_status status=zs_no_match;
	while(1)
	{
		if(eob())
			return check_status(zs_eof);
		const char c= current_ch();
		if((c==' ')||
			(c=='\t')||
			(c=='\r'))
		{
			inc();
			status=zs_matched;
			continue;
		}
		return status;
	}
}

zp_status zp_text_parser::start_test()
{
	if(eob()) 
		return check_status(zs_eof);
	_match_start=get_index();
	set_index_under_test(get_index());
	_match_end=(ctext)-1;
	return zs_matched;

}

void zp_text_parser::print_context()
{
	z_string data;
	if(!_index_under_test)
		return;
	size_t arrow=debug(data,_index_under_test,12,12);
	printf("%s\n",data.c_str());
	size_t i;
	for(i=0;i<arrow;i++)
	{
		printf(".");
	}
	printf("^\n");
}
size_t zp_text_parser::debug(z_string &out,ctext pbuff,size_t before,size_t after)
{
	if(!pbuff)
		pbuff=get_index();

	const char* c=pbuff-before;
	if(c<_start)
	{
		c=_start;
		before=pbuff-_start;
	}
	const char* end=pbuff+after;
	if(end>_end)
	{
		end=_end;
		after=_end-pbuff;
	}
	while(c<end)
	{
		bool escaped=true;
		switch(*c)
		{
			case '\n':
			out << "\\n";
			break;
			case '\r':
			out << "\\r";
			break;
			case '\\':
			out << "\\";
			break;
			case '\t':
			out << "\\t";
			break;
			case ' ':
			out << "~";
			break;
			default:
				escaped=false;
				out<<*c;
			break;
		}
		if(escaped)
		{
			if(c<pbuff)
				before++;
			if(c>pbuff)
				after++;
		}
		c++;
	}
	return before;
	
}
//________________________________________________________________
//
// INTERNAL TESTS
//________________________________________________________________
zp_status zp_text_parser::_test_char(char c)
{
	if(eob()) 
		return check_status(zs_eof);
	if(current_ch()== c)
	{
		inc();
		return zs_matched;
	}
	return zs_no_match;
}
zp_status zp_text_parser::_test_end_char(char c) //  ~>(char) 
{
	while(!eob()) 
	{
		if(current_ch() == c)
		{
			inc();
			return zs_matched;
		}
		inc();
	}
	return check_status(zs_eof);
}

//________________________________________________________________
//
// EXTERNAL TESTS
//________________________________________________________________
zp_status zp_text_parser::test_chars(const cset &set)
{
	zp_status status;
	if((status=start_test())) return status;


	if(current_ch()& set)
	{
		inc();
		return zs_matched;
	}
	return zs_no_match;
}
zp_status zp_text_parser::test_to_eob()
{
	zp_status status;
	if((status=start_test())) return status;
	set_index(_end);
	return zs_matched;
}

zp_status zp_text_parser::test_char(char c)
{
	zp_status status;
	if((status=start_test())) return status;
	
	if(current_ch() == c)
	{
		inc();
		return zs_matched;
	}
	return zs_no_match;
}
zp_status zp_text_parser::test_file_path()
{
	return test_cset(*cset_path_string);
}



size_t zp_text_parser::get_index_offset()
{
	return _index_current-_start;
}

zp_status zp_text_parser::test_any_identifier()
{
	return test_cset(*cset_identchars);
}
zp_status zp_text_parser::test_identifier(const char* str)
{
	zp_status status;
	if((status=start_test())) return status;

	size_t match_len=0;
	size_t len=_end-get_index();
	size_t test_len=strlen(str);
	if (test_len> len) 
		return zs_no_match;
	while(match_len<len)
	{
		if(_index_current[match_len] & *cset_identchars) 
		{
			match_len++;
		}
		else
		{
			break;
		}

	}
	if (test_len== match_len) 
	{
		if(memcmp(str,get_index(),test_len)==0) 
		{
			advance(match_len);
			return zs_matched;
		}
	}
	return zs_no_match;
}

zp_status zp_text_parser::test_single_quoted_string()
{
	zp_status status;
	if((status=start_test())) 
		return status;

	if((status=_test_char('\''))) 
		return status;
	_match_start=get_index();
	if((_test_end_char('\''))) 
		return check_status(zs_template_syntax_error);
	_match_end=get_index()-1;
	return zs_matched;
}
zp_status zp_text_parser::test_not_single_quoted_string(const void *dummy)
{
	zp_status status;
	if((status=start_test())) 
		return status;

	if((status=_test_char('^'))) 
		return status;
	//inc();
	if((status=_test_char('\''))) 
		return check_status(zs_template_syntax_error);
	_match_start=get_index();
	if((_test_end_char('\''))) 
		return check_status(zs_template_syntax_error);
	_match_end=get_index()-1;
	return zs_matched;
}


zp_status zp_text_parser::test_code_string()
{
	zp_status status;
	if((status=start_test())) 
		return status;

	if((status=_test_char('\"'))) 
		return status;
	_match_start=get_index();
	if((_test_end_char('\"'))) 
		return check_status(zs_template_syntax_error);
	_match_end=get_index()-1;
	return zs_matched;

}
zp_status zp_text_parser::test_cset(const cset &set,size_t limit)
{
	zp_status status;
	if((status=start_test())) return status;

	size_t match_len=0;
	while( (*(get_index()+match_len)) &set) 
	{ 
		match_len++;
		if(match_len>=limit) break;
	}
	advance(match_len);
	return (match_len!=0 ? zs_matched : zs_no_match);
}


zp_status zp_text_parser::test_end_char(char c) //  ~>(char) 
{
	zp_status status;
	if((status=start_test())) 
		return status;

	return _test_end_char(c);
}


zp_status zp_text_parser::test_not_char(char c) //  ~^(char)  ~^\n
{
	zp_status status;
	if((status=start_test())) return status;

	while(!eob()) 
	{
		if(current_ch() == c)
		{
			return zs_matched;
		}
		inc();
	}
	return zs_matched;
}
zp_status zp_text_parser::test_any_chars(size_t n)// #5
{
	zp_status status;
	if((status=start_test())) return status;

	size_t len_left=_end-get_index();
	if (len_left< n) 
		return zs_no_match;

	advance(n);
	return zs_matched;
}


zp_status zp_text_parser::test_not_string(const char* str,size_t match_len)
{
	zp_status status;
	if((status=start_test())) return status;


	while(!eob())
	{
		size_t len=_end-get_index();
		if (match_len> len) 
			return zs_matched;
		if(memcmp(str,get_index(),match_len)==0)
		{
			return zs_matched;
		}
		inc();
	}
	return zs_matched;
}
zp_status zp_text_parser::test_string(const char* str,size_t match_len)
{
	zp_status status;
	if((status=start_test())) 
		return status;

	size_t len=_end-get_index();
	if (match_len> len) 
		return check_status(zs_eof);
	if(memcmp(str,get_index(),match_len)==0)
	{
		advance(match_len);		
		return zs_matched;
	}
	return zs_no_match;
}

zp_status zp_text_parser::test_string(const char* str)
{
	size_t match_len=strlen(str);
	return test_string(str,match_len);
}

//________________________________________________________________
//
// RETURNING MATCHES
//________________________________________________________________

void zp_text_parser::get_match(z_string& s)
{
	ctext start;
	size_t len;
	get_match(start,len);
	if(!start)
		return;
	s.assign(start,len);
}
void zp_text_parser::get_match(ctext& match_start,size_t& len)
{
	match_start=_match_start;
	if(_match_end==(ctext)-1)
		_match_end=get_index();

	len=_match_end-match_start;
}





zp_status zp_text_parser::ft_digits  (const void* dummy) 
{
	return test_cset(*cset_digits);
}

zp_status zp_text_parser::ft_single_quoted_string(const void* dummy)
{
	return test_single_quoted_string();
}
zp_status zp_text_parser::ft_file_path(const void* dummy)
{
	return test_file_path();
}

zp_status zp_text_parser::ft_any_identifier(const void* dummy)
{
	return test_any_identifier();
}
zp_status zp_text_parser::ft_scoped_identchars(const void* dummy)
{
	return test_cset(*cset_scoped_identchars);
}
zp_status zp_text_parser::ft_to_eol(const void* dummy)
{
	return  test_not_char('\n');
}

zp_status zp_text_parser::ft_test_identifier(const void* str)
{
	return  test_identifier((ctext)str);

}
zp_status zp_text_parser::ft_test_char(const void* c)
{
	return test_char((char)( (size_t)c & 0xFF));

}
