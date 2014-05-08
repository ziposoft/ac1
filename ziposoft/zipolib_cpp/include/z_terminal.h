#ifndef terminal_h
#define terminal_h
#include "zipolib_cpp/include/zipolib_cpp_pch.h"


#ifdef BUILD_VSTUDIO
	#include <wincon.h>
	#include <conio.h>
#else
#ifdef UNIX
	#include <signal.h>
	#include <termios.h>
	#include <stdlib.h>
	#include <unistd.h>
void CatchSigs();
#endif 
#endif 


enum enum_key
{
	key_alpha,
	key_right,
	key_left,
	key_up,
	key_down,
	key_esc,
	key_enter,
	key_home,
	key_pageup,
	key_page_down,
	key_insert,
	key_delete,
	key_end,
	key_ctrl_C,
	key_back,
	key_tab,
	key_unknown,
	key_max
};
struct key_def
{
	enum_key type;
	ctext name;
	U32 length;
	ctext code;
};
static key_def keys[]=
{ 
	{key_back		,"back",	1,	"\x008"},
#if 1
#ifdef UNIX
	{key_page_down	,"page_down	", 4,"\x01b\x05b\x036\x07e"},
	{key_right		,"right",	3,   "\x01b\x05b\x043"},
	{key_left		,"left",	3,	 "\x01b\x05b\x044"},
	{key_up			,"up",		3,   "\x01b\x05b\x041"},
	{key_down		,"down",	3,   "\x01b\x05b\x042"},
#endif
#ifdef LINUX
	{key_pageup		,"pageup",	4,"\x01b\x05b\x035\x07e"},
	{key_insert		,"insert",	4,"\x01b\x05b\x032\x07e"},
	{key_delete		,"delete",	4,"\x01b\x05b\x033\x07e"},
	{key_home		,"home",	3,"\x01b\x05b\x048"},
	{key_end		,"end",	    3,"\x01b\x05b\x049"},

#endif
#ifdef LYNX
	{key_pageup		,"pageup",	4,"\x01b\x05b\x033\x07e"},
	{key_insert		,"insert",	4,"\x01b\x05b\x031\x07e"},
	{key_delete		,"delete",	4,"\x01b\x05b\x034\x07e"},
	{key_home		,"home",	3,"\x01b\x05b\x032"},
	{key_end		,"end",	    3,"\x01b\x05b\x035"},
#endif
#endif

#ifdef BUILD_VSTUDIO
	{key_right		,"right",	2,	"\x0e0\x04d"},
	{key_left		,"left",	2,	"\x0e0\x04b"},
	{key_up			,"up",		2,	"\x0e0\x048"},
	{key_down		,"down",	2,	"\x0e0\x050"},
	{key_home		,"home",	2,	"\x0e0\x047"},
	{key_end		,"end",	    2,	"\x0e0\x04f"},
	{key_pageup		,"pageup",	2,	"\x0e0\x049"},
	{key_page_down	,"page_down", 2,	"\x0e0\x051"},
	{key_insert		,"insert",	2,	"\x0e0\x052"},
	{key_delete		,"delete",	2,	"\x0e0\x053"},
	{key_enter		,"enter",	1,	"\x00d"},

	#else
	{key_pageup		,"pageup",	4,"\x01b\x05b\x035\x07e"},
	{key_insert		,"insert",	4,"\x01b\x05b\x032\x07e"},
	{key_delete		,"delete",	4,"\x01b\x05b\x033\x07e"},
	{key_home		,"home",	3,"\x01b\x05b\x048"},
	{key_end		,"end",	    3,"\x01b\x05b\x049"},
	{key_enter		,"enter",	1,	"\x00a"},

#endif
	{key_tab		,"tab",		1,	"\x009"},
	{key_ctrl_C		,"break",	1,	"\x003"},
	{key_esc		,"esc",		1,	"\x01b"},
	{key_alpha		,"alpha",	0,	"?"},
	{key_unknown	,"unknown",	0,	"?"}
};
const int num_keys= sizeof(keys)/sizeof(key_def);
class z_terminal 
{
protected:
	//Input buffer
	char *_pBuffer;
	U32 _buffSize;
	U32 _buffNext;
	U32 _buffEnd;

#ifdef BUILD_VSTUDIO
	HANDLE hStdout, hStdin; 
	HANDLE hConsoleOutput;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
#else

	struct termios term_original;
	struct termios term_no_wait;
	struct termios term_wait;
	int mode_normal();
	int set_no_wait();
	int set_wait();
	int set_no_echo();
	int get_resp(char*,char);

#endif
public:	
	char char_back;
	bool debug;

	//Cursor 
	U32 cur_x;
	U32 cur_y;
	char  BuffPeekChar(size_t index)
	{
		size_t x=index+_buffNext;
		if (x>=_buffSize)
			x=x-_buffSize;
		//if (x>=_buffEnd) return 0;
		return _pBuffer[x];
	}
	void  BuffAdvance(int index)
	{
		_buffNext+=index;
		if(_buffNext>=_buffSize) _buffNext-=_buffSize;
	}
	void  BuffAddChar(char c)
	{
		//gz_out.putf("%c %02x\r\n",c,c);
		_pBuffer[_buffEnd]=c;
		_buffEnd++;
		if(_buffEnd >=_buffSize) _buffEnd=0;
	}
	void  WaitForKey();
	size_t  BuffGetCount() 
	{ 
		if (_buffNext>_buffEnd)
		{
			return (_buffSize-_buffNext+_buffEnd);
		}
		return (_buffEnd-_buffNext);
	}
	//bool Init();
	void OutChar(char c);
	z_terminal()
	{
		_pBuffer=0;
	};
	virtual bool GetKey(enum_key& key,char &c);
	//virtual void GoHome();
	virtual void curGotoXY(U32 x,U32 y);
	virtual void curLeft(U32 x);
	virtual void curRight(U32 x);
	virtual void GetXY();
	//virtual void SetUnderscore();
	virtual void Close();
	virtual bool terminal_open();
	virtual ~z_terminal(){};

};



#endif
