#include "plMesh.h"

plMesh::plMesh() 
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{
}

plMesh::plMesh(const plSeq<plTriangle> &triangles)
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{
	// convert triangles to interleaved
	triangleToInterleaved(triangles);
}

plMesh::plMesh(const plSeq<plVector3> &interleaved_vertices, const plSeq<PLuint> &indices)
    : _vertexBufferObject(0), _vertexBufferIndices(0), _vertexArrayObject(0)
{            
	// set VBO and VAO
    setBuffers(interleaved_vertices, indices);
}

void plMesh::destroy()
{            
    glDeleteBuffers(1, &_vertexBufferObject);	    // delete buffer objects
    glDeleteBuffers(1, &_vertexBufferIndices);      // delete indices
	glDeleteVertexArrays(1, &_vertexArrayObject);	// delete VAO		    
}

void plMesh::triangleToInterleaved(const plSeq<plTriangle> &triangles)
{			
	// convert to interleaved format
	plSeq<plVector3> interleaved_vertices( triangles.size() * 3 * 2 );
	plSeq<PLuint>    indices             ( triangles.size() * 3);
	
    int indexCount = 0;
    for (PLuint i = 0; i < triangles.size(); i++) 
    {  
        // p1
	    interleaved_vertices.add(triangles[i].point0());
	    interleaved_vertices.add(triangles[i].normal());
	    indices.add(indexCount++);
	    // p2
	    interleaved_vertices.add(triangles[i].point1());
	    interleaved_vertices.add(triangles[i].normal());
	    indices.add(indexCount++);
	    // p3
	    interleaved_vertices.add(triangles[i].point2());
	    interleaved_vertices.add(triangles[i].normal());
	    indices.add(indexCount++);	    
	}

    setBuffers(interleaved_vertices, indices);
}

void plMesh::setBuffers( const plSeq<plVector3> &interleaved_vertices, const plSeq<PLuint> &indices)
{
    // destroy previous just in case
    //destroy();

    // set number of indices
    _numIndices = indices.size();

    // size of each vertex 
	const GLuint POS_SIZE = sizeof(GLfloat)*3;
	const GLuint NOR_SIZE = sizeof(GLfloat)*3;
    const GLuint TOTAL_SIZE = POS_SIZE + NOR_SIZE ;  
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
	glEnableVertexAttribArray(PL_NORMAL_ATTRIBUTE);
	glVertexAttribPointer(PL_NORMAL_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, TOTAL_SIZE, (GLvoid*)(POS_SIZE));                                
     
    // bind vertex array object
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

void plMesh::draw(const plSeq<PLuint> &indices) const
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

