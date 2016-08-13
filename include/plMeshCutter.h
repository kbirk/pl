#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plMesh.h"
#include "plTriangle.h"
#include "plPolygon.h"
#include "plBoundary.h"
#include "plMath.h"

class plCut
{
    public:

        plCut() {}
        plCut(plVector3 pt, int32_t ei, float32_t ep, int32_t bi, float32_t bp, int32_t dir);

        plVector3 point;           // int32_t point
        int32_t   edgeIndex;       // index of edge (0,1,2)
        float32_t edgeParam;       // param on triangle edge
        int32_t   boundaryIndex;   // index of wall
        float32_t boundaryParam;   // param on wall
        int32_t   direction;       // direction of edge: +1 = toward outside of wall, -1 = toward inside of wall
        bool      processed;
};


namespace plMeshCutter
{
    bool findInteriorMesh(std::vector<plTriangle> &outputTriangles, const std::vector<plTriangle> &inputTriangles, const plBoundary &boundary);
}
