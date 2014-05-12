/*

z_time.c

*/
#include "zipo.h"
#include "zipolib/include/z_time.h"





#ifdef BUILD_VSTUDIO


void z_set_start_time(z_time* time)
{
    LARGE_INTEGER start_time;
	QueryPerformanceCounter(&start_time);
    *time=start_time.QuadPart;
}
///////////////////////////////////////////////////
//  z_get_elapsed_time_us_int
//  
//  Gets the elapsed time in micro-secs, returns a double
//
///////////////////////////////////////////////////
double z_get_elapsed_time_ms_fp(z_time* start_time) //millisec
{
	static LARGE_INTEGER freq={0,0};
	double dt;
	U64 elapsed_tics=z_get_elapsed_time_us_64(start_time);
	if(!(freq.QuadPart))
		QueryPerformanceFrequency(&freq);
	dt=(double)elapsed_tics;
	dt=dt*1000;
	dt=dt/freq.QuadPart;
	return dt; //millisec

}
///////////////////////////////////////////////////
//  z_get_elapsed_time_us_int
//  
//  Gets the elapsed time in micro-secs, returns a 64bit int
//
///////////////////////////////////////////////////
U64    z_get_elapsed_time_us_64(U64* start_time)
{
    LARGE_INTEGER time;
    U64 elapsed_tics;
	QueryPerformanceCounter(&time);
	elapsed_tics=time.QuadPart-*start_time;
	return elapsed_tics;
}
#else
#ifdef CLOCK_REALTIME


void z_set_start_time(z_time* time)
{
    clock_gettime(CLOCK_REALTIME, time);

}
double z_get_elapsed_time_ms_fp(z_time* start_time)
{
    long    us;
    long    s;
    long    ms;
	double    t;
	struct timespec time2;
    clock_gettime(CLOCK_REALTIME, &time2);
	us=time2.tv_nsec/1000;
	us=us-start_time->tv_nsec/1000;
	t=us;
	t=t/1000; //ms
    s=time2.tv_sec-start_time->tv_sec;
	ms=s*1000;
	t=ms+t;
	return t;
}
U64    z_get_elapsed_time_us_64(z_time* start_time)
{
    U64    ns;
    U64    s;
    U64    us;
	struct timespec time2;
    clock_gettime(CLOCK_REALTIME, &time2);
    ns=time2.tv_nsec-start_time->tv_nsec;
    s=time2.tv_sec-start_time->tv_sec;
	us=ns;
	us=us/1000;
	us+=s*1000000;
	return (U64) us;
}
#else
#ifdef PPC_LINUX
uint64 time1;

#define CLKRATE		(133333333/4)

static inline uint64 z_get_timestamp(void)
{
	uint64        now;
	unsigned long tmp;
	unsigned long low;
	unsigned long high;   
 
	asm volatile ("0: mftbu %0;"
	              "   mftb  %1;"
	              "   mftbu %2;"
	              "   cmpw  %2,%0;"
	              "   bne 0b"
	             :"=r" (high), "=r" (low), "=r" (tmp));

	now = high;
	now <<= 32;
	now += low;

	return now;
}

static inline uint64 z_ticks_to_msecs (uint64 ticks)
{
	return ((ticks * 1000) / CLKRATE);
}
void z_set_start_time()
{
    time1=get_timestamp();
}
long z_get_elapsed_time()
{
	uint64 t;
	uint64 time2=get_timestamp();
	t=time2-time1;
	return ticks_to_msecs(t);
}
#else
void z_set_start_time(z_time* time)
{
    
	
}
double z_get_elapsed_time_ms_fp(z_time* start_time)
{

	return 0;
}
U64    z_get_elapsed_time_us_64(z_time* start_time)
{
	return 0;

}
#endif //PPC_LINUX
#endif
#endif


///////////////////////////////////////////////////
//  z_make_date_time_str
//  
//  user must delete returned char!
//
///////////////////////////////////////////////////
size_t  z_make_date_time_stamp(char* sin,size_t size,time_t timein)
{
	struct tm   *t;
	t=localtime(&timein);
	return strftime(sin,size,"%y.%m.%d.%H.%M.%S",t);
}

