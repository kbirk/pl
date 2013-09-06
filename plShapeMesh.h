#ifndef _PL_SHAPE_MESH_H_
#define _PL_SHAPE_MESH_H_ 

#include "plCommon.h"
#include "plMesh.h"

class plShapeMesh : public plMesh
{
    public:
		
        plShapeMesh(float halfWidth);                                                                // cube
        plShapeMesh(float radius, int slices, int stacks);                                           // sphere
        plShapeMesh(float baseRadius, float topRadius, float height, int slices, int stacks);        // cylinder
        plShapeMesh(float innerRadius, float outerRadius, int slices, int loops, bool up = false );  // disk             

};

#endif
