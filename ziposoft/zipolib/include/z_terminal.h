#ifndef terminal_h
#define terminal_h
#include "zipolib/include/zipo.h"


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
#ifdef BUILD_VX
	#include <signal.h>
	#include <stdlib.h>
	#include <unistd.h>
void CatchSigs();
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



class z_terminal 
{


protected:
	//Input buffer
	char *_pBuffer;
	U32 _buffSize;
	U32 _buffNext;
	U32 _buffEnd;

	size_t _key_map_count;
	const key_def* _key_map;

#ifdef BUILD_VSTUDIO
	HANDLE hStdout, hStdin; 
	HANDLE hConsoleOutput;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo; 
#else
#ifdef UNIX
	struct termios term_original;
	struct termios term_no_wait;
	struct termios term_wait;
#endif	
	int mode_normal();
	int set_no_wait();
	int set_wait();
	int set_no_echo();
	int get_resp(char*,char);

#endif
public:	
	enum term_type { tt_vt100=0, tt_windows=1,tt_max };
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
		//zout.putf("%c %02x\r\n",c,c);
		_pBuffer[_buffEnd]=c;
		_buffEnd++;
		if(_buffEnd >=_buffSize) _buffEnd=0;
	}
	void  WaitForKey();
	size_t  BuffGetCount() ;
	//bool Init();
	void OutChar(char c);
	z_terminal();
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
	int set_key_map(term_type map);

};



#endif
