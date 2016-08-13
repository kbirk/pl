#ifndef PL_TIMER_H
#define PL_TIMER_H

#include "plCommon.h"

typedef long long unsigned int  PLtime;

class plTimer
{
    public:

        static PLtime now();

    private:

        plTimer();

};
#endif
