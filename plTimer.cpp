#include "plTimer.h"


PLtime plTimer::now()
{
#ifdef WIN32
	// return elapsed time in seconds
	LARGE_INTEGER WIN32StopTime;
    LARGE_INTEGER WIN32Frequency;
    QueryPerformanceFrequency( &WIN32Frequency );	// cycles per second
	QueryPerformanceCounter( &WIN32StopTime );
    return PLtime( ( WIN32StopTime.QuadPart / double( WIN32Frequency.QuadPart ) ) * 1000.0 );
#else
    typedef std::chrono::high_resolution_clock         plClock;
    typedef std::chrono::milliseconds                  plMilliseconds;                
    return std::chrono::duration_cast<plMilliseconds>( plClock::now().time_since_epoch() ).count();
#endif
}
