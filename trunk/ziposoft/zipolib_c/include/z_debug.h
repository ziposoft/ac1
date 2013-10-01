//________________________________________________________________________/////////////////////////
//
// ZIPO Lib dbg
//
//________________________________________________________________________/////////////////////////
#ifndef zipo_dbg_h
#define zipo_dbg_h
#include "zipo.h"
#ifdef	__cplusplus
extern "C" {
#endif


void z_debug_load_save_args(int* pargc, char*** pargv);
//DEBUG MACROS
#ifdef DEBUG
void z_debug_logfile(ctext name);

#ifdef BUILD_VSTUDIO  

#define Z_DEBUG_BREAK  __debugbreak();
#else
#define Z_DEBUG_BREAK
#endif


void z_debug_out(const char*  lpszFormat,  ...  );

#define	DBG_OUT(_X_)   z_debug_out _X_ ; 
#if WIN32
#define Z_ASSERT(_X_) { if (!(_X_)) {  DBG_OUT(("ASSERTION FAILED _X_ %s(%d) ",__FILE__,__LINE__));Z_DEBUG_BREAK;}}
#define Z_CHECK(_X_) _X_
#else//LINUX


#define Z_ASSERT(_X_)
#define Z_CHECK(_X_) _X_
#endif

//#define Z_ASSERT(_X_) 
int z_check(int exp);
void z_debug_startup();
void z_debug_shutdown();

#else//NO DEBUG

#define Z_DEBUG_BREAK
#define Z_ASSERT(_x_) 
#define	DBG_OUT(_X_)
#define	z_debug_shutdown(_X_)
#define	z_debug_startup(_X_)
#define	z_debug_logfile(_X_)


#endif//NO DEBUG

#ifdef	__cplusplus
}
#endif
#endif //zipo_dbg_h

