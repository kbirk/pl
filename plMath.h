#ifndef __PL_MATH_H__
#define __PL_MATH_H__

#include "plCommon.h"
#include "plVector3.h"

class plMath
{
    public:

        static plVector3 projectVectorOnPlane  ( const plVector3 &vector, const plVector3 &plane_normal);
        static plVector3 closestPointOnSegment ( const plVector3 &point, const plVector3 &a, const plVector3 &b);
        static PLfloat   clamp                 ( PLfloat val, PLfloat min = -1, PLfloat max = 1);

    private:

        plMath() {};
};
#endif
