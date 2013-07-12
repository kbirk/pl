#ifndef __PL_MATH_H__
#define __PL_MATH_H__

#include <limits>

#include "pl.h"
#include "plVector3.h"
#include "plSeq.h"
#include "plPolygon.h"
#include "plTriangle.h"

plVector3 plProjectVectorOnPlane( const plVector3 &vector, const plVector3 &plane_normal);
plVector3 plClosestPointOnLineSegment(const plVector3 &point, const plVector3 &a, const plVector3 &b);
PLfloat   plClamp( PLfloat val, PLfloat min, PLfloat max);
void      plConvexPolysToTris(plSeq<plPolygon> &polys, plSeq<plTriangle> &tris);
void      plConcavePolysToTris(plSeq<plPolygon> &polys, plSeq<plTriangle> &tris);
void      plConcavePolysToTrisHelper(plPolygon&, plSeq<plTriangle>&, PLfloat&);

#endif
