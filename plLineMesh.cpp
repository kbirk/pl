#include "plLineMesh.h"

plLineMesh::plLineMesh() 
    : plMesh()
{
}


plLineMesh::plLineMesh(const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices)
    : plMesh()
{            
	// set VBO and VAO
    setBuffers(vertices, indices);
}


plLineMesh::plLineMesh( const plLineMesh &mesh )
    : plMesh()
{
    _copyMesh( mesh );
}


plLineMesh& plLineMesh::operator = ( const plLineMesh &mesh ) 
{ 
    _copyMesh( mesh );
    return *this;
}


void plLineMesh::setBuffers( const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices)
{
    // size of each vertex 
	const GLuint POS_SIZE = sizeof( plVector3 );
    const GLuint TOTAL_SIZE = POS_SIZE;  
    const GLuint ARRAY_SIZE = TOTAL_SIZE * vertices.size();
    
    // set number of indices
    _numIndices = indices.size();
    // set buffer size
    _numBytes = ARRAY_SIZE;
    
    // create and bind VAO
    if (_vertexArrayObject == 0)
	    glGenVertexArrays(1, &_vertexArrayObject);
	         
	glBindVertexArray(_vertexArrayObject);
     
	// create and bind vertex VBO
	if (_vertexBufferObject == 0)
	    glGenBuffers(1, &_vertexBufferObject);
	    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); 
    glBufferData(GL_ARRAY_BUFFER, ARRAY_SIZE, &vertices[0], GL_STATIC_DRAW);    
       
    // set position pointer, offset and stride
	glEnableVertexAttribArray(PL_POSITION_ATTRIBUTE);
	glVertexAttribPointer(PL_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, 0); 

    // create and bind index VBO
    if (_vertexBufferIndices == 0)
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
