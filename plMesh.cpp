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


plVector3 plMesh::getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const
{
    plVector3 normal(0,0,0);
    PLint count = 0;
    float radiusSquared = radius * radius;
    
    // Find polygons on top of graft
    for ( const plTriangle& triangle : _triangles ) 
    {
        if ( triangle.normal() * up > 0.001)
        {        
            PLfloat dist1 = ( triangle.point0() - origin ).squaredLength();
            PLfloat dist2 = ( triangle.point1() - origin ).squaredLength();
            PLfloat dist3 = ( triangle.point2() - origin ).squaredLength();
           
            // if any point of triangle is withing radial sphere, accept
            float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

            if (minDist <= radiusSquared)
            {        
                normal = normal + triangle.normal();
                count++;
            }
        }
    } 

    if (count == 0)
    {
        // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
        std::cout << "plMesh::getAverageNormal() warning: No normal found" << std::endl;
        return up;
    }    

    return (1.0f/(PLfloat)(count) * normal).normalize();
}


plIntersection plMesh::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool smoothNormal, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    PLfloat min = FLT_MAX;
    plIntersection closestIntersection( false );

    for ( const plTriangle& triangle : _triangles ) 
    {  
        plIntersection intersection = triangle.rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );
        
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
void plMesh::_copyMesh( const plMesh &mesh )
{
    _vao = mesh._vao;
}



void plMesh::_triangleToInterleaved(const std::vector<plTriangle> &triangles)
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


void plMesh::setBuffers( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices)
{
    if ( vertices.size() < 3 || indices.size() < 3 )
    {
        std::cerr << "plMesh::setBuffers() error: not enough vertices or indices " << std::endl;
    }
    
    
    std::vector<PLuint> attributeTypes;
    attributeTypes.push_back( PL_POSITION_ATTRIBUTE );
    attributeTypes.push_back( PL_NORMAL_ATTRIBUTE );
        
    _vao.set( std::vector<plVector4>( vertices.begin(), vertices.end() ), attributeTypes, indices );
}


void plMesh::draw() const
{	
	// use current shader and properly set uniforms
    plShaderStack::use();	
    // bind and draw elements from AVO
    _vao.draw(); 
       
}


void plMesh::draw(const std::vector<PLuint> &indices) const
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


