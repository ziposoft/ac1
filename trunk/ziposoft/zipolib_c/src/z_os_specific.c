/*________________________________________________________________________

z_utility.c

________________________________________________________________________*/
#include "zipo.h"
#include "z_os_specific.h"
#include "z_temp_buff.h"

#ifdef OS_WINDOWS

WCHAR* WCHAR_str_allocate(const utf8 in_Src,size_t MaxLen)
{
	WCHAR* buff =(WCHAR*)z_temp_buffer_get( MaxLen);
	BOOL result=AnsiToUnicode16(in_Src, buff, MaxLen);
	if(result)
		return buff;

	WCHAR_str_deallocate(buff);
	return 0;


}
int WCHAR_str_deallocate(WCHAR* in)
{
	return z_temp_buffer_release((char*) in);
}

BOOL AnsiToUnicode16(utf8 in_Src, WCHAR *out_Dst, INT in_MaxLen)
{
	/* locals */
	INT lv_Len;

	// do NOT decrease maxlen for the eos
	if (in_MaxLen <= 0)
		return FALSE;

	// let windows find out the meaning of ansi
	// - the SrcLen=-1 triggers MBTWC to add a eos to Dst and fails if MaxLen is too small.
	// - if SrcLen is specified then no eos is added
	// - if (SrcLen+1) is specified then the eos IS added
	lv_Len = MultiByteToWideChar(CP_ACP, 0, in_Src, -1, out_Dst, in_MaxLen);

	// validate
	if (lv_Len < 0)
		lv_Len = 0;

	// ensure eos, watch out for a full buffersize
	// - if the buffer is full without an eos then clear the output like MBTWC does
	//   in case of too small outputbuffer
	// - unfortunately there is no way to let MBTWC return shortened strings,
	//   if the outputbuffer is too small then it fails completely
	if (lv_Len < in_MaxLen)
		out_Dst[lv_Len] = 0;
	else if (out_Dst[in_MaxLen-1])
		out_Dst[0] = 0;

	// done
	return TRUE;
}


BOOL AnsiToUnicode16L(utf8 in_Src, INT in_SrcLen, WCHAR *out_Dst, INT in_MaxLen)
{
	/* locals */
	INT lv_Len;


	// do NOT decrease maxlen for the eos
	if (in_MaxLen <= 0)
		return FALSE;

	// let windows find out the meaning of ansi
	// - the SrcLen=-1 triggers MBTWC to add a eos to Dst and fails if MaxLen is too small.
	// - if SrcLen is specified then no eos is added
	// - if (SrcLen+1) is specified then the eos IS added
	lv_Len = MultiByteToWideChar(CP_ACP, 0, in_Src, in_SrcLen, out_Dst, in_MaxLen);

	// validate
	if (lv_Len < 0)
		lv_Len = 0;

	// ensure eos, watch out for a full buffersize
	// - if the buffer is full without an eos then clear the output like MBTWC does
	//   in case of too small outputbuffer
	// - unfortunately there is no way to let MBTWC return shortened strings,
	//   if the outputbuffer is too small then it fails completely
	if (lv_Len < in_MaxLen)
		out_Dst[lv_Len] = 0;
	else if (out_Dst[in_MaxLen-1])
		out_Dst[0] = 0;

	// done
	return TRUE;
}
int UTF8_str_deallocate(utf8* in_src)
{
	free(in_src);
	return 0;
}

utf8 UTF8_str_allocate(WCHAR* in_src)
{
	utf8 out;

	int nLenUnicode = lstrlenW( in_src ); // Convert all UNICODE characters
	int nLen = WideCharToMultiByte( CP_ACP, // ANSI Code Page
		0, // No special handling of unmapped chars
		in_src, // wide-character string to be converted
		nLenUnicode,
		NULL, 0, // No output buffer since we are calculating length
		NULL, NULL ); // Unrepresented char replacement - Use Default 
	out = (utf8)malloc( nLen ); // nUserNameLen includes the NULL character
	WideCharToMultiByte( CP_ACP, // ANSI Code Page
		0, // No special handling of unmapped chars
		in_src, // wide-character string to be converted
		nLenUnicode,
		out, 
		nLen,
		NULL, NULL ); // Unrepresented char replacement - Use Default

	return out;

}



#endif
