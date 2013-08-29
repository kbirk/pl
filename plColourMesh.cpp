#include "plColourMesh.h"

plColourMesh::plColourMesh() 
    : plMesh()
{
}


plColourMesh::plColourMesh(const plSeq<plVector3> &interleaved_vertices, const plSeq<PLuint> &indices) 
{            	
	// set VBO and VAO
    setBuffers(interleaved_vertices, indices);
}


void plColourMesh::setBuffers( const plSeq<plVector3> &interleaved_vertices, const plSeq<PLuint> &indices)
{
    // destroy incase already set
    //destroy();

    // set index count
	_numIndices = indices.size();

    // size of each vertex 
	const GLuint POS_SIZE = sizeof(GLfloat)*3;
	const GLuint NOR_SIZE = sizeof(GLfloat)*3;
	const GLuint COL_SIZE = sizeof(GLfloat)*3;
    const GLuint TOTAL_SIZE = POS_SIZE + NOR_SIZE + COL_SIZE;  
    const GLuint ARRAY_SIZE = TOTAL_SIZE * interleaved_vertices.size()/3;
    
    // create and bind VAO
    if (_vertexArrayObject == 0)
	    glGenVertexArrays(1, &_vertexArrayObject);  

	glBindVertexArray(_vertexArrayObject);
     
	// create and bind vertex VBO
	if (_vertexBufferObject == 0)
	    glGenBuffers(1, &_vertexBufferObject);
	    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); 
    glBufferData(GL_ARRAY_BUFFER, ARRAY_SIZE, &interleaved_vertices[0], GL_STREAM_DRAW);    
       
    // set position pointer, offset and stride
	glEnableVertexAttribArray(PL_POSITION_ATTRIBUTE);
	glVertexAttribPointer(PL_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, 0); 

    // set normal pointer, offset and stride
	glEnableVertexAttribArray(PL_NORMAL_ATTRIBUTE);
	glVertexAttribPointer(PL_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, (GLvoid*)(POS_SIZE));                                
     
    // set colour pointer, offset and stride
	glEnableVertexAttribArray(PL_COLOUR_ATTRIBUTE);
	glVertexAttribPointer(PL_COLOUR_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, (GLvoid*)(POS_SIZE+NOR_SIZE));                                
           
    // bind vertex array object
    if (_vertexBufferIndices == 0)
        glGenBuffers(1, &_vertexBufferIndices);
        
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBufferIndices);   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices*sizeof(PLuint), &indices[0], GL_STREAM_DRAW);
     
	// unbind the vertex array object
	glBindVertexArray(0); 			
}

