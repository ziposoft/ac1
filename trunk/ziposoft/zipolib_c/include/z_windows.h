/*________________________________________________________________________

z_windows.h

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


WCHAR* Allocate_WCHAR_str(utf8 in,size_t MaxLen);
#endif


#ifdef	__cplusplus
}
#endif
#endif

