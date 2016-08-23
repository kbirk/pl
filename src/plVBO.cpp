#include "plVBO.h"

plVBO::plVBO()
{
}


plVBO::plVBO(const std::vector<plVector4>& data, uint32_t usage)
{
    set(data, usage);
}


void plVBO::set(const std::vector<plVector4>& data, uint32_t usage)
{
    _data = data;
    _numBytes = sizeof(plVector4) * _data.size();
    _usage = usage;
}


void plVBO::set(const std::vector<plVector3>& data, uint32_t usage)
{
    set(std::vector<plVector4>(data.begin(), data.end()), usage);
}


void plVBO::set(const plVertexAttributePointer& attributePointer)
{
    _attributePointers.push_back(attributePointer);
}


void plVBO::clear()
{
    _data.clear();
}


void plVBO::upload()
{
    if (_data.empty())
    {
        LOG_WARN("Data buffer empty, ignoring command");
        return;
    }

    if (_attributePointers.empty())
    {
        LOG_WARN("Vertex attribute pointers unspecified, ignoring command");
        return;
    }

    // if buffer not allocated, generate
    if (!_id)
        glGenBuffers(1, &_id);

    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, _numBytes, &_data[0], _usage);

    for (auto attributePointer : _attributePointers)
    {
        // set position pointer, offset and stride
        glEnableVertexAttribArray(attributePointer.index);
        glVertexAttribPointer(attributePointer.index, 4, GL_FLOAT, GL_FALSE, attributePointer.stride, ((GLbyte*)(nullptr) + (attributePointer.offset)));
    }

    // do not unbind buffer as this will most likely be called from a vao
}
