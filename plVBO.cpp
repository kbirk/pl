#include "plVBO.h"

plVBO::plVBO()
{
}


plVBO::plVBO( const std::vector<plVector4>& data, PLuint usage )
{
    set( data, usage );
}


plVBO::plVBO( const plVBO& vbo )
{
    _copy( vbo ); 
}  

     
plVBO::plVBO( plVBO&& vbo )
{
    _move( std::move( vbo ) );
}


plVBO& plVBO::operator = ( const plVBO &vbo ) 
{ 
    _copy( vbo );
    return *this;
}


plVBO& plVBO::operator= ( plVBO&& vbo )
{
    _move( std::move( vbo ) );
    return *this;
}


void plVBO::set( const std::vector<plVector4>& data, PLuint usage )
{
    _data      = data;
    _numBytes  = sizeof( plVector4 ) * _data.size();
    _usage     = usage;   
}


void plVBO::set( const std::vector<plVector3>& data, PLuint usage )
{
    set( std::vector<plVector4>( data.begin(), data.end() ), usage );
}


void plVBO::set( const plVertexAttributePointer& attributePointer )
{
    _attributePointers.push_back( attributePointer );
}


void plVBO::clear()
{
    _data.clear();
}


void plVBO::upload()
{
    if ( _data.empty() )
    {
        std::cerr << "plVBO::upload() error: data buffer empty, ignoring command" << std::endl;
        return;
    }
      
    if ( _attributePointers.empty() )
    {
        std::cerr << "plVBO::upload() error: vertex attribute pointers unspecified, ignoring command" << std::endl;
        return;        
    }

    // if buffer not allocated, generate
	if ( !_id )
	    glGenBuffers( 1, &_id );
    
    glBindBuffer( GL_ARRAY_BUFFER, _id ); 
    glBufferData( GL_ARRAY_BUFFER, _numBytes, &_data[0], _usage );    

    for ( const plVertexAttributePointer& attributePointer : _attributePointers )
    {   
        // set position pointer, offset and stride
	    glEnableVertexAttribArray( attributePointer.index );
	    glVertexAttribPointer( attributePointer.index, 4, GL_FLOAT, GL_FALSE, attributePointer.stride, (GLvoid*)( attributePointer.offset ) ); 
    }
    
    // do not unbind buffer as this will most likely be called from a vao
}
 

void plVBO::_copy( const plVBO &vbo )
{
    set( vbo._data, vbo._usage );
    _attributePointers = vbo._attributePointers;
}


void plVBO::_move( plVBO&& vbo )
{
    _id = vbo._id;
    _numBytes = vbo._numBytes;
    _attributePointers = vbo._attributePointers;
    _usage = vbo._usage;
    _data = std::move( vbo._data );
    
    vbo._id = 0;   
    vbo._numBytes = 0;  
    vbo._usage = 0;
}

