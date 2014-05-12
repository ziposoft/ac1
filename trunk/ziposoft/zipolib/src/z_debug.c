/*

z_debug.c

*/
#include "zipo.h"
#include "zipolib/include/z_dbg.h"
#include "zipolib/include/z_util.h"
#include "zipolib/include/z_os_specific.h"
int g_z_dbg_trace_mask=0;
#define MAX_DEBUG_STR 0x1000
#if DEBUG

void z_debug_printf(const char*  lpszFormat,  ...  )
{
	static char tempbuf[MAX_DEBUG_STR];
	int c;
	va_list ap;
	va_start (ap, lpszFormat);
	c=vsnprintf (tempbuf,MAX_DEBUG_STR, lpszFormat, ap);
	va_end (ap);
	z_debug_out(tempbuf);


}

#ifdef WIN32

void z_debug_out(const char*  str  )
{
		WCHAR* w_str=WCHAR_str_allocate(str,MAX_DEBUG_STR);
		OutputDebugString(w_str);
		WCHAR_str_deallocate(w_str);

}

#include <crtdbg.h>
_CrtMemState s1, s2, s3;
void z_debug_startup()
{
    // Send all reports to STDOUT
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
    _CrtSetDbgFlag (
        _CRTDBG_ALLOC_MEM_DF |
        _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF |_CRTDBG_CHECK_CRT_DF    );
    _CrtSetReportMode ( _CRT_ERROR,
        _CRTDBG_MODE_DEBUG);
    _CrtMemCheckpoint( &s1 );



}
void z_debug_shutdown()
{
    _CrtMemCheckpoint( &s2 );
    if ( _CrtMemDifference( &s3, &s1, &s2 ) )
        _CrtMemDumpStatistics( &s3 );

}
#else
#include <stdarg.h>
#include <stdarg.h>
#include <stdio.h>
size_t g_z_dbg_out_file=0;

void z_debug_printf(const char*  lpszFormat,  ...  )
{
	static char tempbuf[0x1000];
	if(g_z_dbg_out_file)
	{
		int c;
		va_list ap;
		va_start (ap, lpszFormat);
		c=vsnprintf (tempbuf,z_temp_buffer_size(), lpszFormat, ap);
		va_end (ap);
		fwrite(tempbuf,1,c,(FILE*)g_z_dbg_out_file);	
		fflush(g_z_dbg_out_file);
	}

}
void z_debug_logfile(ctext filename)
{
	g_z_dbg_out_file=(size_t)(void*)fopen(filename,"wa");

}
void z_debug_shutdown()
{
	
}
#endif

#else

void z_debug_out(const char*  str  )
{


}

#endif

