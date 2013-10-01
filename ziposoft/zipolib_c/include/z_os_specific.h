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


utf8 Unicode16ToAnsi(WCHAR * in_Src, utf8 out_Dst, INT in_MaxLen);

BOOL AnsiToUnicode16(utf8 in_Src, WCHAR *out_Dst, INT in_MaxLen);
BOOL AnsiToUnicode16L(utf8 in_Src, INT in_SrcLen, WCHAR *out_Dst, INT in_MaxLen);
int WCHAR_str_deallocate(WCHAR* in);

WCHAR* WCHAR_str_allocate(const utf8 in_Src,size_t MaxLen);
utf8 UTF8_str_allocate(WCHAR* in_src);

#endif


#ifdef	__cplusplus
}
#endif
#endif

