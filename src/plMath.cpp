#include "plMath.h"
#include "plMatrix44.h"
#include "plPolygon.h"
#include "plTriangle.h"
#include "plVector3.h"
#include "plVector4.h"

namespace plMath {

void _concavePolysToTris(
    std::vector<plTriangle>& triangles,
    float32_t& minSurfaceArea,
    const plPolygon& polygon);

plVector3 projectVectorOnPlane(
    const plVector3& vector,
    const plVector3& plane_normal)
{
    float32_t dist = vector * plane_normal;
    return vector - (dist * plane_normal);
}

plVector3 closestPointOnSegment(
    const plVector3& point,
    const plVector3& a,
    const plVector3& b)
{
    plVector3 ab = b - a;
    // Project c onto ab, computing parameterized position d(t) = a + t*(b * a)
    float32_t t = ((point - a) * ab) / (ab * ab);
    // If outside segment, clamp t (and therefore d) to the closest endpoint
    if (t < 0.0f)
        t = 0.0f;
    if (t > 1.0f)
        t = 1.0f;
    // Compute projected position from the clamped t
    return a + t * ab;
}

plVector3 closestPointOnLine(
    const plVector3& point,
    const plVector3& lineOrigin,
    const plVector3& lineDirection)
{
    // Project c onto ab, computing parameterized position d(t) = a + t*(b * a)
    float32_t t = ((point - lineOrigin) * lineDirection) / (lineDirection * lineDirection);
    // If outside segment, clamp t (and therefore d) to the closest endpoint
    // Compute projected position from the clamped t
    return lineOrigin + t * lineDirection;
}

float32_t clamp(float32_t val, float32_t min, float32_t max)
{
    if (val > max)
        return max;
    if (val < min)
        return min;
    return val;
}

plIntersection rayIntersect(
    const std::vector<plTriangle>& triangles,
    const plVector3& rayOrigin,
    const plVector3& rayDirection,
    bool smoothNormal,
    bool ignoreBehindRay,
    bool backFaceCull)
{
    float32_t min = FLT_MAX;
    plIntersection closestIntersection(false);

    for (const plTriangle& triangle : triangles) {
        plIntersection intersection = triangle.rayIntersect(rayOrigin, rayDirection, ignoreBehindRay, backFaceCull);
        if (intersection.exists) {
            float32_t tAbs = fabs(intersection.t);
            if (tAbs < min) {
                min = tAbs;
                closestIntersection = intersection;
            }
        }
    }

    if (closestIntersection.exists && smoothNormal) {
        closestIntersection.normal = plMath::getAverageNormal(triangles, PL_NORMAL_SMOOTHING_RADIUS, closestIntersection.point, closestIntersection.normal);
    }

    return closestIntersection;
}

plIntersection rayIntersect(
    const plVector3& rayOrigin,
    const plVector3& rayDirection,
    const plVector3& planePoint,
    const plVector3& planeNormal)
{
    if (planeNormal * rayDirection == 0)
        return plIntersection(false);

    // Compute the t value for the directed line ab intersecting the plane
    float32_t t = ((planePoint * planeNormal) - (planeNormal * rayOrigin)) / (planeNormal * rayDirection);
    plVector3 intPoint = rayOrigin + t * rayDirection;
    return plIntersection(intPoint, planeNormal, t);
}

plVector3 getAverageNormal(
    const std::vector<plTriangle>& triangles,
    float32_t radius,
    const plVector3& origin,
    const plVector3& normal)
{
    plVector3 avgNormal(0, 0, 0);
    int32_t count = 0;
    float32_t radiusSquared = radius * radius;

    // Find polygons on top of graft
    for (const plTriangle& triangle : triangles) {
        if (triangle.normal() * normal > 0.001) {
            float32_t dist1 = (triangle.point0() - origin).squaredLength();
            float32_t dist2 = (triangle.point1() - origin).squaredLength();
            float32_t dist3 = (triangle.point2() - origin).squaredLength();

            // if any point of triangle is withing radial sphere, accept
            float32_t minDist = PL_MIN_OF_3(dist1, dist2, dist3);

            if (minDist <= radiusSquared) {
                avgNormal = avgNormal + triangle.normal();
                count++;
            }
        }
    }

    if (count == 0) {
        // no triangles in radial sphere, just return previous normal
        LOG_WARN("No normal found");
        return normal;
    }

    return (1.0f / (float32_t)(count)*avgNormal).normalize();
}

plIntersection getClosestPointToRay(
    const std::vector<plTriangle>& triangles,
    const plVector3& rayOrigin,
    const plVector3& rayDirection)
{
    plIntersection intersection(false);

    float32_t lowestDist = FLT_MAX;

    for (const plTriangle& triangle : triangles) {
        // find closest point on ray from triangle centre
        plVector3 closestPointOnLine = plMath::closestPointOnLine(triangle.centroid(), rayOrigin, rayDirection);

        plVector3 closestPointOnTri = triangle.closestPointTo(closestPointOnLine);

        float32_t dist = (closestPointOnTri - closestPointOnLine).squaredLength();

        if (dist < lowestDist) {
            lowestDist = dist;
            intersection.exists = true;
            intersection.point = closestPointOnTri;
            intersection.normal = triangle.normal();
        }
    }

    return intersection;
}

void convexPolysToTris(
    std::vector<plTriangle>& tris,
    const std::vector<plPolygon>& polys)
{
    tris.clear();
    for (uint32_t i = 0; i < polys.size(); i++) {
        if (polys[i].points.size() > 2) // general case
        {
            plVector3 normal(polys[i].normal);
            plVector3 point0(polys[i].points[0]);
            // create a fan of triangles from this point
            for (uint32_t currentVertex = 1; currentVertex < polys[i].points.size() - 1; currentVertex++) {
                plVector3 point1(polys[i].points[currentVertex]);
                plVector3 point2(polys[i].points[currentVertex + 1]);
                plTriangle tri(normal, point0, point1, point2);
                tris.push_back(tri);
            }
        } else // must be an invalid polygon
        {
            LOG_WARN("Polygon contains less than three vertices");
        }
    }
}

void concavePolysToTris(
    std::vector<plTriangle>& tris,
    const std::vector<plPolygon>& polys)
{
    tris.clear();
    for (uint32_t i = 0; i < polys.size(); i++) {
        std::vector<plTriangle> polyConverted;
        float32_t area = FLT_MAX;
        _concavePolysToTris(polyConverted, area, polys[i]);
        for (uint32_t j = 0; j < polyConverted.size(); j++) {
            tris.push_back(polyConverted[j]);
        }
    }
}

void _concavePolysToTris(
    std::vector<plTriangle>& triangles, // should be empty
    float32_t& minSurfaceArea, // should be FLT_MAX
    const plPolygon& polygon)
{
    /**
         * Take a polygon as input.
         * Iterate through each possible triangle involving the vertices at
         * indices 0 and 1 (0-1-2, 0-1-3, and so on to 0-1-N). There will be two
         * polygons remaining:
         *  - the one from index 1 to X
         *  - the one from index X to 0
         * Recursively call this function for each of these smaller polygons
         * store only the sequence of triangles that results in the smallest
         * surface area also store the surface area of the triangulation
         */
    if (polygon.points.size() <= 2) {
        // base case
        minSurfaceArea = 0.0f;
        return;
    }

    // three points lie on a line
    if (polygon.points.size() == 3 && (polygon.points[2] ^ polygon.points[0]).length() <= PL_EPSILON && (polygon.points[2] - polygon.points[1]).length() <= PL_EPSILON) {
        minSurfaceArea = FLT_MAX / 2.0f;
        return;
    }

    // try all permutations of triangles involving the edge between
    // points[0] and points[1]
    for (uint32_t i = 2; i < polygon.points.size(); i++) {
        plTriangle bisectingTriangle(
            polygon.points[0],
            polygon.points[1],
            polygon.points[i]);

        float32_t bisectingTriangleArea = bisectingTriangle.getArea();

        if (bisectingTriangleArea <= PL_EPSILON) {
            // bad triangle
            continue;
        }

        // first polygon
        plPolygon poly0;
        for (uint32_t j = 1; j <= i; j++) {
            poly0.points.push_back(polygon.points[j]);
        }
        std::vector<plTriangle> poly0Tris;
        float32_t poly0Area = FLT_MAX;
        _concavePolysToTris(poly0Tris, poly0Area, poly0);

        // second polygon
        plPolygon poly1;
        poly1.points.push_back(polygon.points[0]);
        for (uint32_t j = i; j < polygon.points.size(); j++) {
            poly1.points.push_back(polygon.points[j]);
        }
        std::vector<plTriangle> poly1Tris;
        float32_t poly1Area = FLT_MAX;
        _concavePolysToTris(poly1Tris, poly1Area, poly1);

        // calculate current surface
        float32_t currentSurfaceArea = bisectingTriangleArea + poly0Area + poly1Area;

        // update the output if necessary
        if (currentSurfaceArea < minSurfaceArea) {
            triangles.clear();
            triangles.push_back(bisectingTriangle);
            for (uint32_t j = 0; j < poly0Tris.size(); j++) {
                triangles.push_back(poly0Tris[j]);
            }
            for (uint32_t j = 0; j < poly1Tris.size(); j++) {
                triangles.push_back(poly1Tris[j]);
            }
            minSurfaceArea = currentSurfaceArea;
        }
    }
}
}
