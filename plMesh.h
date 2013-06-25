#ifndef _PL_MESH_H_
#define _PL_MESH_H_ 

#include "pl.h"
#include "plVector3.h"
#include "plTriangle.h"
#include "plShader.h" // attribute enums

#define PL_POSITION_ATTRIBUTE  0
#define PL_NORMAL_ATTRIBUTE    2
#define PL_COLOUR_ATTRIBUTE    3

class plMesh
{
    public:
		
		plMesh();		
		plMesh(const plSeq<plTriangle> &triangles);        
        plMesh(const plSeq<plVector3> &interleaved_vertices, const plSeq<unsigned int> &indices);

        void triangleToInterleaved(const plSeq<plTriangle> &triangles);
        void destroy();

        void generateSphere(float radius, int slices, int stacks);
        void generateCylinder(float baseRadius, float topRadius, float height, int slices, int stacks);
        void generateDisk(float innerRadius, float outerRadius, int slices, int loops, bool up = true);

        virtual void setBuffers( const plSeq<plVector3> &interleaved_vertices, const plSeq<unsigned int> &indices);

        void draw() const;		
		void draw(const std::vector<plOrderPair> &order) const;
        
    protected:

        GLuint _numIndices;

        GLuint _vertexBufferIndices;
        GLuint _vertexBufferObject;	    // vertex buffer objects
		GLuint _vertexArrayObject;	    // vertex array object

};

#endif
