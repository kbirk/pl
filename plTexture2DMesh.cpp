#include "plTexture2DMesh.h"

plTexture2DMesh::plTexture2DMesh() 
    : plMesh(), texture( 0, 0 )
{
}


plTexture2DMesh::plTexture2DMesh( PLuint width, PLuint height ) 
    : plMesh(), texture( width, height )
{
    // default ortho coords
    setBuffers( plVector3(-1,-1, 0), 
                plVector3( 1,-1, 0),
                plVector3( 1, 1, 0),
                plVector3(-1, 1, 0) );
}


plTexture2DMesh::plTexture2DMesh( PLuint width, PLuint height, const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft)              
    : plMesh(), texture( width, height )
{
	// set VBO and VAO
    setBuffers( bottemLeft, bottemRight, topRight, topLeft);
}


plTexture2DMesh::plTexture2DMesh( const plTexture2DMesh &mesh )
    : plMesh(), texture( mesh.texture )
{
    _copyMesh( mesh );
}


plTexture2DMesh& plTexture2DMesh::operator = ( const plTexture2DMesh &mesh ) 
{ 
    _copyMesh( mesh );
    texture = mesh.texture;
    return *this;
}


void plTexture2DMesh::setBuffers( const plVector3 &bottemLeft, const plVector3 &bottemRight, const plVector3 &topRight, const plVector3 &topLeft)
{
    std::vector<plVector3> vertices;    vertices.reserve(8);
    std::vector<PLuint>    indices;     indices.reserve(6);
    
    // position                  // texture coord
    vertices.push_back( bottemLeft  ); vertices.push_back( plVector3( 0,0,0) );   
    vertices.push_back( bottemRight ); vertices.push_back( plVector3( 1,0,0) );   
    vertices.push_back( topRight    ); vertices.push_back( plVector3( 1,1,0) );    
    vertices.push_back( topLeft     ); vertices.push_back( plVector3( 0,1,0) );
    
    indices.push_back( 0 );   indices.push_back( 1 );   indices.push_back( 2 );
    indices.push_back( 0 );   indices.push_back( 2 );   indices.push_back( 3 );

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


void plTexture2DMesh::draw() const
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

