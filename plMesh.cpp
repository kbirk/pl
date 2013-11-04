#include "plMesh.h"

plMesh::plMesh() 
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{
}


plMesh::plMesh(const std::vector<plTriangle> &triangles)
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{
	// convert triangles to interleaved
	_triangleToInterleaved(triangles);
}


plMesh::plMesh(const std::vector<plVector3> &vertices, const std::vector<PLuint> &indices)
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{            
	// set VBO and VAO
    setBuffers(vertices, indices);
}


plMesh::~plMesh()
{
    _destroy();
}


plMesh::plMesh( const plMesh &mesh )
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{
    _copyMesh( mesh );
}


plMesh& plMesh::operator = ( const plMesh &mesh ) 
{ 
    _copyMesh( mesh );
    return *this;
}


void plMesh::_copyMesh( const plMesh &mesh )
{
    std::vector<plVector3> vertices( mesh._numBytes / sizeof( plVector3 ), plVector3() );
    std::vector<PLuint>    indices ( mesh._numIndices, 0 );

    // copy vertex data
    glBindBuffer( GL_ARRAY_BUFFER, mesh._vertexBufferObject ); 
    
    PLfloat *vertexBuffer = (PLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );      
    memcpy( &vertices[0].x, vertexBuffer, mesh._numBytes );
    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    // copy index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._vertexBufferIndices );
    
    PLuint *indexBuffer = (PLuint*)glMapBuffer( GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY );      
    memcpy( &indices[0], indexBuffer, mesh._numIndices * sizeof( PLuint ) );
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 

    // destroy previous buffers
    _destroy();

    // set VBO and VAO
    setBuffers( vertices, indices );  
}


void plMesh::_destroy()
{     
    glDeleteBuffers     (1, &_vertexBufferObject);  // delete buffer objects
    glDeleteBuffers     (1, &_vertexBufferIndices); // delete indices  
	glDeleteVertexArrays(1, &_vertexArrayObject);	// delete VAO		
	
	_vertexBufferObject  = 0;
    _vertexBufferIndices = 0;
    _vertexArrayObject   = 0;    
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
    
    // size of each vertex 
	const GLuint POS_SIZE = sizeof( plVector3 );
	const GLuint NOR_SIZE = sizeof( plVector3 );
    const GLuint TOTAL_SIZE = POS_SIZE + NOR_SIZE ;  
    const GLuint ARRAY_SIZE = TOTAL_SIZE * vertices.size()/2;
    
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

    // set normal pointer, offset and stride
	glEnableVertexAttribArray(PL_NORMAL_ATTRIBUTE);
	glVertexAttribPointer(PL_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, (GLvoid*)(POS_SIZE));                                
     
    // create and bind index VBO
    if (_vertexBufferIndices == 0)
        glGenBuffers(1, &_vertexBufferIndices);
        
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexBufferIndices);   
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices*sizeof(PLuint), &indices[0], GL_STREAM_DRAW);
     
	// unbind the vertex array object
	glBindVertexArray(0);
}


void plMesh::draw() const
{	
    // use current shader and properly set uniforms
    plShaderStack::use();			
	// bind vertex array object
	glBindVertexArray(_vertexArrayObject);
	// draw batch
	glDrawElements( GL_TRIANGLES, _numIndices, GL_UNSIGNED_INT, 0); 
    // unbind VBO
	glBindVertexArray(0); 	    
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

