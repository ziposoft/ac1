//________________________________________________________________________/////////////////////////
//
// time header
//
//________________________________________________________________________/////////////////////////
#ifndef z_time_h
#define z_time_h
#include "zipolib_c/include/zipo.h"
#include <time.h>



#ifdef FREEBSD
#include <sys/time.h>
#endif
//________________________________________________________________________/////////////////////////
// C and C++

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
char * strptime (ctext buf, ctext format, struct tm *timeptr);
#define z_time U64
#else


#define z_time struct timespec


#endif 

void   z_set_start_time(z_time* time); //millisec
double z_get_elapsed_time_ms_fp(z_time* time); //millisec
//double z_get_elapsed_time_ms_fp(U64 time); //millisec
//U64    z_get_elapsed_time_ms_64(z_time* time); //millisec
///////////////////////////////////////////////////
//  z_get_elapsed_time_us_int
//  
//  Gets the elapsed time in micro-secs, returns an 64bit int
//
///////////////////////////////////////////////////
U64    z_get_elapsed_time_us_64(z_time* time); //microsec

size_t  z_make_date_time_stamp(char* sin,size_t size,time_t timein);

#ifdef	__cplusplus
}
#endif

#if 0

	void  z_append_date_time_stamp(z_string& s);


#endif
#endif


