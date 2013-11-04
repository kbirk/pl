#ifndef PL_MESH_H
#define PL_MESH_H

#include "plCommon.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plShader.h" // attribute enums
#include "plRenderingPipeline.h"

class plMesh
{
    public:
		
		plMesh();		
		plMesh( const std::vector<plTriangle> &triangles );        
        plMesh( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices );       
        plMesh( const plMesh &mesh );
        virtual ~plMesh();
        
        plMesh& operator = ( const plMesh &mesh ); 

        virtual void setBuffers( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices);
        virtual void draw() const;	        	
		virtual void draw( const std::vector<PLuint> &indices ) const;    // draw with newly defined index order ( for transparency sorting )
        
    protected:

        GLuint _numIndices;             // num indices needed by glDrawElements
        GLuint _numBytes;               // size of vertex buffer

        GLuint _vertexBufferIndices;
        GLuint _vertexBufferObject;	    // vertex buffer objects
		GLuint _vertexArrayObject;	    // vertex array object
		
		virtual void _destroy();
		virtual void _copyMesh( const plMesh &mesh );
		virtual void _triangleToInterleaved( const std::vector<plTriangle> &triangles );
        
		

};

#endif
