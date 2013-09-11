#ifndef __PL_MATH_H__
#define __PL_MATH_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"
#include "plPolygon.h"
#include "plTriangle.h"

namespace plMath
{

    plVector3 projectVectorOnPlane  ( const plVector3 &vector, const plVector3 &plane_normal);
    plVector3 closestPointOnSegment ( const plVector3 &point, const plVector3 &a, const plVector3 &b);
    PLfloat   fsqrt                 ( PLfloat x );
    PLfloat   clamp                 ( PLfloat val, PLfloat min = -1, PLfloat max = 1);

    plIntersection rayIntersect     ( const plSeq<plTriangle> &triangles, 
                                      const plVector3 &rayOrigin, 
                                      const plVector3 &rayDirection, 
                                      PLbool ignoreBehindRay = false, 
                                      PLbool backFaceCull    = false );

	void convexPolysToTris         ( plSeq<plTriangle> &tris, const plSeq<plPolygon> &polys );
	void concavePolysToTris        ( plSeq<plTriangle> &tris, const plSeq<plPolygon> &polys );

};

#endif

