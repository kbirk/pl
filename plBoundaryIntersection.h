#ifndef PLBOUNDARYINTERSECTION_H
#define PLBOUNDARYINTERSECTION_H

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"

class plPolygon {
 public:

  plPolygon() {}

  plPolygon( plVector3 p1, plVector3 p2, plVector3 p3, plVector3 nn ) {
    ps.add(p1);
    ps.add(p2);
    ps.add(p3);
    n = nn;
  }

  plVector3 n;

  plSeq<plVector3> ps;

  PLbool processed;
};


class plWall {
 public:

  plVector3 p0, p1;		// two points on surface at opposite ends of wall
  plVector3 n0, n1;		// surface normals at p0 and p1
  plVector3 n;			// outward pointing normal of wall
  PLfloat  d;			// scalar in plane equation of wall

  plWall() {}

  plWall( plVector3 pp0, plVector3 pp1, plVector3 nn0, plVector3 nn1, plVector3 nn, PLfloat dd ):
    p0(pp0), p1(pp1), n0(nn0), n1(nn1), n(nn), d(dd)
    {}
};


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



PLbool edgeCutsWall( plVector3 &v0, plVector3 &v1, plWall &wall, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &wallParam, PLint &intDir );
void triangleCutsBoundary( plPolygon &tri,  plSeq<plWall> &walls, plSeq<plPolygon> &polys, plSeq<plVector3> &interiorPoints );
PLint compareEdgeCuts( const void* a, const void* b );
PLint compareWallCuts( const void* a, const void* b );
void findInteriorMesh( plSeq<plPolygon> &triangles, plSeq<plWall> &walls, plSeq<plPolygon> &polygons );






#endif // PLBOUNDARYINTERSECTION_H
