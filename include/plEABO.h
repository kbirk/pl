#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"
#include "plOpenGLInfo.h"
#include "plVertexSpecBuffer.h"

class plEABO : public plVertexSpecBuffer {
public:
    plEABO();
    plEABO(
        const std::vector<uint32_t>& data,
        uint32_t mode = GL_TRIANGLES,
        uint32_t usage = GL_STATIC_DRAW);

    std::vector<uint32_t> data() const { return _data; }

    void set(
        const std::vector<uint32_t>& data,
        uint32_t mode = GL_TRIANGLES,
        uint32_t usage = GL_STATIC_DRAW);
    void upload();
    void clear();

    void bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
        LOG_OPENGL("glBindBuffer");
    }
    void unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        LOG_OPENGL("glBindBuffer");
    }

    void drawElements(uint32_t index = 0) const;

private:
    std::vector<uint32_t> _data;
    uint32_t _type;
    uint32_t _mode;
};
