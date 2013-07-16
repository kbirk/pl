#include "plLineMesh.h"

plLineMesh::plLineMesh() 
    : plMesh()
{
}

plLineMesh::plLineMesh(const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices)
{            
	// set VBO and VAO
    setBuffers(vertices, indices);
}


void plLineMesh::setBuffers( const plSeq<plVector3> &vertices, const plSeq<PLuint> &indices)
{
    // destroy previous just in case
    destroy();

    // set number of indices
    _numIndices = indices.size();

    // size of each vertex 
	const GLuint POS_SIZE = sizeof(GLfloat)*3;
    const GLuint TOTAL_SIZE = POS_SIZE;  
    const GLuint ARRAY_SIZE = TOTAL_SIZE * vertices.size();
    
    // create and bind VAO
	glGenVertexArrays(1, &_vertexArrayObject);     
	glBindVertexArray(_vertexArrayObject);
     
	// create and bind vertex VBO
	glGenBuffers(1, &_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); 
    glBufferData(GL_ARRAY_BUFFER, ARRAY_SIZE, &vertices[0], GL_STATIC_DRAW);    
       
    // set position pointer, offset and stride
	glEnableVertexAttribArray(PL_POSITION_ATTRIBUTE);
	glVertexAttribPointer(PL_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, 0); 

    // bind vertex array object
    glGenBuffers(1, &_vertexBufferIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBufferIndices);   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices*sizeof(PLuint), &indices[0], GL_STATIC_DRAW);
     
	// unbind the vertex array object
	glBindVertexArray(0); 			
}

void plLineMesh::draw() const
{		
    // use current shader and properly set uniforms
    plShaderStack::use();			
	// bind vertex array object
	glBindVertexArray(_vertexArrayObject);
	// draw batch
	glDrawElements( GL_LINES, _numIndices, GL_UNSIGNED_INT, 0); 
    // unbind VBO
	glBindVertexArray(0); 	    
}