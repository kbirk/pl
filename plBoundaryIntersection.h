#ifndef _PL_BOUNDARY_INTERSECTION_H
#define _PL_BOUNDARY_INTERSECTION_H

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"
#include "plTriangle.h"
#include "plPolygon.h"
#include "plBoundary.h"
#include "plMath.h"


class plCut {
public:

  plCut() {}

  plCut( plVector3 pt, PLint ei, PLfloat ep, PLint bi, PLfloat bp, PLint dir ):
    point(pt), edgeIndex(ei), edgeParam(ep), boundaryIndex(bi), boundaryParam(bp), direction(dir)
    {}

  plVector3 point;          // int point
  PLint    edgeIndex;       // index of edge (0,1,2)
  PLfloat  edgeParam;       // param on triangle edge
  PLint    boundaryIndex;   // index of wall
  PLfloat  boundaryParam;   // param on wall
  PLint    direction;       // direction of edge: +1 = toward outside of wall, -1 = toward inside of wall
  PLbool   processed;
};

void            plFindInteriorMesh      ( plSeq<plTriangle> &triangles, plBoundary &walls, plSeq<plTriangle> &interiorTriangles );
static void     plUpdateInteriorPoints  ( plTriangle &triangle , plSeq<plVector3> &interiorPoints );
static PLbool   plEdgeCutsBoundary      ( const plVector3 &v0, const plVector3 &v1, plBoundary &wall, PLuint index, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &wallParam, PLint &intDir );
static void     plTriangleCutsBoundary  ( plTriangle &tri, PLbool &triProcessed, plBoundary &walls, plSeq<plPolygon> &polys, plSeq<plVector3> &interiorPoints );
static PLint    plCompareEdgeCuts       ( const void* a, const void* b );
static PLint    plCompareBoundaryCuts   ( const void* a, const void* b );



#endif // PLBOUNDARYINTERSECTION_H
