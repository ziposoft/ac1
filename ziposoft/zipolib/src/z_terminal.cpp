#include "zipolib_cpp_pch.h"
#include "z_terminal.h"
#include "z_file.h"
#include "zipolib/include/z_os_specific.h"


#ifdef UNIX
#include <stdio.h>
#endif
#ifdef BUILD_VX
#include <vxWorks.h>
#include <ctype.h>
#include <fioLib.h>
#include <ioLib.h>
#endif

z_terminal::z_terminal()
{
	_pBuffer=0;
#ifdef WIN32
	set_key_map(tt_windows);
#else
	set_key_map(tt_vt100);
#endif
};

size_t  z_terminal::BuffGetCount() 
{ 
	if (_buffNext>_buffEnd)
	{
		return (_buffSize-_buffNext+_buffEnd);
	}
	return (_buffEnd-_buffNext);
}
bool z_terminal::terminal_open()
{
	debug=false;
#ifdef WIN32
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
#ifdef UNIX
	if(tcgetattr(fileno(stdin), &term_original) < 0)
		return(-1);
	term_wait = term_original;
	term_wait.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);



	term_no_wait = term_wait;
	term_no_wait.c_cc[VMIN] = 0;
	term_no_wait.c_cc[VTIME] = 0;

	char_back=term_no_wait.c_cc[VERASE];
#endif
	_buffSize=0x40;
	_pBuffer=new char[_buffSize];
	_buffNext=0;
	_buffEnd=0;
	GetXY();
	return(0);

}
void z_terminal::OutChar(char c)
{




}

void z_terminal::Close()
{




}
bool z_terminal::GetKey(enum_key& key,char &c)
{
	
	size_t i,j;
	size_t count;
	bool match=false;
	key=key_alpha;
	if(!_pBuffer){
		terminal_open();	
		//return false; //not open?
	}
	count=BuffGetCount();
	if(!count) 	WaitForKey();
	if(debug) 
	{
		printf("\nkey:");
		for(j=0;j<BuffGetCount();j++)
			printf("%02x ",BuffPeekChar(j));
		printf("\n");
	}

	for(i=0;i<_key_map_count;i++)
	{
		if(_key_map[i].type==key_alpha) 
		{
			c=BuffPeekChar(0);
			BuffAdvance(1);
			if((c>=0x20)&&(c<=0x7e)) 
			{
				
				key=key_alpha;
				return true;
			}
			key=key_unknown;
			return false;
		}
		if((i==0)&&(BuffPeekChar(0)==char_back))
		{
			match=true;
			break;
		}
		if(_key_map[i].length<=BuffGetCount())
		{
			match=true;
			for(j=0;j<_key_map[i].length;j++)
			{
				if(_key_map[i].code[j]!=BuffPeekChar(j)) 
				{
					match=false;
					break;
				}
			}
		}
		if(match) break;
	}
	if(match)
	{
		if(debug)
        {
			printf("%s\r\n",_key_map[i].name);
        }
		key=_key_map[i].type;
		BuffAdvance(_key_map[i].length);
		return true;
	}
	return false;
}





////////////////////////////////////////////////////////////
// WIN32 stuff
////////////////////////////////////////////////////////////
#ifdef WIN32

void z_terminal::GetXY()
{
	BOOL b=GetConsoleScreenBufferInfo(hStdout,&csbiInfo);
	if(b)
	{
		cur_x=csbiInfo.dwCursorPosition.X;
		cur_y=csbiInfo.dwCursorPosition.Y;
	}
	else
	{
		PrintWin32Error();
	}

}
void z_terminal::curGotoXY(U32 x,U32 y)
{
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(hStdout,coord);
}
void z_terminal::curLeft(U32 x)
{
	cur_x=cur_x-x;
	if(cur_x<0) cur_x=0;
	curGotoXY(cur_x,cur_y);
}
void z_terminal::curRight(U32 x)
{
	cur_x=cur_x+x;
	curGotoXY(cur_x,cur_y);
}

void z_terminal::WaitForKey()
{
	BuffAddChar(getch());
	while (_kbhit())
	{
		BuffAddChar(getch());
	}
}
#else
////////////////////////////////////////////////////////////
// UNIX stuff
////////////////////////////////////////////////////////////
void z_terminal::WaitForKey()
{
	int i;
	char c;
	set_wait();
	c=fgetc(stdin);
	BuffAddChar(c);
	if(c==0x1b)
	{
		i=2;
		while(i--)
		{
			
			c=fgetc(stdin);
	    	BuffAddChar(c);
		}
	}
	
	mode_normal();
}
int z_terminal::get_resp(char* buff,char token)
{
	int n=0;
	char c=0;
	while(c!=token)
	{
		c=fgetc(stdin);
		//i=read(fileno(stdin), &c, 1);

		buff[n]=c;
		n++;
		if(n>5)
			break;
	}
	buff[n]=0;
	mode_normal();
	return 0;
}
void z_terminal::GetXY()
{
	//<ESC>[6n
	set_wait();

	printf("\x01b[6n");
	//tcdrain(1);

	char buff[20];
	get_resp(buff,'R');
	sscanf(buff+1,"[%d;%dR",&cur_y,&cur_x);
	cur_x--;


}

void z_terminal::curGotoXY(U32 x,U32 y)
{
	//<ESC>[{ROW};{COLUMN}f
	
	gz_out.putf("\x01b[%d;%df",y,x+1);
	//tcdrain(1);
}
void z_terminal::curLeft(U32 x)
{
	//<ESC>[{ROW};{COLUMN}f
	printf("\x01b[%dD",x);
	cur_x-=x;
	//tcdrain(1);
}
void z_terminal::curRight(U32 x)
{
	//<ESC>[{ROW};{COLUMN}f
	printf("\x01b[%dC",x);
	cur_x+=x;
	//tcdrain(1);
}

/*
void z_terminal::SetUnderscore()
{
	//<ESC>[ {Ps} ; {Ps} m
	gz_out.putf("\x01b[0m");
}
*/
int z_terminal::mode_normal()
{
	int result=0;
#ifdef LYNXOS
	result=tcsetattr(fileno(stdin), TCSAFLUSH, &term_original);
#endif
#ifdef UNIX
	result=tcsetattr(fileno(stdin), TCSANOW, &term_original);
#endif
#ifdef BUILD_VX
			(void) ioctl (STD_IN, FIOSETOPTIONS, OPT_TERMINAL);
#endif
	if(result < 0) 	return(-1);
	return(0);
}
int z_terminal::set_no_wait()
{
	int result=0;
#ifdef LYNXOS
	result=tcsetattr(fileno(stdin), TCSANOW, &term_no_wait);
#endif
#ifdef UNIX
	result=tcsetattr(0, TCSANOW, &term_no_wait);
#endif
#ifdef BUILD_VX
	    (void) ioctl (STD_IN, FIOSETOPTIONS,  /*OPT_ECHO |*/ OPT_CRMOD | OPT_TANDEM | OPT_7_BIT);
#endif
	if(result < 0) 	return(-1);
	return(0);
}
int z_terminal::set_wait()
{
	int result=0;
#ifdef LYNXOS
	result=tcsetattr(0, TCSANOW , &term_wait);
#endif
#ifdef UNIX
	result=tcsetattr(0, TCSANOW, &term_wait);
#endif
#ifdef BUILD_VX
	    (void) ioctl (STD_IN, FIOSETOPTIONS, /*OPT_ECHO |*/   OPT_CRMOD | OPT_TANDEM | OPT_7_BIT);
#endif
	if(result < 0) 	return(-1);
	return(0);
}	

////////////////////////////////////////////////////////////
// Unused stuff
////////////////////////////////////////////////////////////
#if 0
void termsigcatch(int sig)
{
	term_unix->ttyreset(0);
	exit(0);
}

void CatchSigs()
{
	
	if((int) signal(SIGINT,termsigcatch) < 0)
	{
		perror("signal");
		exit(1);
	}
	if((int)signal(SIGQUIT,termsigcatch) < 0)
	{
		perror("signal");
		exit(1);
	}
	if((int) signal(SIGTERM,termsigcatch) < 0)
	{
		perror("signal");
		exit(1);
	}
}
#endif
#endif
