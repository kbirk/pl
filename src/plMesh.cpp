#include "plMesh.h"

plMesh::plMesh()
{
}


plMesh::plMesh(const std::vector<plTriangle> &triangles)
    : _triangles( triangles )
{
}


plMesh::plMesh( std::vector<plTriangle>&& triangles )
    : _triangles( std::move( triangles ) )
{
}


plMesh::plMesh( const plMesh &mesh )
    : _triangles( mesh._triangles )
{
}


plMesh::plMesh( plMesh&& mesh )
    : _triangles( std::move( mesh._triangles ) )
{
}


plMesh::~plMesh()
{
}


plMesh& plMesh::operator= ( const plMesh& mesh )
{
    _triangles = mesh._triangles;
    return *this;
}


plMesh& plMesh::operator= ( plMesh&& mesh )
{
    _triangles = std::move( mesh._triangles );
    return *this;
}


void plMesh::getMinMax(plVector3 &min, plVector3 &max) const
{
    min = plVector3( FLT_MAX, FLT_MAX, FLT_MAX );
    max = -1 * min;

    for ( const plTriangle& triangle : _triangles )
    {
        const plVector3 &v = triangle.centroid();

        if (v.x < min.x) min.x = v.x;
        if (v.y < min.y) min.y = v.y;
        if (v.z < min.z) min.z = v.z;

        if (v.x > max.x) max.x = v.x;
        if (v.y > max.y) max.y = v.y;
        if (v.z > max.z) max.z = v.z;
    }
}


plVector3 plMesh::getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &normal ) const
{
    return plMath::getAverageNormal( _triangles, radius, origin, normal );
}


plIntersection plMesh::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool smoothNormal, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    return plMath::rayIntersect( _triangles, rayOrigin, rayDirection, smoothNormal, ignoreBehindRay, backFaceCull );
}
