#ifndef _PL_MESH_H_
#define _PL_MESH_H_ 

#include "plCommon.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plShader.h" // attribute enums
#include "plRenderingPipeline.h"

class plMesh
{
    public:
		
		plMesh();		
		plMesh(const plSeq<plTriangle> &triangles);        
        plMesh(const plSeq<plVector3> &interleaved_vertices, const plSeq<PLuint> &indices);
        plMesh(float radius, int slices, int stacks);                                           // sphere
        plMesh(float baseRadius, float topRadius, float height, int slices, int stacks);        // cylinder
        plMesh(float innerRadius, float outerRadius, int slices, int loops, bool up = true);    // disk
        
        void triangleToInterleaved(const plSeq<plTriangle> &triangles);
        void destroy();       
        
        virtual void setBuffers( const plSeq<plVector3> &interleaved_vertices, const plSeq<PLuint> &indices);

        void draw() const;		
		void draw(const std::vector<plOrderPair> &order) const;
        
    protected:

        GLuint _numIndices;

        GLuint _vertexBufferIndices;
        GLuint _vertexBufferObject;	    // vertex buffer objects
		GLuint _vertexArrayObject;	    // vertex array object

};

#endif
