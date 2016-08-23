#include "plOctreeMesh.h"

plOctreeMesh::plOctreeMesh()
{
    _octree = std::make_shared<plOctree>();
}


plOctreeMesh::plOctreeMesh(const std::vector<plTriangle> &triangles, uint32_t depth)
    : plMesh(triangles)
{
    _octree = std::make_shared<plOctree>();
    _buildOctree(depth);
}


plOctreeMesh::plOctreeMesh(std::vector<plTriangle>&& triangles, uint32_t depth)
    : plMesh(std::move(triangles))
{
    _octree = std::make_shared<plOctree>();
    _buildOctree(depth);
}


void plOctreeMesh::setTriangles(const std::vector<plTriangle>& triangles)
{
    _triangles = triangles;
    _buildOctree(octree()->depth());
}


void plOctreeMesh::setTriangles(std::vector<plTriangle>&& triangles)
{
    _triangles = std::move(triangles);
    _buildOctree(octree()->depth());
}


void plOctreeMesh::_buildOctree(uint32_t depth)
{
    // get min and max extents of model
    plVector3 min, max;
    getMinMax(min, max);

    // build octree
    _octree->build(min, max, _triangles, depth);
}


plVector3 plOctreeMesh::getAverageNormal(float32_t radius, const plVector3 &origin, const plVector3 &normal) const
{
    // get potential triangles in radius from octree
    std::set<const plTriangle*> triangles;
    _octree->rayIntersect(triangles, origin, -normal, radius);

    plVector3 avgNormal(0, 0, 0);
    int32_t count = 0;
    float32_t radiusSquared = radius * radius;

    // Find polygons on top of graft
    for (const plTriangle* triangle : triangles)
    {
        if (triangle->normal() * normal > 0.001)
        {
            float32_t dist1 = (triangle->point0() - origin).squaredLength();
            float32_t dist2 = (triangle->point1() - origin).squaredLength();
            float32_t dist3 = (triangle->point2() - origin).squaredLength();

            // if any point of triangle is withing radial sphere, accept
            float32_t minDist = PL_MIN_OF_3(dist1, dist2, dist3);

            if (minDist <= radiusSquared)
            {
                avgNormal = avgNormal + triangle->normal();
                count++;
            }
        }
    }

    if (count == 0)
    {
        // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
        LOG_WARN("No normal found");
        return normal;
    }

    return (1.0f/ (float32_t)(count) * avgNormal).normalize();
}


plIntersection plOctreeMesh::rayIntersect(const plVector3 &rayOrigin, const plVector3 &rayDirection, bool smoothNormal, bool ignoreBehindRay, bool backFaceCull) const
{
    // get potential triangles from octree
    std::set<const plTriangle*> triangles;
    _octree->rayIntersect(triangles, rayOrigin, rayDirection, 0.0f, ignoreBehindRay);

    plIntersection closestIntersection(false);
    float32_t min = FLT_MAX;

    for (const plTriangle* tri : triangles)
    {
        // intersect triangle
        plIntersection intersection = tri->rayIntersect(rayOrigin, rayDirection, ignoreBehindRay, backFaceCull);
        if (intersection.exists)
        {
            float32_t tAbs = fabs(intersection.t);
            if (tAbs < min)
            {
                min = tAbs;
                closestIntersection = intersection;
            }
        }
    }

    // smooth intersection normal if specified
    if (smoothNormal)
        closestIntersection.normal = getAverageNormal(PL_NORMAL_SMOOTHING_RADIUS, closestIntersection.point, closestIntersection.normal);

    return closestIntersection;
}
