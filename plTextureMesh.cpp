#include "plTextureMesh.h"

plTextureMesh::plTextureMesh() 
    : plMesh()
{
}


plTextureMesh::plTextureMesh( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft)              
    : plMesh()
{
	// set VBO and VAO
    setBuffers( bottemLeft, bottemRight, topRight, topLeft);
}


void plTextureMesh::setBuffers( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft)
{
    plSeq<plVector3> interleaved_vertices(8);
    plSeq<PLuint>    indices(6);
    
    interleaved_vertices.add( bottemLeft );
    interleaved_vertices.add( plVector3( 0,0,0) );
    
    interleaved_vertices.add( bottemRight );
    interleaved_vertices.add( plVector3( 1,0,0) );
    
    interleaved_vertices.add( topRight );
    interleaved_vertices.add( plVector3( 1,1,0) );
    
    interleaved_vertices.add( topLeft );
    interleaved_vertices.add( plVector3( 0,1,0) );
    
    indices.add( 0 );   indices.add( 1 );   indices.add( 2 );
    indices.add( 0 );   indices.add( 2 );   indices.add( 3 );

    // set number of indices
    _numIndices = 6;

    // size of each vertex 
	const GLuint POS_SIZE = sizeof(GLfloat)*3;
	const GLuint TEX_SIZE = sizeof(GLfloat)*3;
    const GLuint TOTAL_SIZE = POS_SIZE + TEX_SIZE ;  
    const GLuint ARRAY_SIZE = TOTAL_SIZE * interleaved_vertices.size()/2;
    
    // create and bind VAO
    if (_vertexArrayObject == 0)
	    glGenVertexArrays(1, &_vertexArrayObject);   
	      
	glBindVertexArray(_vertexArrayObject);
     
	// create and bind vertex VBO
	if (_vertexBufferObject == 0)
	    glGenBuffers(1, &_vertexBufferObject);
	    
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject); 
    glBufferData(GL_ARRAY_BUFFER, ARRAY_SIZE, &interleaved_vertices[0], GL_STATIC_DRAW);    
       
    // set position pointer, offset and stride
	glEnableVertexAttribArray(PL_POSITION_ATTRIBUTE);
	glVertexAttribPointer(PL_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, 0); 

    // set normal pointer, offset and stride
	glEnableVertexAttribArray(PL_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(PL_TEXCOORD_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, (GLvoid*)(POS_SIZE));                                
     
    // bind vertex array object
    glGenBuffers(1, &_vertexBufferIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBufferIndices);   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices*sizeof(PLuint), &indices[0], GL_STATIC_DRAW);
     
	// unbind the vertex array object
	glBindVertexArray(0); 			
}

void plTextureMesh::draw() const
{	
    // bind texture
    texture.bind();  	
    // use current shader and properly set uniforms
    plShaderStack::use();	      		
	// bind vertex array object
	glBindVertexArray(_vertexArrayObject);
	// draw batch
	glDrawElements( GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0); 
    // unbind VBO
	glBindVertexArray(0); 	
	// unbind texture
	texture.unbind();    
}

