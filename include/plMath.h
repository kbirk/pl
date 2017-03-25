#pragma once

#include "plCommon.h"

class plVector3;
class plVector4;
class plMatrix44;
class plTriangle;
class plPolygon;
class plIntersection;

namespace plMath
{
    plVector3 projectVectorOnPlane(
        const plVector3& vector,
        const plVector3& plane_normal);

    plVector3 closestPointOnSegment(
        const plVector3& point,
        const plVector3& a,
        const plVector3& b);

    plVector3 closestPointOnLine(
        const plVector3& point,
        const plVector3& lineOrigin,
        const plVector3& lineDirection);

    float32_t clamp(float32_t val, float32_t min = -1, float32_t max = 1);

    void convexPolysToTris(
        std::vector<plTriangle>& tris,
        const std::vector<plPolygon>& polys);

    void concavePolysToTris(
        std::vector<plTriangle>& tris,
        const std::vector<plPolygon>& polys);

    plIntersection rayIntersect(
        const std::vector<plTriangle>& triangles,
        const plVector3& rayOrigin,
        const plVector3& rayDirection,
        bool smoothNormal = false,
        bool ignoreBehindRay = false,
        bool backFaceCull = false);

    plIntersection rayIntersect(
        const plVector3& rayOrigin,
        const plVector3& rayDirection,
        const plVector3& planePoint,
        const plVector3& planeNormal);

    plVector3 getAverageNormal(
        const std::vector<plTriangle>& triangles,
        float32_t radius,
        const plVector3& origin,
        const plVector3& normal);

    plIntersection getClosestPointToRay(
        const std::vector<plTriangle>& triangles,
        const plVector3& rayOrigin,
        const plVector3& rayDirection);

}
