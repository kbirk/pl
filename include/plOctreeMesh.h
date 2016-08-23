#pragma once

#include "plCommon.h"
#include "plMesh.h"
#include "plOctree.h"

class plOctreeMesh : public plMesh
{
    public:

        plOctreeMesh();
        plOctreeMesh(
            const std::vector<plTriangle>& triangles,
            uint32_t depth);
        plOctreeMesh(
            std::vector<plTriangle>&& triangles,
            uint32_t depth);

        void setTriangles(const std::vector<plTriangle>& triangles);
        void setTriangles(std::vector<plTriangle>&& triangles);
        void clear() { _triangles.clear(); _octree->clear(); }

        std::shared_ptr<plOctree> octree() const { return _octree; }
        void toggleOctreeVisibility() { _octree->toggleVisibility(); }

        plVector3 getAverageNormal(float32_t radius, const plVector3 &origin, const plVector3 &up) const;

        plIntersection rayIntersect(
            const plVector3 &rayOrigin, const plVector3 &rayDirection,
            bool smoothNormal = false,
            bool ignoreBehindRay = false,
            bool backFaceCull = false) const;

    protected:

        std::shared_ptr<plOctree> _octree;

        void _buildOctree(uint32_t depth);

};
