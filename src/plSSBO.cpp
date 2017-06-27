#include "plSSBO.h"

plSSBO::plSSBO()
{
}

plSSBO::plSSBO(uint32_t numBytes, const void* buffer)
{
    _create(numBytes, buffer);
}

void plSSBO::_create(uint32_t numBytes, const void* buffer)
{
    // set bytes
    _numBytes = numBytes;
    // generate handle if needed
    if (!_id) {
        glGenBuffers(1, &_id);
        LOG_OPENGL("glGenBuffers");
    }

    // bind and buffer data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id);
    LOG_OPENGL("glBindBuffer");

    if (!buffer) {
        // initialize empty buffer to all 0's
        uint32_t* temp = new uint32_t[numBytes];
        memset(temp, 0, numBytes);
        glBufferData(GL_SHADER_STORAGE_BUFFER, numBytes, temp, GL_STREAM_COPY);
        LOG_OPENGL("glBufferData");
        delete[] temp;
    } else {
        glBufferData(GL_SHADER_STORAGE_BUFFER, numBytes, buffer, GL_STREAM_COPY);
        LOG_OPENGL("glBufferData");
    }

    // unbind
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    LOG_OPENGL("glBindBuffer");
}
