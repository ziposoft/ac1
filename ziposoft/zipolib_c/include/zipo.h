/*________________________________________________________________________

 ZIPO Lib main header

________________________________________________________________________*/

#ifndef zipo_h
#define zipo_h
#ifdef	__cplusplus
extern "C" {
#endif
/*________________________________________________________________________

#define BUILD_VSTUDIO
#define BUILD_INTEGRITY
#define BUILD_VXWORKS
#define BUILD_GCC
#define BUILD_MINGW


#define TARGET_X86
#define TARGET_X86_32
#define TARGET_X86_64
#define TARGET_PPC
________________________________________________________________________*/



/*________________________________________________________________________

 Basic defines 
________________________________________________________________________*/
typedef   unsigned long long U64;
typedef   long long I64;
typedef   unsigned int  U32;
typedef   unsigned short  U16;
typedef   unsigned char  U8;


typedef const char* utf8;
typedef char* ascii;
typedef const char* ctext;
typedef char const *STRPTR;
#  define LLCONST(a) (a##ll)

#define I64_MIN (LLCONST(-9223372036854775807)-1)
#define I64_MAX (LLCONST(9223372036854775807))
#define U64_MAX (LLCONST(18446744073709551615u))


#define Z_MAX_PATH_LENGTH 2048





/*________________________________________________________________________

Set the build type
________________________________________________________________________*/
#ifdef _MSC_VER 
#define BUILD_VSTUDIO
#elif __GNUC__
//#warning "__GNUC__ is defined, setting BUILD_GCC"
#define BUILD_GCC
#endif

#if !(defined BUILD_VXWORKS || defined BUILD_GCC || defined BUILD_VSTUDIO || defined BUILD_INTEGRITY)
#error "You must define BUILD_VXWORKS,BUILD_GCC,BUILD_VSTUDIO, or BUILD_INTEGRITY"
#endif

/*________________________________________________________________________

	Build Specific Options
________________________________________________________________________*/

#ifdef BUILD_INTEGRITY
#pragma ghs nowarning 228 //extra ;
#pragma ghs nowarning 381 //trailing comma 
//#pragma ghs nowarning 680 //unnamed member 
#pragma ghs nowarning 620 //unnamed member 
#endif

/*________________________________________________________________________*/

#ifdef BUILD_GCC
#define UNUSED  __attribute__ ((unused))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#endif
/*________________________________________________________________________*/

#ifdef BUILD_VSTUDIO
	#pragma warning (disable :4355)//warning C4355: 'this' : used in base member initializer list
	#pragma warning (disable :4800)//forcing value to bool 'true' or 'false' (performance warning)
	#define OS_WINDOWS 1
	#define UNUSED 
#ifdef INTEL_COMPILIER
	#pragma warning (disable :1684)//conversion from pointer to same-sized integral type (potential portability problem)
#endif 
	#define snprintf _snprintf
	#define strcasecmp stricmp
    #undef BREAKPOINT 
	#define BREAKPOINT _asm int 3;

	#ifdef _DEBUG
		#define DEBUG 1
	#endif
    #define WIN32_LEAN_AND_MEAN
    #define _CRT_SECURE_NO_DEPRECATE 1
    #define _CRT_NONSTDC_NO_DEPRECATE 1
    #ifdef DEBUG_MEM
        #include <stdlib.h>/* have to put this here or you get compile errors*/
        #define _CRTDBG_MAP_ALLOC
	   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)

        #include <crtdbg.h>
		#undef znew
		#define znew DEBUG_CLIENTBLOCK
	#else
    #endif
    #include "Windows_zipo.h"
	#include  <direct.h>

    #include <stdio.h>
    #include <stdlib.h>
    //#define snscanf _snscanf_s
	
	//#define vsnprintf _vsnprintf_s
	//#define fopen fopen_s

#else


#endif




/*________________________________________________________________________

	OS specific defines
_#define OS_WINDOWS
#define OS_UNIX
#define OS_LINUX
#define OS_INTEGRITY
#define OS_VXWORKS
_______________________________________________________________________*/



#ifdef OS_UNIX
#define Sleep(_x_) usleep(_x_*1000)

#include  <string.h>
#include  <errno.h>
#define MAX_PATH 2048
#include  <unistd.h>
#include  <dirent.h>
#include  <sys/stat.h>
#endif




#ifdef	__cplusplus
}
#endif
#endif