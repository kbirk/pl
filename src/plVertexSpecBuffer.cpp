#include "plVertexSpecBuffer.h"
#include "plVAO.h"

plVertexSpecBuffer::plVertexSpecBuffer()
    :   _id(0), _numBytes(0)
{
}


plVertexSpecBuffer::~plVertexSpecBuffer()
{
    _destroy();
}


void plVertexSpecBuffer::_destroy()
{
    glDeleteBuffers(1, &_id);
    _numBytes = 0;
    _id = 0;
}
