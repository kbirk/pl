#include "plOctreeMesh.h"

plOctreeMesh::plOctreeMesh() 
{
}


plOctreeMesh::plOctreeMesh( const std::vector<plTriangle> &triangles, PLuint depth )
    : plMesh( triangles )
{
    _buildOctree( depth );
}

		 
plOctreeMesh::plOctreeMesh( std::vector<plTriangle>&& triangles, PLuint depth )
    : plMesh( std::move( triangles ) )
{ 
    _buildOctree( depth );
} 

           
plOctreeMesh::plOctreeMesh( const plOctreeMesh &mesh )
    : plMesh( mesh._triangles )
{
    _buildOctree( mesh.octree().depth() );  // must build a new octree to maintain proper coherency
}


plOctreeMesh::plOctreeMesh( plOctreeMesh&& mesh )
    : plMesh( std::move( mesh._triangles ) ), _octree( std::move( mesh._octree ) )
{
}      


plOctreeMesh& plOctreeMesh::operator= ( const plOctreeMesh& mesh ) 
{ 
    _triangles = mesh._triangles;
    _buildOctree( mesh.octree().depth() ); // must build a new octree to maintain proper coherency
    return *this;
}


plOctreeMesh& plOctreeMesh::operator= ( plOctreeMesh&& mesh ) 
{ 
    _triangles = std::move( mesh._triangles );
    _octree    = std::move( mesh._octree );
    return *this;
}


void plOctreeMesh::_buildOctree( PLuint depth )
{
    // get min and max extents of model
    plVector3 min, max;
    getMinMax(min,max);

    // build octree
    _octree.build( min, max, _triangles, depth ); 
}


plVector3 plOctreeMesh::getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const
{
    // get potential triangles in radius from octree
    plSet<const plTriangle*> triangles;   
    _octree.rayIntersect( triangles, origin, -up, radius );

    plVector3 normal(0,0,0);
    PLint count = 0;
    float radiusSquared = radius * radius;
    
    // Find polygons on top of graft
    for ( const plTriangle* triangle : triangles ) 
    {
        if ( triangle->normal() * up > 0.001)
        {        
            PLfloat dist1 = ( triangle->point0() - origin ).squaredLength();
            PLfloat dist2 = ( triangle->point1() - origin ).squaredLength();
            PLfloat dist3 = ( triangle->point2() - origin ).squaredLength();
           
            // if any point of triangle is withing radial sphere, accept
            float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

            if (minDist <= radiusSquared)
            {        
                normal = normal + triangle->normal();
                count++;
            }
        }
    } 

    if (count == 0)
    {
        // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
        std::cout << "plOctreeMesh::getAverageNormal() warning: No normal found" << std::endl;
        return up;
    }    

    return ( 1.0f/ (PLfloat)(count) * normal ).normalize();
}


plIntersection plOctreeMesh::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool smoothNormal, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    // get potential triangles from octree
    plSet<const plTriangle*> triangles;   
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

















/*
void plOctreeMesh::_copyMesh( const plOctreeMesh &mesh )
{
    _vao = mesh._vao;
}



void plOctreeMesh::_triangleToInterleaved(const std::vector<plTriangle> &triangles)
{			
	// convert to interleaved format
	std::vector<plVector3> vertices;    vertices.reserve( triangles.size() * 3 * 2 );
	std::vector<PLuint>    indices;     indices.reserve( triangles.size() * 3);
	
    int indexCount = 0;
    for (PLuint i = 0; i < triangles.size(); i++) 
    {  
        // p1
	    vertices.push_back(triangles[i].point0());    // position
	    vertices.push_back(triangles[i].normal());    // normal
	    indices.push_back(indexCount++);
	    // p2
	    vertices.push_back(triangles[i].point1());
	    vertices.push_back(triangles[i].normal());
	    indices.push_back(indexCount++);
	    // p3
	    vertices.push_back(triangles[i].point2());
	    vertices.push_back(triangles[i].normal());
	    indices.push_back(indexCount++);	    
	}

    setBuffers(vertices, indices);
}


void plOctreeMesh::setBuffers( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices)
{
    if ( vertices.size() < 3 || indices.size() < 3 )
    {
        std::cerr << "plOctreeMesh::setBuffers() error: not enough vertices or indices " << std::endl;
    }
    
    
    std::vector<PLuint> attributeTypes;
    attributeTypes.push_back( PL_POSITION_ATTRIBUTE );
    attributeTypes.push_back( PL_NORMAL_ATTRIBUTE );
        
    _vao.set( std::vector<plVector4>( vertices.begin(), vertices.end() ), attributeTypes, indices );
}


void plOctreeMesh::draw() const
{	
	// use current shader and properly set uniforms
    plShaderStack::use();	
    // bind and draw elements from AVO
    _vao.draw(); 
       
}


void plOctreeMesh::draw(const std::vector<PLuint> &indices) const
{    
    // use current shader and properly set uniforms
    plShaderStack::use();
    // bind vertex array object
	glBindVertexArray(_vertexArrayObject);		
    // buffer new index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(PLuint), &indices[0], GL_STREAM_DRAW);        
	// draw batch
	glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
    // unbind VBO
	glBindVertexArray(0); 
}
*/


