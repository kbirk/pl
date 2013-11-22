#include "plVertexSpecBuffer.h"
#include "plVAO.h"

plVertexSpecBuffer::plVertexSpecBuffer()  
    :   _id( 0 ), _numBytes( 0 ) //, _changeFlag( false ), _vao( nullptr )  
{
}


plVertexSpecBuffer::~plVertexSpecBuffer()
{
    _destroy();
}

/*
void plVertexSpecBuffer::_flagChange()
{
    // flag change has happened
    _changeFlag = true;
    // if attached to vao, flag vao has changed
    if ( _vao )
        _vao->_changeFlag = true;
}
*/

void plVertexSpecBuffer::_destroy()
{
    glDeleteBuffers( 1, &_id );
    _numBytes = 0;
    _id = 0;
}

