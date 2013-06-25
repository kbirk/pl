#ifndef __PL_MATH_H__
#define __PL_MATH_H__

#include "pl.h"
#include "plVector3.h"

plVector3 plProjectVectorOnPlane( const plVector3 &vector, const plVector3 &plane_normal);
plVector3 plClosestPointOnLineSegment(const plVector3 &point, const plVector3 &a, const plVector3 &b);

#endif
