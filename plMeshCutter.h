#ifndef _PL_MESH_CUTTER_H
#define _PL_MESH_CUTTER_H

#include "plCommon.h"
#include "plVector3.h"
#include "plSeq.h"
#include "plTriangle.h"
#include "plPolygon.h"
#include "plBoundary.h"
#include "plMath.h"

class plCut
{
    public:

        plCut() {}
        plCut( plVector3 pt, PLint ei, PLfloat ep, PLint bi, PLfloat bp, PLint dir );

        plVector3 point;           // int point
        PLint     edgeIndex;       // index of edge (0,1,2)
        PLfloat   edgeParam;       // param on triangle edge
        PLint     boundaryIndex;   // index of wall
        PLfloat   boundaryParam;   // param on wall
        PLint     direction;       // direction of edge: +1 = toward outside of wall, -1 = toward inside of wall
        PLbool    processed;
};

class plMeshCutter
{
    public:

        static PLbool  findInteriorMesh        ( const plSeq<plTriangle> &triangles, const plBoundary &walls, plSeq<plTriangle> &interiorTriangles );

    private:

        plMeshCutter() {};
               
        static void     _updateInteriorPoints  ( const plTriangle &triangle , plSeq<plVector3> &interiorPoints );
        static PLbool   _edgeCutsBoundary      ( const plVector3 &edgeVert0, const plVector3 &edgeVert1, const plBoundary &wall, PLuint index, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &wallParam, PLint &intDir );
        static PLbool   _triangleCutsBoundary  ( const plTriangle &tri, PLbool &triProcessed, const plBoundary &walls, plSeq<plPolygon> &polys, plSeq<plVector3> &interiorPoints );
        static PLint    _compareEdgeCuts       ( const void* a, const void* b );
        static PLint    _compareBoundaryCuts   ( const void* a, const void* b );
};



#endif 
