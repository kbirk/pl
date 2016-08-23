#pragma once

#include "plCommon.h"

#include "plVector3.h"
#include "plMath.h"
#include "plTriangle.h"
#include "plRenderable.h"
#include "plTransform.h"
#include "plVAO.h"
#include "plRenderer.h"

// children quadrant indices:
//   [0] = - - -
//   [1] = + - -
//   [2] = - + -
//   [3] = + + -
//   [4] = - - +
//   [5] = + - +
//   [6] = - + +
//   [7] = + + +

class plOctree : public plRenderable
{

    public:

        plOctree();
        plOctree(
            const plVector3& centre,
            float32_t halfWidth,
            uint32_t depth);
        plOctree(
            const plVector3& min,
            const plVector3& max,
            const std::vector<plTriangle>& triangles,
            uint32_t depth);

        virtual ~plOctree();

        uint32_t depth() const { return _depth; }

        void build(
            const plVector3& min,
            const plVector3& max,
            const std::vector<plTriangle>& triangles,
            uint32_t depth);
        void clear();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        bool rayIntersect(
            std::set<const plTriangle*>& triangles,
            const plVector3& rayOrigin,
            const plVector3& rayDirection,
            float32_t rayRadius = 0.0f,
            bool ignoreBehindRay = false) const;

        void toggleVisibility();

    private:

        uint32_t _depth;
        plVector3 _centre; // center point of octree node (not strictly needed)
        float32_t _halfWidth; // half the width of the node volume (not strictly needed)
        std::vector<std::shared_ptr<plOctree>> _children; // pointers to the eight children nodes
        std::vector<const plTriangle*> _contained; // triangles contained at this node

        void _insert(const plTriangle& tri);
        void _insertIntoChild(uint32_t index, const plTriangle& tri);
        float32_t _sqrDistFromPoint(const plVector3& point, int32_t child) const;
        plVector3 _closestPointInBox(const plVector3& point, int32_t child) const;
        bool _sphereCheck(const plVector3& centre, float32_t radius, int32_t child) const;

        std::shared_ptr<plVAO> _generateVAO(float32_t halfWidth) const;

        void _move(plOctree &&octree);
        void _copy(const plOctree& octree);

};
