#ifndef PL_MESH_EXTRUDER_H
#define PL_MESH_EXTRUDER_H

#include "plTriangle.h"

#include "plMatrix44.h"

/*
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
*/

class plEdgePointers
{
    public:

        const plVector3 *p0;
        const plVector3 *p1;

        plEdgePointers() {}
        plEdgePointers( const plVector3 *pp0, const plVector3 *pp1 )
            : p0(pp0), p1(pp1)
        {}
};


namespace plMeshExtruder
{
    // returns an empty std::vector if there is an error or failure.
    // else returns an extruded version of the input mesh, in the direction and magnitude provided as inputs
    std::vector<plTriangle> extrudeMesh(const std::vector<plTriangle>& inputTriangles, const PLfloat magnitude, const plVector3& direction);
    std::vector<plTriangle> extrudeMesh(const std::vector<plTriangle>& inputTriangles, const PLfloat magnitude, const PLfloat preTranslation, const plVector3& direction);


}

#endif
