#pragma once

#include "plCommon.h"
#include "plTriangle.h"
#include "plMesh.h"
#include "plMatrix44.h"


class plEdgePointers
{
    public:

        const plVector3 *p0;
        const plVector3 *p1;

        plEdgePointers() {}
        plEdgePointers(const plVector3 *pp0, const plVector3 *pp1)
            : p0(pp0), p1(pp1)
        {}
};


namespace plMeshExtruder
{
    plMesh extrudeMesh(const plMesh& mesh, const PLfloat magnitude, const plVector3& direction);
}
