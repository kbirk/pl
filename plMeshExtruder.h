#ifndef PLMESHEXTRUDER_H
#define PLMESHEXTRUDER_H

#include "plTriangle.h"
#include "plSeq.h"



// used in constructing the template base shape
class plEdgeIndices
{
    public:

        PLuint triangleIndex;
        PLuint vertexIndexA;
        PLuint vertexIndexB;

        plEdgeIndices() {}
        plEdgeIndices( PLuint t, PLuint vA, PLuint vB)
            : triangleIndex(t), vertexIndexA(vA), vertexIndexB(vB)
        {}

};

namespace plMeshExtruder
{
    plSeq<plTriangle> extrudeMesh(const plSeq<plTriangle>& inputTriangles, const PLfloat magnitude, const plVector3& direction);
}

#endif // PLMESHEXTRUDER_H
