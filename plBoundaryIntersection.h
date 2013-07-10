#ifndef PLBOUNDARYINTERSECTION_H
#define PLBOUNDARYINTERSECTION_H

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"
#include "plTriangle.h"
#include "plPolygon.h"
#include "plBoundary.h"


class plCut {
public:

  plCut() {}

  plCut( plVector3 pp, PLint ei, PLfloat ep, PLint wi, PLfloat wp, PLint d ):
    p(pp), edgeIndex(ei), edgeParam(ep), wallIndex(wi), wallParam(wp), dir(d)
    {}

  plVector3 p;			// int point
  PLint    edgeIndex;		// index of edge (0,1,2)
  PLfloat  edgeParam;		// param on triangle edge
  PLint    wallIndex;		// index of wall
  PLfloat  wallParam;		// param on wall
  PLint    dir;			// direction of edge: +1 = toward outside of wall, -1 = toward inside of wall
  PLbool   processed;
};



void findInteriorMesh( plSeq<plTriangle> &triangles, plBoundary &walls, plSeq<plPolygon> &polygons );
PLbool edgeCutsWall( const plVector3 &v0, const plVector3 &v1, plBoundary &wall, PLuint index, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &wallParam, PLint &intDir );
void triangleCutsBoundary( plTriangle &tri, PLbool &triProcessed, plBoundary &walls, plSeq<plPolygon> &polys, plSeq<plVector3> &interiorPoints );
PLint compareEdgeCuts( const void* a, const void* b );
PLint compareWallCuts( const void* a, const void* b );






#endif // PLBOUNDARYINTERSECTION_H
