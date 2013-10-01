/*________________________________________________________________________

z_os_specific.h

________________________________________________________________________*/

#include "zipolib_c/include/zipo.h"
#ifndef z_windows_h
#define z_windows_h
#ifdef	__cplusplus
extern "C" {
#endif

#ifdef OS_WINDOWS

BOOL AnsiToUnicode16(utf8 in_Src, WCHAR *out_Dst, INT in_MaxLen);
BOOL AnsiToUnicode16L(utf8 in_Src, INT in_SrcLen, WCHAR *out_Dst, INT in_MaxLen);
int WCHAR_str_deallocate(WCHAR* in);
WCHAR* WCHAR_str_allocate(utf8 in_Src,size_t MaxLen);
#endif


#ifdef	__cplusplus
}
#endif
#endif

