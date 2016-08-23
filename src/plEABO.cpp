#include "plEABO.h"

plEABO::plEABO()
{
}


plEABO::plEABO(const std::vector<GLuint>& data, uint32_t mode, uint32_t usage)
{
    set(data, mode, usage);
}


void plEABO::set(const std::vector<uint32_t>& data, uint32_t mode, uint32_t usage)
{
    _data = data;
    _numBytes = _data.size() * sizeof(uint32_t);
    _type = GL_UNSIGNED_INT;
    _mode = mode;
    _usage = usage;
}


void plEABO::clear()
{
    _data.clear();
}


void plEABO::upload()
{
    if (_data.empty())
    {
        std::cerr << "plEABO::upload() error: data buffer empty, ignoring command" << std::endl;
        return;
    }

    // if buffer not allocated, generate
    if (!_id)
        glGenBuffers(1, &_id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numBytes, &_data[0], _usage);

    // do not unbind buffer as this will most likely be called from a vao
}


void plEABO::drawElements(uint32_t index) const
{
    glDrawElements(_mode, _data.size(), _type, (GLbyte*)(nullptr) + (index));
}
