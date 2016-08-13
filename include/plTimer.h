#pragma once

#include "plCommon.h"

typedef long long unsigned int  PLtime;

class plTimer
{
    public:

        static PLtime now();

    private:

        plTimer();

};
