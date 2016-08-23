#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plMath.h"

class plMesh
{
    public:

        plMesh();
        plMesh(const std::vector<plTriangle> &triangles);
        plMesh(std::vector<plTriangle>&& triangles);
        plMesh(const plMesh &mesh);
        plMesh(plMesh&& mesh);

        virtual ~plMesh();

        plMesh& operator= (const plMesh& mesh);
        plMesh& operator= (plMesh&& mesh);

        virtual void setTriangles(const std::vector<plTriangle>& triangles);
        virtual void setTriangles(std::vector<plTriangle>&& triangles);
        virtual void clear() { _triangles.clear(); }

        const std::vector<plTriangle>& triangles() const { return _triangles; }

        virtual void getMinMax(plVector3& min, plVector3& max) const;

        virtual plVector3 getAverageNormal(
            float32_t radius,
            const plVector3& origin,
            const plVector3& up) const;

        virtual plIntersection rayIntersect(
            const plVector3 &rayOrigin, const plVector3 &rayDirection,
            bool smoothNormal = false,
            bool ignoreBehindRay = false,
            bool backFaceCull = false) const;

    protected:

        std::vector<plTriangle> _triangles;

};
