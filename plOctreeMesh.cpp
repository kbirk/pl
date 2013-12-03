#include "plOctreeMesh.h"

plOctreeMesh::plOctreeMesh() 
{
}


plOctreeMesh::plOctreeMesh( const std::vector<plTriangle> &triangles, PLuint depth, PLbool verbose )
    : plMesh( triangles )
{
    _buildOctree( depth, verbose );
}

		 
plOctreeMesh::plOctreeMesh( std::vector<plTriangle>&& triangles, PLuint depth, PLbool verbose )
    : plMesh( std::move( triangles ) )
{ 
    _buildOctree( depth, verbose );
} 

           
plOctreeMesh::plOctreeMesh( const plOctreeMesh &mesh )
    : plMesh( mesh._triangles )
{
    _buildOctree( mesh.octree().depth(), false );  // must build a new octree to maintain proper coherency
}


plOctreeMesh::plOctreeMesh( plOctreeMesh&& mesh )
    : plMesh( std::move( mesh._triangles ) ), _octree( std::move( mesh._octree ) )
{
}      


plOctreeMesh& plOctreeMesh::operator= ( const plOctreeMesh& mesh ) 
{ 
    _triangles = mesh._triangles;
    _buildOctree( mesh.octree().depth(), false );  // must build a new octree to maintain proper coherency
    return *this;
}


plOctreeMesh& plOctreeMesh::operator= ( plOctreeMesh&& mesh ) 
{ 
    _triangles = std::move( mesh._triangles );
    _octree    = std::move( mesh._octree );
    return *this;
}


void plOctreeMesh::_buildOctree( PLuint depth, PLbool verbose )
{
    // get min and max extents of model
    plVector3 min, max;
    getMinMax( min, max );

    // build octree
    _octree.build( min, max, _triangles, depth, verbose ); 
}


plVector3 plOctreeMesh::getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &normal ) const
{
    // get potential triangles in radius from octree
    std::set< const plTriangle* > triangles;   
    _octree.rayIntersect( triangles, origin, -normal, radius );

    plVector3 avgNormal(0,0,0);
    PLint count = 0;
    float radiusSquared = radius * radius;
    
    // Find polygons on top of graft
    for ( const plTriangle* triangle : triangles ) 
    {
        if ( triangle->normal() * normal > 0.001)
        {        
            PLfloat dist1 = ( triangle->point0() - origin ).squaredLength();
            PLfloat dist2 = ( triangle->point1() - origin ).squaredLength();
            PLfloat dist3 = ( triangle->point2() - origin ).squaredLength();
           
            // if any point of triangle is withing radial sphere, accept
            float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

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
        //std::cout << "plOctreeMesh::getAverageNormal() warning: No normal found" << std::endl;
        return normal;
    }    

    return ( 1.0f/ (PLfloat)(count) * avgNormal ).normalize();
}


plIntersection plOctreeMesh::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool smoothNormal, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    // get potential triangles from octree
    std::set< const plTriangle* > triangles;   
    _octree.rayIntersect( triangles, rayOrigin, rayDirection, 0.0f, ignoreBehindRay );

    plIntersection closestIntersection( false );
    PLfloat min = FLT_MAX;
    
    for ( const plTriangle* tri : triangles )
    {  
        // intersect triangle
        plIntersection intersection = tri->rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );           
        if (intersection.exists)
        {
            PLfloat tAbs = fabs(intersection.t);
            if ( tAbs < min) 
            {
                min = tAbs;
                closestIntersection = intersection;
            }
        }
    }

    // smooth intersection normal if specified
    if ( smoothNormal )
        closestIntersection.normal = getAverageNormal( PL_NORMAL_SMOOTHING_RADIUS, closestIntersection.point, closestIntersection.normal );

    return closestIntersection;  
}



