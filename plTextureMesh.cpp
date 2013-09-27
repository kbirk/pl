#include "plTextureMesh.h"

plTextureMesh::plTextureMesh() 
    : plMesh(), texture( 0, 0 )
{
}


plTextureMesh::plTextureMesh( PLuint width, PLuint height ) 
    : plMesh(), texture( width, height )
{
    // default ortho coords
    setBuffers( plVector3(-1,-1, 0), 
                plVector3( 1,-1, 0),
                plVector3( 1, 1, 0),
                plVector3(-1, 1, 0) );
}


plTextureMesh::plTextureMesh( PLuint width, PLuint height, const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft)              
    : plMesh(), texture( width, height )
{
	// set VBO and VAO
    setBuffers( bottemLeft, bottemRight, topRight, topLeft);
}


plTextureMesh::plTextureMesh( const plTextureMesh &mesh )
    : plMesh(), texture( mesh.texture )
{
    _copyMesh( mesh );
}


plTextureMesh& plTextureMesh::operator = ( const plTextureMesh &mesh ) 
{ 
    _copyMesh( mesh );
    texture = mesh.texture;
    return *this;
}


void plTextureMesh::setBuffers( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft)
{
    plSeq<plVector3> vertices(8);
    plSeq<PLuint>    indices(6);
    
    // position                  // texture coord
    vertices.add( bottemLeft  ); vertices.add( plVector3( 0,0,0) );   
    vertices.add( bottemRight ); vertices.add( plVector3( 1,0,0) );   
    vertices.add( topRight    ); vertices.add( plVector3( 1,1,0) );    
    vertices.add( topLeft     ); vertices.add( plVector3( 0,1,0) );
    
    indices.add( 0 );   indices.add( 1 );   indices.add( 2 );
    indices.add( 0 );   indices.add( 2 );   indices.add( 3 );

    // size of each vertex 
	const GLuint POS_SIZE = sizeof(GLfloat)*3;
	const GLuint TEX_SIZE = sizeof(GLfloat)*3;
    const GLuint TOTAL_SIZE = POS_SIZE + TEX_SIZE ;  
    const GLuint ARRAY_SIZE = TOTAL_SIZE * vertices.size()/2;
    
    // set number of indices
    _numIndices = 6;
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

    // set normal pointer, offset and stride
	glEnableVertexAttribArray(PL_TEXCOORD_ATTRIBUTE);
	glVertexAttribPointer(PL_TEXCOORD_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, (GLvoid*)(POS_SIZE));                                
     
    // create and bind index VBO
    if (_vertexBufferIndices == 0)
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

