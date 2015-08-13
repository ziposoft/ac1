/*

z_debug.c

*/
#include "zipo.h"
#include "zipolib/include/z_dbg.h"
#include "zipolib/include/z_util.h"
#include "zipolib/include/z_os_specific.h"
int g_z_dbg_trace_mask=0;
int gz_debug_break_on_error=0;







#define MAX_DEBUG_STR 0x1000
#if DEBUG


int z_debug_break()
{
	if(gz_debug_break_on_error)
	{
		Z_DEBUG_BREAK;
	}
	return 1;
}


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

#ifdef BUILD_VSTUDIO

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
void z_debug_out(const char*  str  )
{
	printf("%s",str);

}

#endif
#else //NOT DEBUG
void z_debug_out(const char*  str  )
{

}
int z_debug_break()
{
	return 1;
}

#endif

