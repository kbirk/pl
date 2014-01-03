#include "plBufferObject.h"

plBufferObject::plBufferObject()  
    :   _id( 0 ), _numBytes( 0 ) 
{
}


plBufferObject::~plBufferObject()
{
    _destroy();
}


void plBufferObject::_destroy()
{
    glDeleteBuffers( 1, &_id );
    _numBytes = 0;
    _id = 0;
}

