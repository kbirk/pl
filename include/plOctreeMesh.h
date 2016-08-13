#pragma once

#include "plCommon.h"
#include "plMesh.h"
#include "plOctree.h"

class plOctreeMesh : public plMesh
{
    public:

        plOctreeMesh();
        plOctreeMesh(const std::vector<plTriangle> &triangles, uint32_t depth, bool verbose = false);
        plOctreeMesh(std::vector<plTriangle>&& triangles, uint32_t depth, bool verbose = false);

        plOctreeMesh(const plOctreeMesh &mesh);
        plOctreeMesh(plOctreeMesh&& mesh);

        plOctreeMesh& operator= (const plOctreeMesh& mesh);
        plOctreeMesh& operator= (plOctreeMesh&& mesh);

        const plOctree& octree() const { return _octree; }
        void toggleOctreeVisibility() { _octree.toggleVisibility(); }

        plVector3 getAverageNormal(float32_t radius, const plVector3 &origin, const plVector3 &up) const;

        plIntersection rayIntersect (const plVector3 &rayOrigin, const plVector3 &rayDirection,
                                      bool smoothNormal    = false,
                                      bool ignoreBehindRay = false,
                                      bool backFaceCull    = false) const;

    protected:

        plOctree _octree;

        void _buildOctree(uint32_t depth, bool verbose);

};
